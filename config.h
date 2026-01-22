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
  { "hexapod-ap", "hexapod123" },
  { "raspberrypi", "elevprojekt" },
};

static const int WIFI_NETWORK_COUNT = sizeof(WIFI_NETWORKS) / sizeof(WIFI_NETWORKS[0]);

// If LED_BUILTIN is defined, it will be used for feedback.
// Otherwise, you may have to consult your board's pinout.
// Setting LED_FEEDBACK to NULL will disable LED feedback.
#ifdef LED_BUILTIN
  const int LED_FEEDBACK = LED_BUILTIN;
#else
  const int LED_FEEDBACK = 2;
#endif

#endif // CONFIGURATION_H