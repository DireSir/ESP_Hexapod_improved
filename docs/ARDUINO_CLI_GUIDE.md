# A Linux User's guide (Arduino CLI Guide)
###### By [YourDire](https://yourdire.net)

This short guide will help you with installing the Arduino Command Line Interface on a machine running Linux or MacOS, and should be ignored if you use windows or value your time (use the IDE). It is assumed that you already cloned the project repository and navigated to it - for help with that read the [quickstart guide](./QUICK_START_GUIDE.md).

## Table of Contents
1.  [Installing Arduino CLI](#1-installing-arduino-cli)
2.  [Downloading Project Dependencies](#2-downloading-project-dependencies)
3.  [Compiling and Uploading Your Code](#3-compiling-and-uploading-your-code)

---

## 1. Installing Arduino CLI

arduino-cli is not officially distributed through any package managers but Homebrew, but it is available as a package on pacman nevertheless, and I can't be sure about other popular package managers because I use Arch btw, if you couldn't tell. Refer to the official [arduino site](https://docs.arduino.cc/arduino-cli/installation/) for instructions. Installing arduino-cli on Arch Linux is as easy as runnning `sudo pacman -S arduino-cli` though.

## 2. Downloading Project Dependencies

This project is not trivial to start working on through the CLI, but fret not! Most of the work was already done for you. You must only initialize the CLI config by running
```bash
arduino-cli config init
```
And update the arduino core index and install the entirety of esp32 core. Unfortunately I couldn't find a way to install onlt the XIAO_ESP32S3 module that we'll need. (You need to install different cores for different boards, and the esp32 core weighs several gigabytes.)
```bash
arduino-cli core update-index
arduino-cli core install esp32:esp32
```
Now, you only need to install the libraries required for this project by running
``` bash
arduino-cli lib install "Adafruit PWM Servo Driver Library"
arduino-cli lib install "ArduinoJson"
```
And you're good to go!

## 3. Compiling and Uploading Your Code

Assuming that you're in the same directory as the hexapod_proj.ino file (Feel free to rename it, but its name and the folder name must match), run
```bash
arduino-cli compile --fqbn esp32:esp32:XIAO_ESP32S3
```
To compile the project for the XIAO_ESP32S3 board, If you're using a different board, use
```bash
arduino-cli board listall
```
To get a list of all the boards that your system currently supports. If your board is not there, google's your best friend - go and download a new core.

If the project compiles successfully, use
```bash
arduino-cli upload -p /dev/ttyACM0 --fqbn esp32:esp32:XIAO_ESP32S3
```
After connecting your computer to the board to upload compiled code to the board.

Congratulations, you did it! Read the other guides for details on what to do next.