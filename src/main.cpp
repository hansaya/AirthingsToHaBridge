/*   
 * A home assistant friendly esp32-based MQTT Garage Door Controller
 * Licensed under the MIT License, Copyright (c) 2017 hansaya
*/

#include <Arduino.h>
#include "ota.h"
#include "config.h"
#include "managed_wifi.h"
#include "button.h"
#include "wave_plus.h"
#include "wave_mini.h"
#include "mqtt.h"
#include "log.h"
#include "led.h"
#include "ble.h"
#include "ListLib.h"

// Setup a button with events.
ButtonEvents sortButtonPress({50, false, true});
ButtonEvents longButtonPress({800, false, true});
ButtonEvents resetButtonPress({6000, false, true});
Button<3> gpio0Button(BUTTON, LOW, &sortButtonPress, &longButtonPress, &resetButtonPress);
List<Airthings* > g_airthings;

// Handle button inputs
void handleButtonInput()
{
  if (sortButtonPress.pressed)
  {
    sortButtonPress.pressed = false;
    g_log.write(Log::Debug, "S");
  }
  if (longButtonPress.pressed)
  {
    longButtonPress.pressed = false;
    g_log.write(Log::Debug, "L");
  }
  if (resetButtonPress.pressed)
  {
    resetButtonPress.pressed = false;
    g_log.write(Log::Debug, "R");
    g_managedWiFi.reset();
  }
}

void setup()
{
  Serial.begin(115200);
  g_log.write(Log::Debug, "Starting Airthing Bridge...");
  g_led.begin();
  g_led.doubleFastBlink();

  // Read the flash for the previous config
  g_config.begin();
  // Function button setup.
  gpio0Button.begin();

  // Connect to access point
  g_managedWiFi.begin();

  // Start the logger. Need wifi
  g_log.begin();

  // Start the services.
  g_ota.begin();
  g_mqtt.begin();
  g_ble.begin();

  // Search for compatible devices using the ble service id
  g_ble.getScanner()->addServiceId(WavePlus::getServiceID());
  g_ble.getScanner()->addServiceId(WaveMini::getServiceID());
  g_ble.search();
  g_log.write(Log::Debug, "MAIN: Found Airthings " + String(g_ble.getScanner()->size()) + " devices.");

  // Initialize Airthings devices.
  if (g_ble.getScanner()->foundDevice())
  {
    for (auto i=0; i<g_ble.getScanner()->size(); i++)
    {
      g_log.write(Log::Debug, "MAIN: device serial: " + String(Airthings::parseSerial(g_ble.getScanner()->getDevices()[i]->getManufacturerData())));
      switch (Airthings::parseModelNumber(g_ble.getScanner()->getDevices()[i]->getManufacturerData()))
      {
        case 2930:
          g_airthings.Add(new WavePlus(*g_ble.getScanner()->getDevices()[i]));
          g_airthings.Last()->begin();
          break;
        case 2920:
          g_airthings.Add(new WaveMini(*g_ble.getScanner()->getDevices()[i]));
          g_airthings.Last()->begin();
          break;
        default:
          g_log.write(Log::Error, "MAIN: Invalid device found!");
      }
    }
  }
}

void loop()
{
  g_ota.loop();
  g_managedWiFi.loop();
  g_log.loop();

  // routine functions.
  if (!g_ota.busy())
  {
    g_mqtt.loop();
    for (auto i=0; i<g_airthings.Count(); i++)
    {
      g_airthings[i]->loop();
    }
    handleButtonInput();
  }
}