#ifndef WIFI_CONFIGURATION_H
#define WIFI_CONFIGURATION_H

#include <Arduino.h>

/**
 * @brief Sets up the WiFi menu.
 * Prints the help message and the current WiFi status.
 */
inline void setupWifiConfigurationMenu();
/**
 * @brief Main loop for the WiFi configuration menu.
 * @return True to continue running this mode, false to exit to the main menu.
 */
inline bool wifiConfigurationUpdate();

bool wifiConnectKnownNetworks();
/**
 * @brief Attempts to connect to a WiFI network using a specified SSID and password.
 * @return Returns a boolean depending on if it successfull or not.
 */
inline bool connectToWifi(const char* ssid, const char* password);

/**
 * @brief Prints available serial commands for the wifi menu.
 */
inline void printWifiHelp();

#endif // WIFI_CONGURATION_H