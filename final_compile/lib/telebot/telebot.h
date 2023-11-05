// Header file for telebot class
// Controls telegram bot behaviour as a separate object, instead of stuffing it all in the main loop
#ifndef telebot_h
#define telebot_h
#include <Arduino.h>
#include <string>
#include <vector>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

// Class declaration
class telebot {

// Public stuff
public:
	// Constructor
	telebot(UniversalTelegramBot &bot, String chat_id);

	// Functions
	void handleActiveUpdates();
	void handlePassiveUpdates();
	void queueMessage(std::string messageQueued);

// Private stuff
private:
	// Constants
	const int botRequestDelay;
  const String chat_id;

	// Variables
	unsigned long lastTimeBotRan;
	int numMessagesReceived;
	int numMessagesQueued;
	std::vector<std::string> messageQueue;

	// Functions
	void handleMessagesReceived(int numMessagesReceived);

	// External objects
	UniversalTelegramBot *bot;

};

#endif