#include "ble.h"

void FoundDeviceCallback::onResult(NimBLEAdvertisedDevice *advertisedDevice)
{
   if (advertisedDevice->haveServiceUUID())
   {
      for (size_t i = 0; i < m_serviceIds.Count(); i++)
      {
         if (advertisedDevice->getServiceUUID().equals(m_serviceIds[i]))
         {
            if (m_count < MAX_BLE_CLIENTS)
            {
               for (size_t i = 0; i < m_count; i++)
               {
                  if (m_devices[i]->getAddress() == advertisedDevice->getAddress())
                     return;
               }

               m_devices[m_count] = new BLEAdvertisedDevice(*advertisedDevice);
               m_count++;
               m_foundDevice = true;
            }
            else
            {
               g_log.write(Log::Warn, "BLE: Device search limit reached!");
               advertisedDevice->getScan()->stop();
            }
         }
      }
   }
}

void BLE::begin()
{
   BLEDevice::init("AirthingsScan");
   BLEDevice::setMTU(517);
   m_bleClient = BLEDevice::createClient();
   m_pBLEScan = BLEDevice::getScan();
   m_btCallback = new FoundDeviceCallback();
   m_pBLEScan->setAdvertisedDeviceCallbacks(m_btCallback);
   m_pBLEScan->setActiveScan(true);
   m_pBLEScan->setInterval(100);
   m_pBLEScan->setWindow(99);
}

BLEScanResults BLE::search()
{
   BLEScanResults foundDevices = m_pBLEScan->start(BLE_SCAN_PERIOD, false);
   m_pBLEScan->clearResults();
   return foundDevices;
}

BLE g_ble;
