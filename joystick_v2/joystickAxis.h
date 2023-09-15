// Header file for "joystickAxis" class
// Public child class of "joystick" class, meant to store information about each individual axis of the joystick
// Include these lines by default
#ifndef joystickAxis_h
#define joystickAxis_h
#include <Arduino.h>

// Class declaration
class joystickAxis {

// Public stuff
public:
  // Constructor
	joystickAxis(int joystickAxisPin, bool joystickAxisXY); // Creates object for each pin

  // Functions
  void joystickAxisLoop(); // Main loop to check state
  String returnJoystickAxisToggledMsg(); // Returns message

  // Variables
  int joystickAxisCurrentState; // joystickAxis current state

// Private stuff
private:
  // Variables
	int _joystickAxisPin; // Pin int
	bool _joystickAxisXY; // joystickAxis X or Y (0/1)
	int _joystickAxisValue; // joystickAxis current value

  // Functions
	int joystickAxisToggleCheck(int _joystickAxisPin); // Checks if joystickAxis is toggled
	String joystickAxisToggledMsg(); // Returns message from joystickAxis
};

#endif