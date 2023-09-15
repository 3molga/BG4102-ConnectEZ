/* Header file for "joystick" class, meant to store information about the overall joystick
 It will construct 2 joystickAxis objects for each pin on the ESP32
 Include these lines by default
*/
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
  void joystickSetup();
  bool joystickStateTrigger();
  String joystickMessageCheck();

  // Variables
  int joystickDebounceTime();

// Private stuff
private:
  // Variables
  int _joystickCurValues[2];
  int _joystickPrevValues[2]; 
  unsigned long _joystickStateUpdateTime;
  
  // Functions
  void _joystickUpdateState();
};

#endif