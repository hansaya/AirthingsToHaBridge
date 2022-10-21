#ifndef user_config_h
#define user_config_h

// #define DEBUG_SERIAL
#define DEBUG_TELNET
#define LOG_LEVEL Log::Debug

// -- Project -------------------------------------------
#define CLIENT "AirthingsBridge" // Client ID for the ESP

// MQTT Parameters
#define MQTT_BROKER "example.com"
#define MQTT_PORT "1883" // [MQTT] MQTT Port on the Server
#define MQTT_USERNAME ""
#define MQTT_PASSWORD ""
#define MQTT_HOME_ASSISTANT_DISCOVERY_PREFIX "homeassistant"

// Pins for peripherals
#define STATUS_LED 2
#define BUTTON 0

#endif