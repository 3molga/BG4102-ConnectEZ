// Source file for joystick class

#include <Arduino.h>
#include "joystick.h"

// Constructor
joystick::joystick(int joystickXPin, int joystickYPin) {
  joystickAxis joyX(joystickXPin, 0);
  joystickAxis joyY(joystickYPin, 1);
}

// joystickStateCheck
// Continuous checking of previous and current joystick axes, returns 1 if either axis has an update
bool joystick::joystickStateCheck(){
  if (joyX.joystickAxisStateCheck() || joyY.joystickAxisStateCheck()){
    return 1;
  } else {
    return 0;
  }
}

// joystickMessageCheck
// Call only if joystickStateCheck returns a 1
String joystick::joystickMessageCheck(){
  if (joystickStateCheck){
    String joyXMsg = joyX.joystickAxisToggledMsg();
    String joyYMsg = joyY.joystickAxisToggledMsg();
    String joystickMsg = joyXMsg + " + " + joyYMsg;
    return joystickMsg;
  }
}