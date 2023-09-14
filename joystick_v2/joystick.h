// Include these lines by default
#ifndef joystick_h
#define joystick_h
#include <Arduino.h>

// Class declaration
class joystick{

// Public stuff
public:
	joystick(int joystickPin, bool joystickXY); // Creates object for each pin
	String joystickToggledMsg(); // Returns message from joystick

// Private stuff
private:
	int _joystickPin; // Pin int
	bool _joystickXY; // Joystick X or Y (0/1)
	int _joystickValue; // Joystick current value
	int _joystickCurrentState; // Joystick current state
	int _joystickPreviousState; // Stores previous state of joystick
	int joystickToggleCheck(int _joystickPin); // Checks if joystick is toggled
};

#endif