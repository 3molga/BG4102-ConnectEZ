// Source file for joystickAxis subclass

#include <Arduino.h>
#include "joystickAxis.h"

// joystickAxis thresholds
#define threshold_high 4500
#define threshold_low 500
int _joystickAxisPin = 0; // Pin int
bool _joystickAxisXY = 0; // joystickAxis X or Y (0/1)
int _joystickAxisValue = 0; // joystickAxis current value
int _joystickAxisCurrentState = 0; // joystickAxis current state
int _joystickAxisPreviousState = 0; // Stores previous state of joystickAxis

// Constructor
joystickAxis::joystickAxis(int joystickAxisPin, bool joystickAxisXY) {
	_joystickAxisPin = joystickAxisPin;
	_joystickAxisXY = joystickAxisXY;
}

// Main public loop
void joystickAxis::joystickAxisLoop(){
  _joystickAxisValue = joystickAxisToggleCheck(_joystickAxisPin);
    
}

// Public function to return message of joystickAxis state
String joystickAxis::joystickAxisToggledMsg(int _joystickAxisCurrentState, int _joystickAxisPreviousState){
	// Return string corresponding to toggle state + joystickAxis XY
	if (_joystickAxisCurrentState == 1){
		// Higher threshold
		if (_joystickAxisXY == 0){ 
			return "Left";
		} else {
			return "Up";
		}
	} else if (_joystickAxisCurrentState == 2){
		if (_joystickAxisXY == 1){
			return "Right";
		} else {
			return "Down";
		}
	} else {
		return "Neutral";
	}
}

// Public function to check if joystickAxis axis current state is different to previous state
bool joystickAxis::joystickAxisStateCheck(int _joystickAxisPin, int joystickAxisPreviousState){
	// First compare previous joystickAxis state to current joystickAxis state
	int _joystickAxisCurrentState = joystickAxisToggleCheck(_joystickAxisPin);

  // If joystickAxis current state is different to previous state, return 1; else return 0
  if (_joystickAxisCurrentState == _joystickAxisPreviousState){
    return 0;
  } else {
    return 1;
  }

  // Assign current state to be previous state for next iteration
	_joystickAxisPreviousState = _joystickAxisCurrentState;
}

// Private function to check if joystickAxis is toggled
// 0 =  no toggle, 1 = upper toggle, 2 = lower toggle
int joystickAxis::joystickAxisToggleCheck(int _joystickAxisPin){
	int _joystickAxisValue = analogRead(_joystickAxisPin); // Reads analog value passed into joystickAxis pin
	
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
