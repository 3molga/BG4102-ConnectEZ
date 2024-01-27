// Version 3 of joystick code: midterm rush edition
// Includes: support for all 4 directions
// Prints inputs to serial port only if a change is detected
// Now with object-oriented programming for joystick

// Defining classes
#include "joystick.h"
#include "joystickAxis.h"
#include "ezButton.h"
#include "telebot.h"
#include <iostream>
#include <vector>
#include <string>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
using namespace std;

// Input objects/pins
joystick joystick(32, 35);
ezButton buttonConfirm(27);  
ezButton buttonReturn(26);
ezButton buttonTele(25);
#define touchPinInput 13

// Output pins
#define touchPinLED 15  // External LED is at Pin 15
#define ONBOARD_LED 2   // ON_BOARD LED is at Pin 2

// Wifi and Telegram setup
const char *ssid = "tmt";                                          // Wi-Fi ID
const char *password = "yeah8913";                                 // Wi-Fi password
#define BOTtoken "6606670486:AAGP24iLfP047ysZ7yaAUHyv3cOf-iLKMqE"  // Telegram bot API token
#define CHAT_ID "-4016407865"                                      // "Chat with esp32" telegram group ID
WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);
telebot botController(bot, CHAT_ID);

// Joystick/array controlling variables
std::vector<int> userState{ 0, 0 };       // Stores x and y coords of where the user currently is on the UI
std::vector<int> userInput{ 0, 0 };       // Stores inputs from user
std::vector<int> matrixSize = { 3, 10 };  // Arbitrary assumption for maximum size of array (+1)
bool joystickUpdateTrigger;
const String newLineBar = "-----------------------------------------------------------------------------------------------";

// Capacitance touch variables
int touchValue;                 // Variable for storing the touch pin value
const int touchThreshold = 30;  // Capacitance touch value lower than this will indicate the person is touching the device
bool touchBool;

// -----------------------------------------SETUP-------------------------------------------
void setup() {
  Serial.begin(115200);
  joystick.joystickSetup();
  buttonConfirm.setDebounceTime(50);
  buttonReturn.setDebounceTime(50);
  buttonTele.setDebounceTime(50);

  // Connect to Wi-Fi
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  WiFi.begin(ssid, password);
  client.setCACert(TELEGRAM_CERTIFICATE_ROOT);
  while (millis() < 5000) {
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("WiFi connected");
      botController.queueMessage(std::string("Hello everyone, I am connected!\nUse /start for commands to control me!"));
      break;
    } else {
      delay(500);
      Serial.print(".");
    }
  }

  // Set up button, Wi-Fi confirmaton LED and afk LED
  pinMode(ONBOARD_LED, OUTPUT);
  pinMode(touchPinLED, OUTPUT);
  digitalWrite(touchPinLED, 0);

  // Set up parallel processing task to handle Telegram reading
  xTaskCreatePinnedToCore(
    TeleHandler,            /* Task function. */
    "Telegram Bot Handler", /* name of task. */
    100000,                 /* Stack size of task */
    NULL,                   /* parameter of the task */
    1,                      /* priority of the task */
    NULL,                   /* Task handle to keep track of created task */
    1);                     /* pin task to core 0 */
}

// -----------------------------------------LOOPS-------------------------------------------
/* Main loop to handle all active user inputs
ie. joystick and button updates, queueing messages to send through Telegram */
void loop() {
  buttonConfirm.loop();
  buttonReturn.loop();
  buttonTele.loop();
  joystickUpdateTrigger = joystick.joystickStateTrigger();

  // Check if the touchValue is below the threshold, check if the person is touching the device
  touchValue = touchRead(touchPinInput);
  if (touchValue < touchThreshold) {
    touchBool = true;
    digitalWrite(touchPinLED, HIGH);
  } else {
    touchBool = false;
    digitalWrite(touchPinLED, LOW);
  }

  // Get and print joystick state and returned message
  if (joystickUpdateTrigger) {
    userInput = joystick.joystickReturnState();
    updateInputs();

    // Print everything to serial (for debugging)
    Serial.println(joystick.joystickMessageCheck());
    Serial.println(intToString(userInput).c_str());
    Serial.println(intToString(userState).c_str());
    Serial.println(newLineBar);
  }

  // Handle user input from buttons
  if (buttonConfirm.isPressed()) {
    Serial.println("Confirm button pressed");
    if (WiFi.status() == WL_CONNECTED) {
      botController.queueMessage(std::string("User is sending: ") + intToString(userState) + ".");
    }
  }

  if (buttonReturn.isPressed()) {
    Serial.println("Return button pressed");
  }

  if (buttonTele.isPressed()) {
    Serial.println("Tele button pressed");
    if (WiFi.status() == WL_CONNECTED) {
      botController.queueMessage(std::string("Help, I am in danger!"));
    }
  }
}

/* 2nd Loop
Handles all Telegram-related actions
ie. polling the API for chat updates, automatically responding to them, sending messages queued by the user
Separate from the main loop for performance purposes (this stuff takes a while to process) */
void TeleHandler(void *pvParameters) {
  while (1) {
    if (WiFi.status() != WL_CONNECTED) {
      // Wi-Fi is not connected LED
      digitalWrite(ONBOARD_LED, LOW);
    } else {
      // Wi-Fi is connected LED
      digitalWrite(ONBOARD_LED, HIGH);
      botController.handleActiveUpdates();
      botController.handlePassiveUpdates();
    }
  }
}

//------------------------------------FUNCTIONS------------------------------------
// Function to update user state (temporary, maybe move into class later)
// Takes user inputs and upper bound of array size
void updateInputs() {
  for (int i = 0; i < userState.size(); i++) {
    userState[i] = userState[i] + userInput[i];
    if (userState[i] < 0) {
      userState[i] = 0;
    } else if (userState[i] > matrixSize[i]) {
      userState[i] = matrixSize[i];
    }
  }
}

// Function to convert int vector into a std::string to print
std::string intToString(std::vector<int> intVector) {
  std::string tempOutput = to_string(intVector[0]);
  for (int i = 1; i < intVector.size(); i++) {
    tempOutput = tempOutput + ", " + to_string(intVector[i]);
  }
  return tempOutput;
}