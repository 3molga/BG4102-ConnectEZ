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

// Input objects
joystick joystick(32, 35);
ezButton buttonConfirm(27); // Fill this in later ya dumb cunt
ezButton buttonReturn(26);
ezButton buttonTele(25);
#define touchPinInput 13
#define touchPinLED 12 // External LED is at Pin 26
#define ONBOARD_LED 2  // ON_BOARD LED value is 2

// Wifi setup
const char *ssid = "tmt";                                         // Wi-Fi ID
const char *password = "yeah8913";                                // Wi-Fi password
#define BOTtoken "6606670486:AAGP24iLfP047ysZ7yaAUHyv3cOf-iLKMqE" // Telegram bot API token
String CHAT_ID = "-4016407865";                                   // "Chat with esp32" telegram group ID
WiFiClientSecure client;
UniversalTelegramBot botActive(BOTtoken, client);
UniversalTelegramBot botPassive(BOTtoken, client);
telebot botController(botPassive);

// Joystick variables
std::vector<int> userState{0, 0};      // Stores x and y coords of where the user currently is on the UI
std::vector<int> userInput{0, 0};      // Stores inputs from user
std::vector<int> matrixSize = {3, 10}; // Arbitrary assumption for maximum size of array (+1)
bool joystickUpdateTrigger;
const String newLineBar = "-----------------------------------------------------------------------------------------------";

// Capacitance touch to detect if person is afk or not
int touchValue;                // Variable for storing the touch pin value
const int touchThreshold = 30; // Capacitance touch value lower than this will indicate the person is touching the device
bool touchBool;

TaskHandle_t Task1;

// -----------------------------------------SETUP-------------------------------------------
void setup()
{
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
  while (millis() < 5000)
  {
    if (WiFi.status() == WL_CONNECTED)
    {
      Serial.println("WiFi connected");
      botActive.sendMessage(CHAT_ID, "Hello everyone, I am connected!");
      break;
    }
    else
    {
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
      TelegramPollReturn, /* Task function. */
      "TelePoll",         /* name of task. */
      100000,             /* Stack size of task */
      NULL,               /* parameter of the task */
      1,                  /* priority of the task */
      &TelePoll,          /* Task handle to keep track of created task */
      1);                 /* pin task to core 1 */
}

// -----------------------------------------LOOP-------------------------------------------
void loop()
{
  buttonConfirm.loop();
  buttonReturn.loop();
  buttonTele.loop();
  joystickUpdateTrigger = joystick.joystickStateTrigger();

  // Check if the touchValue is below the threshold, check if the person is touching the device
  touchValue = touchRead(touchPinInput);
  if (touchValue < touchThreshold)
  {
    touchBool = true;
    digitalWrite(touchPinLED, HIGH);
  }
  else
  {
    touchBool = false;
    digitalWrite(touchPinLED, LOW);
  }

  // User input from button has higher precendence than joystick
  // Have individual actions for either confirm or return
  if (buttonConfirm.isPressed())
  {
    Serial.println("Confirm button pressed");
  }

  if (buttonReturn.isPressed())
  {
    Serial.println("Return button pressed");
  }

  if (buttonTele.isPressed())
  {
    Serial.println("Tele button pressed");
    if (WiFi.status() == WL_CONNECTED){
    botActive.sendMessage(CHAT_ID, "HELP! I AM IN DANGER!");
  }

  // Lowest priority state: only joystick input
  // Get and print joystick state and returned message
  if (joystickUpdateTrigger)
  {
    userInput = joystick.joystickReturnState();
    updateInputs();

    // Print everything
    Serial.println(joystick.joystickMessageCheck());
    Serial.println(intToString(userInput));
    Serial.println(intToString(userState));
    Serial.println(newLineBar);
  }

  // Check Wifi connection
}

/*2nd Loop
Handles all passive Telegram-related actions
ie. polling the API for chat updates and automatically responding to them.
Separate from the active Telegram-related actions (ie. initiated by the user.)*/
void TelePoll(void *pvParameters)
{
  while (1)
  {
    if (WiFi.status() != WL_CONNECTED)
    {
      // Wi-Fi is not connected LED
      digitalWrite(ONBOARD_LED, LOW);
      return;
    }
    else
    {
      // Wi-Fi is connected LED
      digitalWrite(ONBOARD_LED, HIGH);
      botController.handleUpdates();
    }
  }

  //------------------------------------FUNCTIONS------------------------------------
  // Function to update user state (temporary, maybe move into class later)
  // Takes user inputs and upper bound of array size (assume array is square for now?)
  void updateInputs()
  {
    if (WiFi.status() != WL_CONNECTED)
    {
      // Wi-Fi is not connected LED
      digitalWrite(ONBOARD_LED, LOW);
      return;
    }
    else
    {
      // Wi-Fi is connected LED
      digitalWrite(ONBOARD_LED, HIGH);
      Serial.println("Checkpoint 1");
      botController.handleUpdates();
      Serial.println("Checkpoint 2");
      if (buttonTele.isPressed())
      {
        bot.sendMessage(CHAT_ID, "HELP! I AM IN DANGER!");
      }
    }
  }

  // Funcion to convert int vector into a string to print
  String intToString(std::vector<int> intVector)
  {
    std::string tempOutput = to_string(intVector[0]);
    for (int i = 1; i < intVector.size(); i++)
    {
      tempOutput = tempOutput + " , " + to_string(intVector[i]);
    }
    String tempOutputString = String(tempOutput.data());
    return tempOutputString;
  }