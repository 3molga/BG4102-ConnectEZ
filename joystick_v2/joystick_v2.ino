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
// Joystick 
joystick joystick(13, 12);

void setup() {
  Serial.begin(9600);
  joystick.joystickSetup();
}

void loop() {
  // Check for update to joystick and print message if there is one
  if (joystick.joystickStateTrigger()){
    Serial.println(joystick.joystickMessageCheck());
  }
}