// Version 2 of joystick code
// Includes: support for all 4 directions
// Prints inputs to serial port only if a change is detected
// Now with object-oriented programming (?)

// Defining classes
#include "joystick.h"
#include <iostream>
using namespace std;

// Creating objects
// Joysticks
joystick joyX(13, 0);
joystick joyY(12, 1);

void setup() {
  Serial.begin(9600);
}

void loop() {
  // Check for message
  String joyXMsg = joyX.joystickToggledMsg();
  String joyYMsg = joyY.joystickToggledMsg();

  // If there is no message, end loop
  if (joyXMsg.isEmpty() && joyYMsg.isEmpty()){
    return;
  } 

  // If there is a message in at least one, return either a combination of both or either nonempty
  if (!joyXMsg.isEmpty()){
      if (!joyYMsg.isEmpty()){
        Serial.println(joyXMsg + " + " + joyYMsg);
      } else {
        Serial.println(joyXMsg);
      }
  } else {
    Serial.println(joyYMsg);
  }
}