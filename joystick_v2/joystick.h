// Header file for "joystick" class, meant to store information about the overall joystick
// It will construct 2 joystickAxis 
// Include these lines by default
#ifndef joystick_h
#define joystick_h
#include <Arduino.h>
#include "joystickAxis.h"

// Class declaration
class joystick {

// Public stuff
public:
  // Constructor and joystickAxis objects
	joystick(int joystickXPin, int joystickYPin); // Creates object from X and Y pins
  extern joystickAxis joyX(int joystickXPin, bool 0);
  extern joystickAxis joyY(int joystickYPin, bool 1);

  // Functions
  bool joystickStateCheck();
  String joystickMessageCheck();

// Private stuff
private:
};

#endif