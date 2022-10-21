#include "wave_mini.h"
#include "managed_wifi.h"
#include "mqtt.h"
#include <ArduinoJson.h>
#include "log.h"
#include "ble.h"

void WaveMini::begin()
{
   Airthings::begin();
}

void WaveMini::announce() const
{
   // Publish auto discovery home assistant config. This is only needed for very first initialization.
   g_mqtt.publishConfig([this]()
                        {
                             mqttAnnounce("Airthings 2930 Humidity", "%", "hum");
                             mqttAnnounce("Airthings 2930 Temperature", "°C", "temp");
                             mqttAnnounce("Airthings 2930 Air Presure", "hPa", "air");
                             mqttAnnounce("Airthings 2930 Volatile Gases", "ppb", "voc"); });
}

void WaveMini::parseData(BLERemoteCharacteristic *data)
{
   // memcpy(&m_data, data, sizeof(m_data));
   m_data = data->getValue<WaveMiniReadings>();
   m_event = true;
}

void WaveMini::publishStatus() const
{
   char statusTopic[80];
   snprintf(statusTopic, 80, "%s/%s/state", m_topicMQTTHeader, m_serialNo.c_str());

   StaticJsonDocument<120> json;
   json["hum"] = m_data.humidity / 2.0;
   json["temp"] = m_data.temperature / 100.0;
   json["air"] = m_data.pressure / 50.0;
   json["voc"] = m_data.voc * 1.0;

   char outgoingJsonBuffer[100];
   serializeJson(json, outgoingJsonBuffer);
   g_mqtt.publishToMQTT(statusTopic, outgoingJsonBuffer);
}
