// Source file for joystick class
// Includes joystickAxis class

#include <Arduino.h>
#include <array>
#include "joystick.h"
//#include "joystickAxis.h"

// joystick variables
const unsigned long joystickDebounceTime = 250; // In ms
String joystickMessage;
std::array<int, 2> _joystickPrevValues;
std::array<int, 2> _joystickCurValues;
unsigned long _joystickRecentStateUpdateTime;

// joystickAxis variables
const int threshold_high = 4500
const int threshold_low = 500
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

joystickAxis::joystickAxis(int joystickAxisPin, bool joystickAxisXY) {
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
void joystickAxis::joystickAxisLoop(){
  joystickAxisCurrentState = _joystickAxisToggleCheck();
  _joystickAxisUpdateMsg();
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

// Private function to check if joystickAxis is toggled
// 0 =  no toggle, 1 = upper toggle, 2 = lower toggle
int joystickAxis::_joystickAxisToggleCheck(){
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
void joystickAxis::_joystickAxisUpdateMsg(){
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
