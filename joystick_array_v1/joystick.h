/* Header file for "joystick" class, meant to store information about the overall joystick
 It will construct 2 joystickAxis objects for each pin on the ESP32
 Include these lines by default
*/
#ifndef joystick_h
#define joystick_h
#include <Arduino.h>
#include <array>
#include "joystickAxis.h"

// Class declaration
class joystick {

// Public stuff
public:
  // Constructor and joystickAxis objects
	  joystick(int joystickXPin, int joystickYPin); // Creates object from X and Y pins
    joystickAxis joyX;
    joystickAxis joyY; 
    String joystickMessage;

  // Functions
    void joystickSetup();
    bool joystickStateTrigger();
    String joystickMessageCheck();
    String joystickReturnState(); 

// Private stuff
private:
  // Variables
    std::array<int, 2> _joystickCurValues;
    std::array<int, 2> _joystickPrevValues;
    unsigned long _joystickRecentStateUpdateTime;
  
  // Functions
    void _joystickUpdateState();
};

#endif