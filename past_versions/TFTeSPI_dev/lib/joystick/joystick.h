/* Header file for "joystick" class, meant to store information about the overall joystick
 It will construct 2 joystickAxis objects for each pin on the ESP32
 Include these lines by default
*/
#ifndef joystick_h
#define joystick_h
#include <Arduino.h>
#include <vector>
#include "joystickAxis.h"

//  Class declaration
class joystick {

// Public stuff
public:
  // Constructor and joystickAxis objects
	  joystick(int joystickXPin, int joystickYPin, bool use_buffer); // Creates object from X and Y pins
    joystickAxis joyX;
    joystickAxis joyY; 
    String message;

  // Functions
    void setup();
    void stateTrigger();
    String messageCheck();
    uint8_t returnState();
    void returnBufferElement(uint8_t *buffer);
    void clearBufferElement();

// Private stuff
private:
  // Variables
    uint8_t _curValue;
    uint8_t _prevValue;
    std::vector<uint8_t> _buffer;
    bool _bufferuse;
    unsigned long _lastTriggered;
  
  // Functions
    void _updateState();
    void _addBufferElement(uint8_t _curValue);
};

#endif