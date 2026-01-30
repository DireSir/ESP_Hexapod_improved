#ifndef CONFIGURATION_H
#define CONFIGURATION_H

struct WifiCred {
  const char* ssid;
  const char* password;
};

// This is a list of all default WiFi networks that persist between runtimes
// You can add your WiFi network here, and then re-compile and re-upload the project
// The hexapod tries to connect to these network in order, so the higher a network is, the higher its priority
static const WifiCred WIFI_NETWORKS[] = {
  { "hexapod-ap", "hexapod123" }, // These default networks are intentionally left here
  { "raspberrypi", "elevprojekt" },
};

static const int WIFI_NETWORK_COUNT = sizeof(WIFI_NETWORKS) / sizeof(WIFI_NETWORKS[0]);

// Consult your board's pinout for the user-controlled LED.
// Setting LED_FEEDBACK to NULL will disable LED feedback.
const int LED_FEEDBACK = 21;

#endif // CONFIGURATION_H