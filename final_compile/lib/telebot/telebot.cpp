// Source file to control telegram bot behaviour

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
int numMessagesSent;
int chatHistoryID;
std::vector<std::string> messageQueue;
std::vector<std::string> sentenceHistory;
std::vector<std::string> rawHistory;

//---------------------------------PUBLIC---------------------------------
// Constructor
telebot::telebot(UniversalTelegramBot &bot, String CHAT_ID)
    : botRequestDelay(1000),
      chat_id(CHAT_ID)
{
  this->bot = &bot;
}

// handleActiveUpdates
// Sends all queued messages in the queue
void telebot::handleActiveUpdates()
{
  if (numMessagesQueued)
  {
    while (numMessagesQueued)
    {
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
void telebot::handlePassiveUpdates()
{
  if (millis() > lastTimeBotRan + botRequestDelay)
  {
    numMessagesReceived = bot->getUpdates(bot->last_message_received + 1);

    while (numMessagesReceived)
    {
      handleMessagesReceived(numMessagesReceived);
      numMessagesReceived = bot->getUpdates(bot->last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }
}

// queueMessage
// Queues another message to be sent by bot
void telebot::queueMessage(std::string messageToQueue)
{
  numMessagesQueued += 1;
  numMessagesSent += 1;
  chatHistoryID += 1;
  messageQueue.push_back(messageToQueue);
}

// queueNewUserInput
// Queues a new user input to be sent by bot
void telebot::queueNewUserInput(std::string formattedSentence, std::string rawInput)
{
  // Add both the formatted sentence and the raw input to the history
  sentenceHistory.push_back(formattedSentence);
  rawHistory.push_back(rawInput);

  // And format them into an entire message to be sent
  std::string entireMessage = "USER IS SENDING A MESSAGE:\n\n";
  entireMessage += "Formatted Sentence: ";
  entireMessage += formattedSentence;
  entireMessage += "\nRaw User Input: ";
  entireMessage += rawInput;

  // And away we go!
  queueMessage(entireMessage);
}

// -----------------------PRIVATE-----------------------
void telebot::handleMessagesReceived(int numMessagesReceived)
{
  for (int i = 0; i < numMessagesReceived; i++)
  {
    // Check the chat id of the requester
    String chat_id = String(bot->messages[i].chat_id);

    // Print the received message to strings
    String text = bot->messages[i].text;
    String from_name = bot->messages[i].from_name;

    // Change this message accordingly to the device function
    if (text == "/start")
    {
      String welcome = "Welcome, " + from_name + ".\n";
      welcome += "Use the following commands to control your outputs:\n\n";
      welcome += "/info for information about our device;\n";
      welcome += "/history for the last messages sent by the user;\n";
      // welcome += "/easter for a surprise easter egg!"
      bot->sendMessage(chat_id, welcome, "");
      return;
    }

    // Returns information about our device
    if (text == "/info")
    {
      bot->sendMessage(chat_id, "This is the ConnectEZ, an AAC device designed to prioritize user personalization and maximise social interaction!", "");
      return;
    }

    // Returns last chat history
    if (text == "/history")
    {
      String messageHistoryString = "This is the earliest message sent by the user: \n\n";
      messageHistoryString += sentenceHistory[0].c_str();
      messageHistoryString += "\n\nUse the following commands to navigate the chat history: \n";
      messageHistoryString += "/previous to retrieve the previous message from this history; \n";
      messageHistoryString += "/next to retrieve the next message from this history.";
      bot->sendMessage(chat_id, messageHistoryString, "");
      return;
    }

    // /previous
    if (text == "/previous")
    {
      if (chatHistoryID - 1 > 0) // Ignore the very first message, which is the startup message (sent from outside)
      {
        chatHistoryID -= 1;
        String messagePrevString = "Retrieving previous message: \n\n";
        messagePrevString += sentenceHistory[chatHistoryID].c_str();
        bot->sendMessage(chat_id, messagePrevString);
        return;
      }
      return;
    }

    // /next
    if (text == "/next")
    {
      if (chatHistoryID + 1 <= numMessagesSent)
      {
        chatHistoryID += 1;
        String messageNextString = "Retrieving next message: \n\n";
        messageNextString += sentenceHistory[chatHistoryID].c_str();
        bot->sendMessage(chat_id, messageNextString);
        return;
      }
      return;
    }

    // Easter egg :)
    if (text == "/easter" || text == "/owo" || text == "/:3")
    {
      bot->sendMessage(chat_id, "A good :3 to you as well!", "");
      return;
    }
  }
}
