#include "config.h"         // For the default WiFi networks and passwords
#include <Arduino.h>        // For Serial, String, etc.
#include <WiFi.h>           // For Serial, String, etc.
#include "remote_control.h" // For TCP and UDP ports


// --- Public Functions ---
bool connectToWifi(const char* ssid, const char* password) {
  Serial.println("Connecting to: ");
  Serial.println(ssid);

  WiFi.disconnect(true);
  delay(100);

  WiFi.begin(ssid, password);

  int wifi_retries = 0;
  bool led_state = false; // For blinking during connection attempt

  while (WiFi.status() != WL_CONNECTED && wifi_retries < 30) { // Retry for ~15 seconds
    digitalWrite(LED_FEEDBACK, led_state);
    led_state = !led_state;
    delay(250);
    Serial.print(".");
    wifi_retries++;
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi Connected!");
    return true;
  }
  else {
    Serial.println("\nWiFi Connection Failed!");
    return false;
  }
}

bool wifiConnectKnownNetworks() {
  Serial.println("Attempting to connect to WiFi.");
  WiFi.disconnect(true);
  delay(100);
  for (int i = 0; i < WIFI_NETWORK_COUNT && WiFi.status() != WL_CONNECTED; i++) {
    if (connectToWifi(WIFI_NETWORKS[i].ssid, WIFI_NETWORKS[i].password)) {
      Serial.print("IP Address: ");
      Serial.println(WiFi.localIP());
      digitalWrite(LED_FEEDBACK, LOW); // LED ON for successful connection - it is inverted
      return true;
    }
  }
  Serial.println("All known WiFi networks are unreachable");
  digitalWrite(LED_FEEDBACK, HIGH);  // LED OFF for failed connection - it is inverted
    // Proceeding, but remote mode will be non-functional without WiFi.
  WiFi.disconnect(true); // Just to make sure.
  return false;
}

void printWifiHelp() {
  Serial.println("\nAvailable Commands (WiFi Configuration Menu):");
  Serial.println("  I                - Current WiFi status (Connection status, IP-address, etc.)");
  Serial.println("  C <SSID> <PSSWD> - Attempt to connect to a new simple WiFi network");
  Serial.println("  K                - Attempt to connect to all default WiFi networks");
  Serial.println("  D                - Disconnect from WiFi");
  Serial.println("  H / ?            - Display this Help");
  Serial.println("  X                - Exit the WiFi Configuration Menu");
}

void setupWifiConfigurationMenu() {
  Serial.println("\n=== WiFi Configuration Menu ===");
  printWifiHelp();
  Serial.println("===============================");
}

bool wifiConfigurationUpdate() {
  // I mostly copied the servoTestModeUpdate function
  if (Serial.available() > 0) {
    String commandStr = Serial.readStringUntil('\n');
    commandStr.trim();

    if (commandStr.length() == 0) return true;

    Serial.print("CMD_WiFi> "); Serial.println(commandStr);

    char cmdBuffer[64];
    strncpy(cmdBuffer, commandStr.c_str(), sizeof(cmdBuffer) - 1);
    cmdBuffer[sizeof(cmdBuffer) - 1] = '\0';

    int leg_idx_param = -1, joint_idx_param = -1;
    float angle_val_param = 0.0f;

    if (commandStr.equalsIgnoreCase("X")) {
      Serial.println("\nExiting WiFi Configuration Menu ...");
      return false;
    } else if (commandStr.equalsIgnoreCase("I")) {
      if (WiFi.status() == WL_CONNECTED){
        Serial.println("Status: Connected");
        Serial.print("IP Address: ");
        Serial.println(WiFi.localIP());
        Serial.print("MAC Address: ");
        Serial.println(WiFi.macAddress());
        Serial.print("TCP port: ");
        Serial.println(TCP_LISTEN_PORT);
        Serial.print("UDP port: ");
        Serial.println(UDP_LISTEN_PORT);
      }
      else {
        Serial.println("Status: Disconnected");
      }
    } else if (commandStr.equalsIgnoreCase("H") || commandStr.equals("?")) {
      printWifiHelp();
    }
    else if (commandStr.length() >= 2 && (commandStr.substring(0, 1).equalsIgnoreCase("C")) ) {
      char ssid[33];
      char password[65];

      if (sscanf(cmdBuffer + 2, "%32s %64s", ssid, password) == 2) {
        connectToWifi(ssid, password);
      } else {
        Serial.println("[Error] Invalid format. Usage: C <SSID> <PSSWD>");
      }
    } else if (commandStr.equalsIgnoreCase("K")) {
      wifiConnectKnownNetworks();
    } else if (commandStr.equalsIgnoreCase("D")) {
      WiFi.disconnect(true);
      delay(100);
      Serial.println("Disconnected WiFi");
    } else {
      Serial.print("Unknown command: '"); Serial.print(commandStr);
      Serial.println("'. Type 'H' or '?' for help");
    }
    while(Serial.available())
      Serial.read();
      // Weren't we taught by the same guy? Did you not pay any attention? This looks sooo much better...
  }
  return true;
}