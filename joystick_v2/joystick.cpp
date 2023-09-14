#include <Arduino.h>
#include "joystick.h"

// Joystick thresholds
#define threshold_high 4500
#define threshold_low 500
#define joystickPreviousState 0

// Define joystick object
joystick::joystick(int joystickPin, bool joystickXY) {
	_joystickPin = joystickPin;
	_joystickXY = joystickXY;
}

// Private function to check if joystick is toggled
// 0 =  no toggle, 1 = upper toggle, 2 = lower toggle
int joystick::joystickToggleCheck(int _joystickPin){
	int _joystickValue = analogRead(_joystickPin); // Reads analog value passed into joystick pin
	
	// If joystick value greater than upper threshold
	if (_joystickValue > threshold_high){ 
		return 1;

	// If joystick value lower than lower threshold
	} else if (_joystickValue < threshold_low){ 
		return 2;

	// If joystick value between thresholds
	} else { 
		return 0;
	}
}

// Public function to return message if joystick state is different from previous joystick state
String joystick::joystickToggledMsg(){
	// First compare previous joystick state to current joystick state
	int _joystickCurrentState = joystickToggleCheck(_joystickPin);
	
	// If no change is detected, return early
	if (_joystickCurrentState == joystickPreviousState){
		return "";
	}

	// If a change is detected, return string corresponding to toggle state + joystick XY
	if (_joystickCurrentState == 1){
		// Higher threshold
		if (_joystickXY == 0){ 
			return "Left";
		} else {
			return "Up";
		}
	} else if (_joystickCurrentState == 2){
		if (_joystickXY == 1){
			return "Right";
		} else {
			return "Down";
		}
	} else {
		return "";
	}

	// Assign current state to be previous state for next iteration
	_joystickPreviousState = _joystickCurrentState;
}

