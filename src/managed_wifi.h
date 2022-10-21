#ifndef managed_wifi_h
#define managed_wifi_h

#include <Arduino.h>
#include "user_config.h"
#include "WiFiManager.h"

#define PARAM_SIZE 10

/**
 * Class ManagedWiFi handles all the WIFI magic. Capable of automatically handling the configuration portal at inital setup and can be used with a external reset button to reset.
 */
class ManagedWiFi
{
public:
    /**
     * @brief Constructor.
     */
    ManagedWiFi();
    /**
     * @brief Execute anything that belong in setup ().
     */
    void begin();
    /**
     * @brief Reconnect logic if connection drops.
     */
    void loop();
    /**
     * @brief Get the hostname.
     * @return String containing hostname.
     */
    String getHostName() const;
    /**
     * @brief Get the mac address as a string.
     * @return String containing mac address.
     */
    String getMacStr() const;
    /**
     * @brief Get the mac address in bytes.
     * @return Mac address in bytes.
     */
    byte *getMac();
    /**
     * @brief Return true if connected to a device.
     * @return Connected indicator.
     */
    bool connected() const;
    /**
     * @brief Force the WIFI config portal.
     */
    void reset();

private:
    /**
     * Manage wifi portal.
     * @param[in] reset_config Reset the WIFI config indicator. False by default.
     */
    void manageWiFi(const bool reset_config = false);
    /**
     * Config mode callback function for WIFI manager library.
     * @param[in] myWiFiManager WiFiManager instance.
     */
    static void configModeCallback(WiFiManager *myWiFiManager);
    /**
     * Save Config callback.
     */
    static void saveConfigCallback();

    String m_hostName;
    String m_macString;
    byte m_mac[7];
    static bool m_gotTheConfig;
};

extern ManagedWiFi g_managedWiFi;

#endif