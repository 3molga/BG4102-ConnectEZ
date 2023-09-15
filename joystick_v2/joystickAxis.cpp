// Source file for joystickAxis subclass

#include <Arduino.h>
#include "joystickAxis.h"

// joystickAxis thresholds
#define threshold_high 4500
#define threshold_low 500
int _joystickAxisPin = 0; // Pin int
bool _joystickAxisXY = 0; // joystickAxis X or Y (0/1)
int _joystickAxisValue = 0; // joystickAxis current value
int joystickAxisCurrentState = 0; // joystickAxis current state

// Constructor
joystickAxis::joystickAxis(int joystickAxisPin, bool joystickAxisXY) {
	_joystickAxisPin = joystickAxisPin;
	_joystickAxisXY = joystickAxisXY;
}

// Main public loop
void joystickAxis::joystickAxisLoop(){
  joystickAxisCurrentState = joystickAxisToggleCheck(_joystickAxisPin);
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

// Public function to return message of joystickAxis state
String joystickAxis::joystickAxisToggledMsg(int _joystickAxisCurrentState, int _joystickAxisPreviousState){
	// Return string corresponding to toggle state + joystickAxis XY
	if (joystickAxisCurrentState == 1){
		// Higher threshold
		if (_joystickAxisXY == 0){ 
			return "Left";
		} else {
			return "Up";
		}
	} else if (joystickAxisCurrentState == 2){
		if (_joystickAxisXY == 1){
			return "Right";
		} else {
			return "Down";
		}
	} else {
		return "Neutral";
	}
}
