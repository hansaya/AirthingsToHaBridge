# Airthings Plus and Mini to Home Assistant bridge
This is a simple project that search for Airthings Plus and Airthings Mini devices and submits the data to MQTT. If homeassistant is configured with auto discovery, it will pick up these devices automatically.

## Supported devices
* https://www.airthings.com/wave-plus
* https://www.airthings.com/wave-mini

## How to get started
### Compile
1) Download and install vscode with platformio. https://platformio.org/
2) Modify the platformio.ini file to either use serial port or use over the air updates.
4) Press upload.

### Configure the device
Connect to AP "AirthingsBridge" and fill in the blanks for your wifi and MQTT server.

### Check for logs
Telnet to `<hostname>::23` or use the serial adapter