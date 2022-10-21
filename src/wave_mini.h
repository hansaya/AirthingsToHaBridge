#ifndef wave_mini_h
#define wave_mini_h

#include "airthings.h"

/**
 * @class WavePlus handles all things required to read the airthings mini sensor.
 */
class WaveMini: public Airthings
{
public:
   /**
    * Constructor
    */
   WaveMini(const BLEAdvertisedDevice device) : Airthings(device, getServiceID()), m_data() {}
   /**
    * Execute anything that belong in setup ()
    */
   void begin();

   /**
    * @brief Get service ID to access device data
    *
    * @return BLEUUID
    */
   static BLEUUID getServiceID() { return BLEUUID("b42e3882-ade7-11e4-89d3-123b93f75cba"); }

private:
   // Packed data structure transmisted through BLE
   struct __attribute__((packed)) WaveMiniReadings
   {
      WaveMiniReadings() : unused01(0), temperature(0), pressure(0), humidity(0), voc(0), unused02(0), unused03(0), unused04(0) {}
      uint16_t unused01;    // 2 byte   - @0
      uint16_t temperature; // 2 byte   - @1
      uint16_t pressure;    // 2 byte   - @0
      uint16_t humidity;    // 2 byte   - @1
      uint16_t voc;         // 2 bytes  - @2 and 3
      uint16_t unused02;    // 2 bytes  - @2 and 3
      uint32_t unused03;    // 4 bytes  - @2 and 3
      uint32_t unused04;    // 4 bytes  - @2 and 3
   };

   /**
    * @copydoc Airthings::parseData()
    *
    */
   void parseData(BLERemoteCharacteristic *data);

   /**
    * @copydoc Airthings::getCharacteristicID()
    *
    */
   BLEUUID getCharacteristicID() { return BLEUUID("b42e3b98-ade7-11e4-89d3-123b93f75cba"); }

   /**
    * @copydoc Airthings::getDevInfoServiceID()
    *
    */
   BLEUUID getDevInfoServiceID() { return BLEUUID((uint32_t)0x180A); }

   /**
    * @copydoc Airthings::publishStatus()
    *
    */
   void publishStatus() const;

   /**
    * @copydoc Airthings::announce()
    *
    */
   void announce() const;

   WaveMini::WaveMiniReadings m_data;
};

#endif