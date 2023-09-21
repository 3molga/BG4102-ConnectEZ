// Version 3 of joystick code: midterm rush edition
// Includes: support for all 4 directions
// Prints inputs to serial port only if a change is detected
// Now with object-oriented programming for joystick

// Defining classes
#include "joystick.h"
#include "joystickAxis.h"
#include "ezButton.h"
#include <iostream>
#include <vector>
#include <string>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
using namespace std;

// Input objects
joystick joystick(15, 4);
ezButton buttonConfirm(18);  // Fill this in later ya dumb cunt
ezButton buttonReturn(19);
#define BUTTON_PIN 21
const int touchPinInput = 13;
const int touchPinLED = 26;     // External LED is at Pin 26 
#define ONBOARD_LED 2  // ON_BOARD LED value is 2

/*
// Wifi setup
const char* ssid = "tmt";                                          // Wi-Fi ID
const char* password = "yeah8913";                                 // Wi-Fi password
#define BOTtoken "6606670486:AAGP24iLfP047ysZ7yaAUHyv3cOf-iLKMqE"  // Telegram bot API token
#define CHAT_ID "-4016407865"                                      // "Chat with esp32" telegram group ID
WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);
int botRequestDelay = 1000;
unsigned long lastTimeBotRan;
*/
// Defining variables
std::vector<int> userState {0, 0}; // Stores x and y coords of where the user currently is on the UI
std::vector<int> userInput {0, 0}; // Stores inputs from user
int matrixSize = 3; // Arbitrary assumption for maximum size of array (+1)
bool userUpdateTrigger;
const String newLineBar = "-----------------------------------------------------------------------------------------------";

// Wi-Fi 
int lastState = LOW;   // the previous state from the input pin
int currentState;      // the current reading from the input pin

// Capacitance touch to detect if person is afk or not
int touchValue;            // Variable for storing the touch pin value
const int touchThreshold = 30;  // Capacitance touch value lower than this will indicate the person is touching the device

// -----------------------------------------SETUP-------------------------------------------
void setup() {
  Serial.begin(9600);
  joystick.joystickSetup();
  buttonConfirm.setDebounceTime(500);
  buttonReturn.setDebounceTime(500); /*
  // Connect to Wi-Fi
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  WiFi.begin(ssid, password);
  client.setCACert(TELEGRAM_CERTIFICATE_ROOT);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");  
  bot.sendMessage(CHAT_ID, "Hello everyone, I am connected!");
*/
  // Setup button, Wi-Fi confirmaton LED and afk LED
  pinMode(ONBOARD_LED, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(touchPinLED, OUTPUT);
  digitalWrite(touchPinLED, 0);
}

// -----------------------------------------LOOP-------------------------------------------
void loop() {
  buttonConfirm.loop();
  buttonReturn.loop();
  userUpdateTrigger = joystick.joystickStateTrigger();

  // Check if the touchValue is below the threshold, check if the person is touching the device
  touchValue = touchRead(touchPinInput);
  if (touchValue < touchThreshold) {
    digitalWrite(touchPinLED, HIGH);
  } else {
    digitalWrite(touchPinLED, LOW);
  }
 /*
  // If there is no update from joystick or either button, return early and don't execute the following code
  if (!(joystick.joystickStateTrigger() || buttonConfirm.isPressed() || buttonReturn.isPressed())) {
    return;
  }

  // User input from button has higher precendence than joystick
  // If for whatever reason both confirm and return are pressed at the same time, return early
  if (buttonConfirm.isPressed() && buttonReturn.isPressed()) {
    Serial.println("Both confirm and return buttons pressed");
    return;
  }

  // Else have individual actions for either confirm or return
  if (buttonConfirm.isPressed()) {
    Serial.println("Confirm button pressed");
    return;
  }

  if (buttonReturn.isPressed()) {
    Serial.println("Return button pressed");
    return;
  }
*/
  // Lowest priority state: only joystick input
  // Get and print joystick state and returned message
  if (userUpdateTrigger){
  userInput = joystick.joystickReturnState();
  updateInputs(matrixSize);

  // Print everything
  Serial.println(joystick.joystickMessageCheck());
  Serial.println(intToString(userInput));
  Serial.println(intToString(userState));
  Serial.println(newLineBar);
  }
/*
  // Check Wifi connection
  if (WiFi.status() != WL_CONNECTED) {
    // Wi-Fi is not connected LED
    digitalWrite(ONBOARD_LED, LOW);
    return;
  } else {
    // Wi-Fi is connected LED
    digitalWrite(ONBOARD_LED, HIGH);
  }

  // Handling query from telegram user
  if (millis() > lastTimeBotRan + botRequestDelay) {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while (numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }

  // Send message when button is pressed
  currentState = digitalRead(BUTTON_PIN);
  if (lastState == HIGH && currentState == LOW) {
    Serial.println("The button is pressed");
    bot.sendMessage(CHAT_ID, "HELP! I AM IN DANGER!");
  } else if (lastState == LOW && currentState == HIGH) {
    Serial.println("The button is released");
  }
  lastState = currentState;
  */
}

//------------------------------------FUNCTIONS------------------------------------
// Function to update user state (temporary, maybe move into class later)
// Takes user inputs and upper bound of array size (assume array is square for now?)
void updateInputs(int upper_bound){
  for (int i = 0; i < userState.size(); i++){
    userState[i] = userState[i] + userInput[i];
    if (userState[i] < 0){
      userState[i] = 0;
    } else if (userState[i] > upper_bound){
      userState[i] = upper_bound;
    }
  }
}

// Funcion to convert int vector into a string to print
String intToString(std::vector<int> intVector){
  std::string tempOutput = to_string(intVector[0]);
  for (int i = 1; i < intVector.size(); i++){
    tempOutput = tempOutput + " , " + to_string(intVector[i]);
  }
  String tempOutputString = String(tempOutput.data());
  return tempOutputString;
}

/*
// Allow telegram to query
void handleNewMessages(int numNewMessages) {
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));

  for (int i = 0; i < numNewMessages; i++) {
    // Check the chat id of the requester
    String chat_id = String(bot.messages[i].chat_id);
    if (chat_id != CHAT_ID) {
      bot.sendMessage(chat_id, "Unauthorized user", "");
      continue;
    }

    // Print the received message
    String text = bot.messages[i].text;
    Serial.println(text);
    Serial.println(chat_id);
    String from_name = bot.messages[i].from_name;

    // Change this message accordingly to the device function
    if (text == "/start") {
      String welcome = "Welcome, " + from_name + ".\n";
      welcome += "Use the following commands to control your outputs.\n\n";
      welcome += "/hello to say hello with bot \n";
      welcome += "/name to to ask bot's name \n";
      welcome += "/mode to ask is bot happy or not \n";
      welcome += "/afk to ask whether user holding the device \n";
      bot.sendMessage(chat_id, welcome, "");
    }

    if (text == "/hello") {
      bot.sendMessage(chat_id, "Hello, people", "");
    }

    if (text == "/name") {
      bot.sendMessage(chat_id, "My name is Zane Bot!", "");
    }

    if (text == "/mode") {
      bot.sendMessage(chat_id, "I am happy!", "");
    }

    // To query if the person is touching the device by questioning is the touchPinLED HIGH or LOW
    if (text == "/afk") {
      if (digitalRead(touchPinLED)) {
        bot.sendMessage(chat_id, "Person is touching the device", "");
      } else {
        bot.sendMessage(chat_id, "Person is afk", "");
      }
    }
  }
}
*/