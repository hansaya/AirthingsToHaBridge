#ifndef airthings_h
#define airthings_h

#include <Arduino.h>
#include <NimBLEDevice.h>

#define READ_WAIT_SECONDS 60 * 5

const static BLEUUID k_devInfoSerialUUID((uint32_t)0x2A25);
const static BLEUUID k_devInfoModelUUID((uint32_t)0x2A24);

/**
 * @class Airthings handles all things required to read the airthings sensor.
 */
class Airthings
{
public:
    /**
     * Constructor
     */
    Airthings(const BLEAdvertisedDevice device, BLEUUID serviceId);
    /**
     * @brief Execute anything that belong in setup ()
     */
    void begin();
    /**
     * @brief logic that needs to run within loop()
     */
    void loop();
    /**
     * @brief Parse the serial number using the manufacture data
     *
     * @param manuData Raw manufacture data
     * @return uint32_t
     */
    static uint32_t parseSerial(std::string manuData);
    /**
     * @brief Parse the model number using the manufacture data
     *
     * @param manuData Raw manufacture data
     * @return uint32_t
     */
    static long parseModelNumber(std::string manuData);
protected:
    /**
     * @brief Publish json to MQTT with all the data
     *
     */
    virtual void publishStatus() const = 0;
    /**
     * @brief Get characteristic id to get the sensor data
     *
     * @return BLEUUID
     */
    virtual BLEUUID getCharacteristicID() = 0;
    /**
     * @brief Get Service id to access the dev info
     *
     * @return BLEUUID
     */
    virtual BLEUUID getDevInfoServiceID() = 0;
    /**
     * @brief Announce MQTT broker that you are alive
     *
     */
    virtual void announce() const = 0;
    /**
     * @brief Parse the raw data from ble
     *
     * @param data pointer to raw data
     */
    virtual void parseData(BLERemoteCharacteristic *data) = 0;

    /**
     * @brief Announce the cover config to home assistant.
     *
     * @param name Name of the sensor
     * @param unit Data unit.
     * @param id Unique id to the sensor reading
     */
    void mqttAnnounce(const String &name, const String &unit, const String &id) const;

    /**
     * @brief Convert radon value
     * 
     * @param radon_raw raw radon data.
     * @return float 
     */
    float conv2Radon(uint16_t radon_raw) const
    {
        if (0 <= radon_raw && radon_raw <= 16383)
        {
            return radon_raw;
        }
        return 0.0;
    };

    bool m_event;
    char m_topicMQTTHeader[50];
    String m_serialNo;
    String m_modelNo;

private:
    /**
     * @brief Connect to the device
     * 
     * @return true 
     * @return false 
     */
    bool connect();
    /**
     * @brief Disconnect from the device.
     * 
     */
    void disconnect();

    /**
     * @brief Get the sensor reading.
     */
    bool getSensorReading();

    BLEAdvertisedDevice m_device;
    bool m_announced;
    BLEUUID m_serviceId;
};
#endif