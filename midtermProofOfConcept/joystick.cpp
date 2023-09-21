// Source file for joystick class
// Includes joystickAxis class

#include <Arduino.h>
#include <array>
#include "joystick.h"

// joystick variables
const unsigned long joystickHoldTime = 750; 
const unsigned long joystickDebounceTime = 250; // In ms
String joystickMessage;
std::vector<int> _joystickPrevValues;
std::vector<int> _joystickCurValues;
unsigned long _joystickRecentStateUpdateTime;

// joystickAxis variables
const int threshold_high = 4000;
const int threshold_low = 50;
int _joystickAxisPin; // Pin int
bool _joystickAxisXY; // joystickAxis X or Y (0/1)
int _joystickAxisValue; // joystickAxis current value
int joystickAxisCurrentState; // joystickAxis current state
String joystickAxisMessage; // stores message to be sent out

//---------------------------------PUBLIC---------------------------------
// Constructors
joystick::joystick(int joystickXPin, int joystickYPin) : 
  joyX(joystickXPin, false), 
  joyY(joystickYPin, true) {
}

joystick::joystickAxis::joystickAxis(int joystickAxisPin, bool joystickAxisXY) {
	_joystickAxisPin = joystickAxisPin;
	_joystickAxisXY = joystickAxisXY;
}

// joystickSetup
// Assigns initial state
void joystick::joystickSetup(){
  _joystickPrevValues =  {0, 0};
  _joystickRecentStateUpdateTime = 0;
  joystickMessage = "Starting Up...";
}

// joystickAxis loop - continuously updates state of each axis
void joystick::joystickAxis::joystickAxisLoop(){
  joystickAxisCurrentState = _joystickAxisToggleCheck();
  _joystickAxisUpdateMsg();
}

// joystickStateTrigger
/* Continuous checking of previous and current joystick axes, returns 1 if 
 - New update above debounce time
 - Position hasn't changed, isn't neutral and above hold time
*/ 
bool joystick::joystickStateTrigger(){
  _joystickUpdateState();
  unsigned long current_time = millis();

  // If there is an update in state and time elapsed since last update is greater than debounce time
  if ((_joystickCurValues != _joystickPrevValues) && (current_time - _joystickRecentStateUpdateTime > joystickDebounceTime)){
    _joystickRecentStateUpdateTime = current_time;
    _joystickPrevValues = _joystickCurValues;
    return 1;
  }

  // Else if the current and previous states are same but NOT neutral and time elapsed since last update is greater than hold time
  if ((_joystickPrevValues[0] || joystickPrevValues[1]) && (current_time - _joystickRecentStateUpdateTime >= joystickHoldTime)){
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
  if (joystickStateTrigger()){
    joystickMessage = joyX.joystickAxisMessage + " + " + joyY.joystickAxisMessage;
    return joystickMessage;
  }
}

//joystickReturnState
//Returns state of joystick to main loop
std::vector<int> joystick::joystickReturnState(){
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

// Private function to check if joystickAxis is toggled
// 0 =  no toggle, 1 = upper toggle, 2 = lower toggle
int joystick::joystickAxis::_joystickAxisToggleCheck(){
	_joystickAxisValue = analogRead(_joystickAxisPin); // Reads analog value passed into joystickAxis pin
	
	// If joystickAxis value greater than upper threshold
	if (_joystickAxisValue > threshold_high){ 
		return 1;

	// If joystickAxis value lower than lower threshold
	} else if (_joystickAxisValue < threshold_low){ 
		return 2;

	// If joystickAxis value between thresholds
	} else { 
		return 0;
	}
}

// Private function to update message of joystickAxis state
void joystick::joystickAxis::_joystickAxisUpdateMsg(){
	// Return string corresponding to toggle state + joystickAxis XY
	// Higher threshold
	if (joystickAxisCurrentState == 1){
		if (_joystickAxisXY == 0){ 
			joystickAxisMessage =  "Right";
		} else {
			joystickAxisMessage = "Down";
		}

	} else if (joystickAxisCurrentState == 2){
		if (_joystickAxisXY == 0){
			joystickAxisMessage = "Left";
		} else {
			joystickAxisMessage = "Up";
		}

	} else {
		joystickAxisMessage = "Neutral";
	}
}
