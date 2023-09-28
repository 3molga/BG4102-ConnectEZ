// Source file for telegram bot behaviour

#include <Arduino.h>
#include <string>
#include <vector>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
#include "telebot.h"

// Private Variables
unsigned long lastTimeBotRan;
int numMessagesReceived;
int numMessagesQueued;
std::vector<std::string> messageQueue;

// External pins
extern bool touchBool;

//---------------------------------PUBLIC---------------------------------
// Constructor
telebot::telebot(UniversalTelegramBot &bot, String CHAT_ID)
  : botRequestDelay(1000),
    chat_id(CHAT_ID) {
  this->bot = &bot;
}

// handleActiveUpdates
// Sends all queued messages in the queue
void telebot::handleActiveUpdates() {
  if (numMessagesQueued) {
    while (numMessagesQueued) {
      // Send first message in queue
      // Convert from std::string to String for UniversalTelegramBot
      String messageToSend = String(messageQueue[0].c_str());
      bot->sendMessage(chat_id, messageToSend, "");

      // Update queue and number
      numMessagesQueued -= 1;
      messageQueue.erase(messageQueue.begin());
    }
  }
}

// handlePassiveUpdates
// Queries updates from Telegram and handles them
void telebot::handlePassiveUpdates() {
  if (millis() > lastTimeBotRan + botRequestDelay) {
    numMessagesReceived = bot->getUpdates(bot->last_message_received + 1);

    while (numMessagesReceived) {
      handleMessagesReceived(numMessagesReceived);
      numMessagesReceived = bot->getUpdates(bot->last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }
}

// queueMessage
// Queues another message to be sent by bot
void telebot::queueMessage(std::string messageToQueue) {
  numMessagesQueued += 1;
  messageQueue.push_back(messageToQueue);
}

// -----------------------PRIVATE-----------------------
void telebot::handleMessagesReceived(int numMessagesReceived) {
  for (int i = 0; i < numMessagesReceived; i++) {
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

    // To query if the person is touching the device
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
