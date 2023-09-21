// Source file for joystick class

#include <Arduino.h>
#include <array>
#include "joystick.h"
#include "joystickAxis.h"

const unsigned long joystickUpdateTime = 750; // In ms
const unsigned long joystickDebounceTime = 500; 

String joystickMessage;

std::array<int, 2> _joystickPrevValues;
std::array<int, 2> _joystickCurValues;
unsigned long _joystickRecentStateUpdateTime;


//---------------------------------PUBLIC---------------------------------
// Constructor
joystick::joystick(int joystickXPin, int joystickYPin) : 
  joyX(joystickXPin, false), 
  joyY(joystickYPin, true) {
}

// joystickSetup
// Assigns initial state
void joystick::joystickSetup(){
  _joystickPrevValues =  {0, 0};
  _joystickRecentStateUpdateTime = 0;
  joystickMessage = "Starting Up...";
}

// joystickStateTrigger
// Continuous checking of previous and current joystick axes, returns 1 if either axis has an update
// or if 200ms has passed with same state AND there is an input
bool joystick::joystickStateTrigger(){
  _joystickUpdateState();
  unsigned long current_time = millis();

  // If there is an update in state and the time elapsed since last update is longer than debounce time
  // Return 1 and update _joystickCurValues
  if ((_joystickCurValues != _joystickPrevValues) && (current_time - _joystickRecentStateUpdateTime > joystickDebounceTime)){
    _joystickRecentStateUpdateTime = current_time;
    _joystickPrevValues = _joystickCurValues;
    return 1;
  }

  // Else if time elapsed between last update and current time is greater than debounce time
  // and last state isn't full neutral
  // return 1 and update time since last update
  if ((current_time - _joystickRecentStateUpdateTime >= joystickDebounceTime) && (_joystickPrevValues[0] || _joystickPrevValues[1])){
    _joystickRecentStateUpdateTime = current_time;
    _joystickPrevValues = _joystickCurValues; 
    return 1;

  // Else no update has happened, proceed as usual
  } else {
    return 0;
  }  
} 

// joystickMessageCheck
// Returns message with additional check for whether trigger is pulled
String joystick::joystickMessageCheck(){
  joystickMessage = joyX.joystickAxisMessage + " + " + joyY.joystickAxisMessage;
  return joystickMessage;
}

//joystickReturnState
//Returns state of joystick to main loop (messing around with pointers here)
std::array<int, 2> joystick::joystickReturnState(){
  return _joystickCurValues;
}

// ---------------------------------PRIVATE---------------------------------
// _joystickUpdateState
// Updates state of joystick
void joystick::_joystickUpdateState(){
  joyX.joystickAxisLoop();
  joyY.joystickAxisLoop();
  _joystickCurValues = {joyX.joystickAxisCurrentState, joyY.joystickAxisCurrentState};
}
