// Version 2 of joystick code
// Includes: support for all 4 directions
// Prints inputs to serial port only if a change is detected
// Now with object-oriented programming (?)

// Defining classes
#include "joystick.h"
#include "joystickAxis.h"
#include <iostream>
#include <array>
using namespace std;

// Creating objects
joystick joystick(13, 12);

// Defining variables
std::array(int, 2) userInputs;

void setup() {
  Serial.begin(9600);
  joystick.joystickSetup();
}

void loop() {
  // If there is no update from joystick, do NOT execute the following lines
  if (!joystick.joystickStateTrigger()){
    return;
  }

  // Get and print joystick state and returned message
  userInputs = joystick.joystickReturnState();
  Serial.println(joystick.joystickMessageCheck());
  Serial.println(userInputs);
}