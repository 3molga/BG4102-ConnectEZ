// Source file for joystick class

#include <Arduino.h>
#include <algorithm>
#include "joystick.h"
#include "joystickAxis.h"

int _joystickCurValues[2] = {0, 0};
int _joystickPrevValues[2] = {0, 0};
int joystickDebounceTime = 200; // In ms

// Constructor
joystick::joystick(int joystickXPin, int joystickYPin) : 
  joyX(joystickXPin, false), 
  joyY(joystickYPin, true) {
}


// joystickSetup
// Set up joystickAxis loops
void joystick::joystickSetup(){
  joyX.joystickAxisLoop();
  joyY.joystickAxisLoop();
}

// _joystickUpdateState
// Updates state of joystick
void joystick::_joystickUpdateState(){
  _joystickCurValues[0] = joyX.joystickAxisCurrentState;
  _joystickCurValues[1] = joyY.joystickAxisCurrentState;
}

// joystickStateTrigger
// Continuous checking of previous and current joystick axes, returns 1 if either axis has an update
// or if 200ms has passed with same state AND there is an input
bool joystick::joystickStateTrigger(){
  _joystickUpdateState();

  // If there is an update in state, return 1 and save the time the state update happened
  if (_joystickCurValues != _joystickPrevValues){
    unsigned long _joystickRecentStateUpdateTime = millis();
    return 1;
  
  // Else if time elapsed between last update and current time is greater than debounce time
  // return 1 and update time since last update
  } else if ((millis() - _joystickRecentStateUpdateTime) > joystickDebounceTime){
    unsigned long _joystickRecentStateUpdateTime = millis();
    return 1;

  // Else no update has happened, proceed as usual
  } else {
    return 0;
  }

  // Re-assign current joystick values to previous joystick values
  _joystickPrevValues[0] = _joystickCurValues[0];
  _joystickPrevValues[1] = _joystickCurValues[1];
} 

// joystickMessageCheck
// Call only if joystickStateTrigger returns a 1
String joystick::joystickMessageCheck(){
  if (joystickStateTrigger()){
    String joyXMsg = joyX.returnJoystickAxisToggledMsg();
    String joyYMsg = joyY.returnJoystickAxisToggledMsg();
    String joystickMsg = joyXMsg + " + " + joyYMsg;
    return joystickMsg;
  }
}