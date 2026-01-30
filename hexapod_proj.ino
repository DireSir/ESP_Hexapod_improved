// hexapod_proj.ino

// #############################################################################
// ### LIBRARIES ###
#include "config.h"          // For WiFi credentials
#include <WiFi.h>
#include <WiFiUdp.h>
#include "config.h"
#include "servo_angles.h"       // For setupPwm, servo control
#include "robot_spec.h"         // For robot constants and global state
#include "remote_control.h"     // Handles all remote operations
#include "servo_test_mode.h"    // For the servo test mode
#include "walkcycle.h"          // For basic walk cycle setup if needed by remote_control
#include "wifi_configuration.h" // A hastily coded WiFi configuration menu as to eliviate the need to re-flash the program every time the netwroks change

// #############################################################################
// ### DEFINES AND GLOBAL VARIABLES ###

// --- Network ---
WiFiUDP udp; // UDP instance used by remote_control.cpp as well

// --- Robot Operating Modes ---
enum RobotOperatingMode {
  MAIN_MENU,          // Serial command menu
  REMOTE_CONTROL,     // Default: UDP JSON control, mobile app input
  SERVO_TEST,         // Direct servo control via serial
  WIFI_CONFIGURATION  // WiFI connection managment through a CLI
};
RobotOperatingMode currentOperatingMode = MAIN_MENU;

unsigned long lastLoopTimeMicros = 0;

// #############################################################################
// ### SETUP ###
void setup() {
  Serial.begin(115200);
  while (!Serial && millis() < 2000); // Wait for serial, but timeout
  Serial.println("\n\n--- Hexapod Control System Booting ---");

  pinMode(LED_FEEDBACK, OUTPUT);
  digitalWrite(LED_FEEDBACK, HIGH); // It's inverted

  // --- Hardware Initialization ---
  setupPwm(); // Initialize PCA9685 servo drivers
  Serial.println("PWM Drivers Initialized.");

  // --- WiFi Connection ---
  if (wifiConnectKnownNetworks()) {
    setupRemoteControl();
    currentOperatingMode = REMOTE_CONTROL;
  } else {
    currentOperatingMode = MAIN_MENU;
  }


  // --- Initialize Control Modes ---
  // setupWalkcycle(); // Basic walkcycle params - called within setupRemoteControl if needed
  // setupRemoteControl(); // Initialize remote control systems (UDP, JSON parsing states)
  // setupServoTestMode(); // Initialize servo test mode (prints help, etc.)

  Serial.println("All systems initialized.");

  // --- Default Operating Mode ---
  if (currentOperatingMode == REMOTE_CONTROL) {
    Serial.println("Entering Remote Control Mode by default.");
  } else if (currentOperatingMode == MAIN_MENU) {
    printMainMenuHelp();
  }

  lastLoopTimeMicros = micros();
}

// #############################################################################
// ### MAIN LOOP ###
void loop() {
  unsigned long startMicros = micros();

  switch (currentOperatingMode) {
    case MAIN_MENU:
      if (processMainMenuCommands()) {
        // Command processed, potentially changed mode
      }
      break;

    case REMOTE_CONTROL:
      if (!remoteControlUpdate()) { // remoteControlUpdate returns false if it wants to exit
        Serial.println("Exiting Remote Control Mode, returning to Main Menu.");
        currentOperatingMode = MAIN_MENU;
        printMainMenuHelp();
      }
      break;

    case SERVO_TEST:
      if (!servoTestModeUpdate()) { // servoTestModeUpdate returns false if 'X' is entered
        Serial.println("Exiting Servo Test Mode, returning to Main Menu.");
        currentOperatingMode = MAIN_MENU;
        printMainMenuHelp();
      }
      break;

    case WIFI_CONFIGURATION:
      if (!wifiConfigurationUpdate()) {
        Serial.println("Exiting WiFi configuration, returning to Main Menu.");
        currentOperatingMode = MAIN_MENU;
        printMainMenuHelp();
      }
      break;

    default:
      Serial.println("[ERROR] Unknown operating mode! Reverting to Main Menu.");
      currentOperatingMode = MAIN_MENU;
      printMainMenuHelp();
      break;
  }
  lastLoopTimeMicros = startMicros;
}

// #############################################################################
// ### MAIN MENU SERIAL COMMANDS ###
void printMainMenuHelp() {
  Serial.println("\n========== Main Menu ==========");
  Serial.println("Enter command:");
  Serial.println("  R - Enter Remote Control Mode");
  Serial.println("  T - Enter Servo Test Mode");
  Serial.println("  W - Configure WiFi");
  Serial.println("  H / ? - Display this help");
  Serial.println("===============================");
}

bool processMainMenuCommands() {
  if (Serial.available() > 0) {
    char command = toupper(Serial.read());
    while (Serial.available() > 0 && (Serial.peek() == '\n' || Serial.peek() == '\r')) { Serial.read(); } // Clear buffer

    Serial.print("MainMenu CMD> "); Serial.println(command);

    switch (command) {
      case 'R':
        Serial.println("Transitioning to Remote Control Mode...");
        currentOperatingMode = REMOTE_CONTROL;
        setupRemoteControl();
        break;
      case 'T':
        Serial.println("Transitioning to Servo Test Mode...");
        currentOperatingMode = SERVO_TEST;
        setupServoTestMode();
        break;
      case 'W':
        Serial.println("Opening the WiFi Configuration Menu ...");
        currentOperatingMode = WIFI_CONFIGURATION;
        setupWifiConfigurationMenu();
        break;
      case 'H':
      case '?':
        printMainMenuHelp();
        break;
      default:
        Serial.println("Unknown command. Type 'H' or '?' for help.");
        return false;
    }
    return true;
  }
  return false;
}