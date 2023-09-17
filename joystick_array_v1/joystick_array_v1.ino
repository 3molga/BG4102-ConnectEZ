// Version 2 of joystick code
// Includes: support for all 4 directions
// Prints inputs to serial port only if a change is detected
// Now with object-oriented programming (?)

// Defining classes
#include "joystick.h"
#include "joystickAxis.h"
#include "ezButton.h"
#include <iostream>
#include <array>
using namespace std;

// Creating objects
joystick joystick(13, 12);
ezButton buttonConfirm();
ezButton buttonReturn();


// Defining variables
std::array(int, 2) userInputs;

void setup() {
  Serial.begin(9600);
  joystick.joystickSetup();
  buttonConfirm.setDebounceTime(50);
  buttonReturn.setDebounceTime(50);
}

void loop() {
  buttonConfirm.loop();
  buttonReturn.loop();

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
  Serial.println(userInputs);
}