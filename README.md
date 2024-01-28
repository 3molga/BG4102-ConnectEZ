
# ConnectEZ: Group Project for BG4102 Medical Device Design

This repository stores code for an alternative and augmented communication (AAC) medical device that was designed during the course of BG4102 Medical Device Design in Nanyang Technological University, AY2023-2024. The codebase is a mix of C (primarily from the LVGL library and associated code) and C++, based on the Arduino framework, and compiled via the PlatformIO extension on Visual Studio Code. 

Refer to [final_compile](https://github.com/3molga/BG4102-ConnectEZ/tree/main/final_compile) for the final version of the code (as submitted for evaluation), while the [past_versions](https://github.com/3molga/BG4102-ConnectEZ/tree/main/past_versions) folder stores code from development of the device.

## Code
Custom code written for this project include:
- UI: utilizes the LVGL library to create and display a custom UI for the device.
- joystick and joystickAxis: utilizes the ezButton library to detect and process inputs from the joystick module.
- googlefirebase: handles detection, processing and logging of inputs from the accelerometer module on Google Firebase.
- telebot: utilizes the UniversalTelegramBot library to handle sending and receiving Telegram messages.
- main: main script that handles overall functioning of the device.

External dependencies include:

- LVGL: microcontroller graphics library that handles creation, rendering and processing of graphical elements for a function GUI. 
- WiFi: default ESP32 library that handles creation of WiFi client for wireless connectivity.
- WiFiClientSecure: handles creation of secure WiFi connections. 
- UniversalTelegramBot: handles connection and interfacing with Telegram's bot API.
- ArduinoJson: handles formatting of messages for UniversalTelegramBot.
- Firebase Arduino Client Library: handles connection and interfacing with the Google Firebase API.
- TFT_eSPI: handles SPI connection from the ESP32 to the screen for the display.
- XPT2046_calibrated: handles processing of touch inputs and calibration of the touchscreen.
- MPU6050_accelerometer: handles detection of inputs from the accelerometer. 
- Adafruit MPU6050: handles detection of inputs from the accelerometer.
- MAX3010x Sensor Library: handles detection and processing of inputs from the pulse oximetry module. 
- ezButton: handles detection and processing of inputs from buttons.

## Contributors
Contributors to the codebase include:
- [Myself](https://github.com/3molga): UI, joystick and telebot libraries, and overall compilation 
- [Zane](https://github.com/yongzane00): Firebase library and overall compilation 
- [Cher](https://github.com/szetosy): Accelerometer and pulse oximetry module code
- Celine: Accelerometer and pulse oximetry module code
