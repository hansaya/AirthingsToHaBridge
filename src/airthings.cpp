#include "airthings.h"
#include "managed_wifi.h"
#include "mqtt.h"
#include <ArduinoJson.h>
#include "log.h"
#include "ble.h"
#include "config.h"

Airthings::Airthings(const BLEAdvertisedDevice device, BLEUUID serviceId)
    : m_event(false), m_device(device), m_announced(false), m_serviceId(serviceId)
{
    m_serialNo = parseSerial(m_device.getManufacturerData());
    m_modelNo = parseModelNumber(m_device.getManufacturerData());
}

void Airthings::loop()
{
    const unsigned long currentMillis = millis(); // Time now
    // Publish data every 60 secs
    static unsigned long sensorDataPublishPeriod;
    if (currentMillis - sensorDataPublishPeriod >= READ_WAIT_SECONDS * 1000)
    {
        sensorDataPublishPeriod = currentMillis;

        if (getSensorReading())
        {
            g_log.write(Log::Debug, "Airthings: Reading complete. Sleeping for " + String(READ_WAIT_SECONDS) + " seconds before taking another reading.");
            // timeToSleep = READ_WAIT_SECONDS;
        }
        else
        {
            g_log.write(Log::Error, "Airthings: Reading FAILED. Sleeping for 60 seconds before retrying.");
            // timeToSleep = READ_WAIT_RETRY_SECONDS;
        }
    }

    if (m_event && g_mqtt.connected())
    {
        m_event = false;
        if (!m_announced)
            announce();
        publishStatus();
    }
}

void Airthings::begin()
{
    snprintf(m_topicMQTTHeader, 50, "%s/sensor/%s", g_config.getConfig()["ha_prefix"].as<const char *>(), g_managedWiFi.getHostName().c_str());

    if (getSensorReading())
    {
        g_log.write(Log::Debug, "Airthings: Device found, Model: " + m_modelNo + " Serial: " + m_serialNo + ". Initlizing...");
        announce();
        m_announced = true;
    }
    else
        g_log.write(Log::Error, "Airthings: Could not retrive the device meta data");
}

bool Airthings::connect()
{
    g_log.write(Log::Debug, "Airthings: Connecting to " + String(m_device.getAddress().toString().c_str()) + "...");

    // Connect to the remove BLE Server.
    if (!g_ble.getClient()->connect(&m_device))
    {
        g_log.write(Log::Error, "Airthings: Failed to connect..");
        disconnect();
        return false;
    }
    g_log.write(Log::Debug, "Airthings: Connected!");
    return true;
}

void Airthings::disconnect()
{
    if (g_ble.getClient()->isConnected())
        g_ble.getClient()->disconnect();
    g_log.write(Log::Debug, "Airthings: Disconnected!");
}

bool Airthings::getSensorReading()
{
    if (!connect())
        return false;

    BLERemoteService *pRemoteService = g_ble.getClient()->getService(m_serviceId);
    if (pRemoteService == nullptr)
    {
        g_log.write(Log::Error, "Airthings: Airthings refused its data service UUID.");
        disconnect();
        return false;
    }

    // Get references to our characteristics
    g_log.write(Log::Debug, "Airthings: Reading the sensors...");
    BLERemoteCharacteristic *remoteData;
    if (g_ble.getClient()->isConnected())
        remoteData = pRemoteService->getCharacteristic(getCharacteristicID());

    if (remoteData && remoteData->canRead() && remoteData->readValue().length() > 0)
        parseData(remoteData);
    else
    {
        g_log.write(Log::Error, "Airthings: Failed to find our characteristic UUID");
        disconnect();
        return false;
    }

    disconnect();
    return true;
}

void Airthings::mqttAnnounce(const String &name, const String &unit, const String &id) const
{
    // Generate a unique id for hassio to keep track.
    char uniqId[80];
    snprintf(uniqId, 80, "%s%s", m_serialNo.c_str(), id.c_str());

    // Topic to push sensor config. Path needs to be unique.
    char statusDiscoverTopic[80];
    snprintf(statusDiscoverTopic, 80, "%s/%s/config", m_topicMQTTHeader, uniqId);

    char val[80];
    snprintf(val, 80, "{{value_json.%s}}", id.c_str());

    StaticJsonDocument<500> root;
    root["~"] = m_topicMQTTHeader;
    root["uniq_id"] = uniqId;
    root["name"] = name ;
    root["avty_t"] = g_mqtt.getAvailabilityTopic();
    root["stat_t"] = "~/" + m_serialNo + "/state";
    root["unit_of_meas"] = unit;
    root["val_tpl"] = val;
    root["qos"] = MQTT_QOS;
    root["device"]["ids"] = m_serialNo;
    root["device"]["name"] = "Airthings " + m_modelNo;
    root["device"]["mf"] = "Airthings";
    root["device"]["mdl"] = m_modelNo;
    root["device"]["sw"] = "1";
    char outgoingJsonBuffer[500];
    serializeJson(root, outgoingJsonBuffer);
    g_mqtt.publishToMQTT(statusDiscoverTopic, outgoingJsonBuffer);
}

uint32_t Airthings::parseSerial(std::string manuData)
{
    uint32_t sn = manuData[2];
    sn |= ((uint32_t)manuData[3] << 8);
    sn |= ((uint32_t)manuData[4] << 16);
    sn |= ((uint32_t)manuData[5] << 24);
    return sn;
}

long Airthings::parseModelNumber(std::string manuData)
{
    return String(parseSerial(manuData)).substring(0, 4).toInt();
}