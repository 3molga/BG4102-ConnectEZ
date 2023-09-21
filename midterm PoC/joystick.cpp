// Source file for joystick class

#include <Arduino.h>
#include <array>
#include "joystick.h"
#include "joystickAxis.h"

const unsigned long joystickDebounceTime = 250; // In ms

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

  // If there is an update in state, return 1 and save the time the state update happened
  if (_joystickCurValues != _joystickPrevValues){
    _joystickRecentStateUpdateTime = current_time;
    _joystickPrevValues.swap(_joystickCurValues);
    return 1;
  }

  // Else if time elapsed between last update and current time is greater than debounce time
  // return 1 and update time since last update
  if (current_time - _joystickRecentStateUpdateTime >= joystickDebounceTime){
    _joystickRecentStateUpdateTime = current_time;
    _joystickPrevValues.swap(_joystickCurValues); 
    return 1;

  // Else no update has happened, proceed as usual
  } else {
    return 0;
  }  
} 

// joystickMessageCheck
// Returns message with additional check for whether trigger is pulled
String joystick::joystickMessageCheck(){
  if (joystickStateTrigger()){
    joystickMessage = joyX.joystickAxisMessage + " + " + joyY.joystickAxisMessage;
    return joystickMessage;
  }
}

//joystickReturnState
//Returns state of joystick to main loop (messing around with pointers here)
std::array joystick::joystickReturnState(){
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
