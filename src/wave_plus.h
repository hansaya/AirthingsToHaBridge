#ifndef wave_plus_h
#define wave_plus_h

#include "airthings.h"

/**
 * @class WavePlus handles all things required to read the airthings plus sensor.
 */
class WavePlus: public Airthings
{
public:
   /**
    * Constructor
    */
   WavePlus(const BLEAdvertisedDevice device) : Airthings(device, getServiceID()), m_data() {}
   /**
    * Execute anything that belong in setup ()
    */
   void begin();

   /**
    * @brief Get service ID to access device data
    *
    * @return BLEUUID
    */
   static BLEUUID getServiceID() { return BLEUUID("b42e1c08-ade7-11e4-89d3-123b93f75cba"); }

private:
   // Packed data structure transmisted through BLE
   struct __attribute__((packed)) WavePlusReadings
   {
      WavePlusReadings() : version(0), humidity(0), ambientLight(0), unused01(0), radon(0), radon_lt(0), temperature(0), pressure(0), co2(0), voc(0), unused02(0), unused03(0) {}
      uint8_t version; // 1 byte   - @0
      uint8_t humidity; // 1 byte   - @1
      uint8_t ambientLight; // 1 byte   - @0
      uint8_t unused01;     // 1 byte   - @1
      uint16_t radon;       // 2 bytes  - @2 and 3
      uint16_t radon_lt;    // 2 bytes  - @2 and 3
      uint16_t temperature; // 2 bytes  - @2 and 3
      uint16_t pressure;    // 2 bytes  - @2 and 3
      uint16_t co2;         // 2 bytes  - @2 and 3
      uint16_t voc;         // 2 bytes  - @2 and 3
      uint16_t unused02;    // 2 bytes  - @2 and 3
      uint16_t unused03;    // 2 bytes  - @2 and 3
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
   BLEUUID getCharacteristicID() { return BLEUUID("b42e2a68-ade7-11e4-89d3-123b93f75cba"); }

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

   WavePlus::WavePlusReadings m_data;
};

#endif