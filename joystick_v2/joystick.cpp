// Source file for joystick class

#include <Arduino.h>
#include "joystick.h"

int _joystickCurValues[2] = {0, 0};
int _joystickPrevValues[2] = {0, 0};
int joystickDebounceTime = 200; // In ms

// Constructor
joystick::joystick(int joystickXPin, int joystickYPin) {
  joystickAxis joyX(joystickXPin, false);
  joystickAxis joyY(joystickYPin, true);
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
    unsigned long _joystickStateUpdateTime = millis();
    return 1;
  
  // Else if time elapsed between last update and current time is greater than debounce time
  // return 1 and update time since last update
  } else if ((millis() - _joystickStateUpdateTime) > joystickDebounceTime){
    unsigned long _joystickStateUpdateTime = millis();
    return 1;

  // Else no update has happened, proceed as usual
  } else {
    return 0;
  }

  // Re-assign current joystick values to previous joystick values
  _joystickPrevValues = _joystickCurValues;
}

// joystickMessageCheck
// Call only if joystickStateTrigger returns a 1
String joystick::joystickMessageCheck(){
  if (joystickStateTrigger()){
    String joyXMsg = joyX.joystickAxisToggledMsg();
    String joyYMsg = joyY.joystickAxisToggledMsg();
    String joystickMsg = joyXMsg + " + " + joyYMsg;
    return joystickMsg;
  }
}