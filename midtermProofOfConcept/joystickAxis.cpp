// Source file for joystickAxis subclass

#include <Arduino.h>
#include "joystickAxis.h"

// joystickAxis thresholds
const int threshold_high = 4500
const int threshold_low = 500
int _joystickAxisPin; // Pin int
bool _joystickAxisXY; // joystickAxis X or Y (0/1)
int _joystickAxisValue; // joystickAxis current value
int joystickAxisCurrentState; // joystickAxis current state
String joystickAxisMessage; // stores message to be sent out

// Constructor
joystickAxis::joystickAxis(int joystickAxisPin, bool joystickAxisXY) {
	_joystickAxisPin = joystickAxisPin;
	_joystickAxisXY = joystickAxisXY;
}

// Main public loop
void joystickAxis::joystickAxisLoop(){
  joystickAxisCurrentState = _joystickAxisToggleCheck();
  _joystickAxisUpdateMsg();
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