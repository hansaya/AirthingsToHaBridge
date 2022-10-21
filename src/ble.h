#ifndef ble_h
#define ble_h

#include <Arduino.h>
// #include <BLEDevice.h>
#include <NimBLEDevice.h>
#include "log.h"
#include "ListLib.h"

const static BLEUUID k_serviceUUID("b42e1c08-ade7-11e4-89d3-123b93f75cba");
#define MAX_BLE_CLIENTS 50
#define BLE_SCAN_PERIOD 5

// The bluetooth stack takes a callback when scannign for devices.  The first Airthings device it finds it will record in pServerAddress.
class FoundDeviceCallback : public BLEAdvertisedDeviceCallbacks
{
public:
   /**
    * Return the BLE client.
    * @param[out] BLEClient.
    */
   bool foundDevice()
   {
      if (m_foundDevice)
      {
         m_foundDevice = false;
         return true;
      }
      else
         return false;
   }
   /**
    * Return ble devices that was found.
    * @param[out] BLEAdvertisedDevice.
    */
   BLEAdvertisedDevice **getDevices()
   {
      return m_devices;
   }
   /**
    * Return the number of ble devices found
    * @param[out] size_t.
    */
   size_t size()
   {
      return m_count;
   }
   /**
    * Add a service id to search.
    * @param[out] BLEClient.
    */
   void addServiceId(BLEUUID id)
   {
      m_serviceIds.Add(id);
   }
protected:
   /**
    * Call back function when device was found.
    */
   void onResult(NimBLEAdvertisedDevice *advertisedDevice);

private:
   bool m_foundDevice;
   BLEAdvertisedDevice *m_devices[MAX_BLE_CLIENTS];
   size_t m_count;
   List<BLEUUID> m_serviceIds;
};

/**
 * Class handle ble related calls.
 */
class BLE
{
public:
   /**
    * Execute anything that belong in setup ().
    */
   void begin();

   /**
    * Search for BLE devices
    */
   BLEScanResults search();

   /**
    * Return the BLE client.
    * @param[out] BLEClient.
    */
   BLEClient* getClient() { return m_bleClient; }

   /**
    * Return the BLE scanner.
    * @param[out] FoundDeviceCallback.
    */
   FoundDeviceCallback *getScanner() { return m_btCallback; }
private:
   BLEClient *m_bleClient;
   FoundDeviceCallback *m_btCallback;
   BLEScan *m_pBLEScan;

};

extern BLE g_ble;

#endif