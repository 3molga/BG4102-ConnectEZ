// Source file for telegram bot behaviour

#include <Arduino.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
#include "telebot.h"

// Private Variables
unsigned long lastTimeBotRan;
int numNewMessages;

// External pins
extern bool touchBool;

//---------------------------------PUBLIC---------------------------------
// Constructor
telebot::telebot(UniversalTelegramBot &bot)
  : botRequestDelay(1000) {
  this->bot = &bot;
}

// handleUpdates
// Queries updates from Telegram and handles them
void telebot::handleUpdates() {
  if (millis() > lastTimeBotRan + botRequestDelay) {
    numNewMessages = bot->getUpdates(bot->last_message_received + 1);

    while (numNewMessages) {
      handleNewMessages(numNewMessages);
      numNewMessages = bot->getUpdates(bot->last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }
}

// -----------------------PRIVATE-----------------------
void telebot::handleNewMessages(int numNewMessages) {
  for (int i = 0; i < numNewMessages; i++) {
    // Check the chat id of the requester
    String chat_id = String(bot->messages[i].chat_id);

    // Print the received message to strings
    String text = bot->messages[i].text;
    String from_name = bot->messages[i].from_name;

    // Change this message accordingly to the device function
    if (text == "/start") {
      String welcome = "Welcome, " + from_name + ".\n";
      welcome += "Use the following commands to control your outputs.\n\n";
      welcome += "/hello to say hello with bot \n";
      welcome += "/name to to ask bot's name \n";
      welcome += "/mode to ask is bot happy or not \n";
      welcome += "/afk to ask whether user holding the device \n";
      bot->sendMessage(chat_id, welcome, "");
    }

    if (text == "/hello") {
      bot->sendMessage(chat_id, "Hello, people", "");
      return;
    }

    if (text == "/name") {
      bot->sendMessage(chat_id, "My name is Zane Bot!", "");
      return;
    }

    if (text == "/mode") {
      bot->sendMessage(chat_id, "I am happy!", "");
      return;
    }

    // To query if the person is touching the device by questioning is the touchPinLED HIGH or LOW
    if (text == "/afk") {
      if (touchBool) {
        bot->sendMessage(chat_id, "Person is touching the device", "");
        return;
      } else {
        bot->sendMessage(chat_id, "Person is afk", "");
        return;
      }
    }
  }
}
