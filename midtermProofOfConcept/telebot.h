// Header file for telebot class
// Controls telegram bot behaviour as a separate object, instead of stuffing it all in the main loop
#ifndef telebot_h
#define telebot_h
#include <Arduino.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

// Class declaration
class telebot {

// Public stuff
public:
	// Constructor
	telebot(UniversalTelegramBot &bot);

	// Functions
	void handleUpdates();

// Private stuff
private:
	// Constants
	const int botRequestDelay;

	// Variables
	unsigned long lastTimeBotRan;
	int numNewMessages;
	bool touchBool;

	// Functions
	void handleNewMessages(int numNewMessages);

	// External objects
	UniversalTelegramBot *bot;

};

#endif