// Version 3 of joystick code: midterm rush edition
// Includes: support for all 4 directions
// Prints inputs to serial port only if a change is detected
// Now with object-oriented programming for joystick

// Defining classes
#include "joystick.h"
//#include "joystickAxis.h"
#include "ezButton.h"
#include <iostream>
#include <vector>
#include <string>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
using namespace std;

// Creating objects
joystick joystick(13, 12);
ezButton buttonConfirm(1); // Fill this in later ya dumb cunt
ezButton buttonReturn(2);

// Wifi setup
const char* ssid = "Zane’s iPhone"; // Wi-Fi ID
const char* password = "zane1234"; // Wi-Fi password
#define BOTtoken "6606670486:AAGP24iLfP047ysZ7yaAUHyv3cOf-iLKMqE" // Telegram bot API token
#define CHAT_ID "-4016407865" // "Chat with esp32" telegram group ID
WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);
int botRequestDelay = 1000;
unsigned long lastTimeBotRan;

// Defining variables
std::vector<int> userState; // Stores x and y coords of where the user currently is on the UI
std::vector<int> userInputs;

// Wi-Fi is connected confirmation LED
#define ONBOARD_LED  2 // ON_BOARD LED value is 2

// Button to send message
#define BUTTON_PIN 21  // GPIO21 pin connected to button
int lastState = LOW;  // the previous state from the input pin
int currentState;     // the current reading from the input pin

// set pin numbers
const int touchPin = 4; // Capacitance touch to detect if person is afk or not
int touchValue; // Variable for storing the touch pin value 
const int threshold = 30; // Capacitance touch value lower than this will indicate the person is touching the device
const int ledPin = 22; // External LED is 
bool ledState = LOW; // Set initial state as low 

// -----------------------------------------SETUP-------------------------------------------
void setup() {
  Serial.begin(9600);
  joystick.joystickSetup();
  buttonConfirm.setDebounceTime(50);
  buttonReturn.setDebounceTime(50);

  // Connect to Wi-Fi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  client.setCACert(TELEGRAM_CERTIFICATE_ROOT);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");  
  bot.sendMessage(CHAT_ID, "Hello everyone, I am connected!");

  // Setup button, Wi-Fi confirmaton LED and afk LED
  pinMode(ONBOARD_LED,OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, ledState);
}

// -----------------------------------------LOOP-------------------------------------------
void loop() {
  buttonConfirm.loop();
  buttonReturn.loop();

  // Check if the touchValue is below the threshold, check if the person is touching the device
  touchValue = touchRead(touchPin); 
  if(touchValue < threshold){
    digitalWrite(ledPin, HIGH);
    Serial.println(" - LED on");
  }
  else{
    digitalWrite(ledPin, LOW);
  }
  delay(100);
  
  // Handling query from telegram user
  if (millis() > lastTimeBotRan + botRequestDelay) {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while(numNewMessages) {
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
  }
  else if (lastState == LOW && currentState == HIGH) {
    Serial.println("The button is released");
  }
  lastState = currentState;

  // If Wi-Fi is connected, this ON_BOARD led will blink
  delay(100);
  digitalWrite(ONBOARD_LED,HIGH);
  delay(100);
  digitalWrite(ONBOARD_LED,LOW);

  // If there is no update from joystick or either button, return early and don't execute the following code
  if (!(joystick.joystickStateTrigger() || buttonConfirm.isPressed() || buttonReturn.isPressed())){
    return;
  }

  // User input from button has higher precendence than joystick
  // If for whatever reason both confirm and return are pressed at the same time, return early
  if (buttonConfirm.isPressed() && buttonReturn.isPressed()){
    return;
  }

  // Else have individual actions for either confirm or return
  if (buttonConfirm.isPressed()){
    return;
  }

  if (buttonReturn.isPressed()){
    return;
  }

  // Lowest priority state: joystick input
  // Get and print joystick state and returned message
  userInputs = joystick.joystickReturnState();
  Serial.println(joystick.joystickMessageCheck());
  updateInputs(userInputs);
  Serial.println(vector2Str(userState));
}

//------------------------------------FUNCTIONS------------------------------------
// Function to update user state (temporary, maybe move into class later)
void updateInputs(std::vector<int> userInputs){
  // Assuming the grid is 2D, get the x and y coords of the current box
  int x_coord = userState[0];
  int y_coord = userState[1];

  // Update the coords accordingly
  x_coord = x_coord + userInputs[0];
  y_coord = y_coord + userInputs[1];

  // Check that the values are within the bounds
  // Will need to update this with some way to either dynamically get the grid size from some kinda object, or just define a static grid size
  // For now just check that it doesn't go negative
  if (x_coord < 0){
    x_coord = 0;
  } 

  if (y_coord < 0){
    y_coord = 0;
  }

  // Update user state
  userState[0] = x_coord;
  userState[1] = y_coord;
}

// Funcion to convert vector input into a string to print
String vector2Str(std::vector<int> inputVector){
  std::string stdOutput = to_string(inputVector[0]);
  for (int i = 1; i < inputVector.size(); i++){
    stdOutput = stdOutput + " + " + to_string(inputVector[i]);
  }
  String strOutput = String(stdOutput.c_str());
  return strOutput;
}


// Allow telegram to query 
void handleNewMessages(int numNewMessages) {
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));

  for (int i=0; i<numNewMessages; i++) {
    // Check the chat id of the requester
    String chat_id = String(bot.messages[i].chat_id);
    if (chat_id != CHAT_ID){
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

    // To query if the person is touching the device by questioning is the ledPin HIGH or LOW
    if (text == "/afk") {
      if (digitalRead(ledPin)){
        bot.sendMessage(chat_id, "Person is touching the device", "");
      }
      else{
        bot.sendMessage(chat_id, "Person is afk", "");
      }
    }
  }
}