// Source file for joystick class

#include <Arduino.h>
#include <array>
#include "joystick.h"
#include "joystickAxis.h"

const unsigned long updateTime = 400; // In ms
const unsigned long debounceTime = 150;

// Public variables
String message;

// Private variables
uint8_t _prevValue;
uint8_t _curValue;
unsigned long _lastTriggered;
std::vector<uint8_t> _buffer;
bool _bufferuse = false;

//---------------------------------PUBLIC---------------------------------
// Constructor
joystick::joystick(int joystickXPin, int joystickYPin, bool use_buffer) : joyX(joystickXPin, false),
                                                                          joyY(joystickYPin, true)
{
  if (use_buffer)
  {
    _bufferuse = true;
  }
}

// joystickSetup
// Assigns initial state
void joystick::setup()
{
  _prevValue = 0;
  _lastTriggered = 0;
  message = "Starting Up...";
}

// stateTrigger
// Continuous checking of previous and current joystick axes, returns 1 if either axis has an update
// or if 200ms has passed with same state AND there is an input
void joystick::stateTrigger()
{
  _updateState();
  unsigned long current_time = millis();

  // If there is an update in state and the time elapsed since last update is longer than debounce time
  // Return 1 and update _joystickCurValues
  if ((_curValue != _prevValue) && (current_time - _lastTriggered > debounceTime))
  {
    _lastTriggered = current_time;
    _prevValue = _curValue;
    if (_bufferuse)
    {
      _addBufferElement(_curValue);
    }
  }

  // Else if time elapsed between last update and current time is greater than minimum update time
  // and last state isn't full neutral
  // return 1 and update time since last update
  else if ((current_time - _lastTriggered >= updateTime) && (_prevValue == _curValue))
  {
    _lastTriggered = current_time;
    _prevValue = _curValue;
    if (_bufferuse)
    {
      _addBufferElement(_curValue);
    }
  }
}

// messageCheck
// Returns message with additional check for whether trigger is pulled
String joystick::messageCheck()
{
  message = joyX.joystickAxisMessage + " + " + joyY.joystickAxisMessage;
  return message;
}

// returnState
// Returns state of joystick to main loop (messing around with pointers here)
uint8_t joystick::returnState()
{
  return _curValue;
}

// returnBuffer
// Returns _buffer's first element in a pass-by-ref way
void joystick::returnBufferElement(uint8_t *bufferElement)
{
  if (!_buffer.empty())
  {
    *bufferElement = _buffer[0];
  }
  else
  {
    *bufferElement = 0;
  }
}

// clearBufferElement
// Clears _buffer's first element
void joystick::clearBufferElement()
{
  _buffer.erase(_buffer.begin());
}

// ---------------------------------PRIVATE---------------------------------
// updateState
// Updates state of joystick
// Update: instead of storing in an int vector, return 1 of 9 possible values mapped to combination of inputs
void joystick::_updateState()
{
  joyX.joystickAxisLoop();
  joyY.joystickAxisLoop();

  // This will be kinda messy... call it the burden of legacy code x)
  if ((joyX.joystickAxisCurrentState == 0) && (joyY.joystickAxisCurrentState == 0))
  { // Both are inactive
    _curValue = 0;
  }
  else if ((joyX.joystickAxisCurrentState == -1) && (joyY.joystickAxisCurrentState == 0))
  { // X is left
    _curValue = 1;
  }
  else if ((joyX.joystickAxisCurrentState == 1) && (joyY.joystickAxisCurrentState == 0))
  { // X is right
    _curValue = 2;
  }
  else if ((joyX.joystickAxisCurrentState == 0) && (joyY.joystickAxisCurrentState == -1))
  { // Y is up
    _curValue = 3;
  }
  else if ((joyX.joystickAxisCurrentState == 0) && (joyY.joystickAxisCurrentState == 1))
  { // Y is down
    _curValue = 4;
  }
  else if ((joyX.joystickAxisCurrentState == -1) && (joyY.joystickAxisCurrentState == -1))
  { // X is left, Y is up
    _curValue = 5;
  }
  else if ((joyX.joystickAxisCurrentState == -1) && (joyY.joystickAxisCurrentState == 1))
  { // X is left, Y is down
    _curValue = 6;
  }
  else if ((joyX.joystickAxisCurrentState == 1) && (joyY.joystickAxisCurrentState == -1))
  { // X is right, Y is up
    _curValue = 7;
  }
  else if ((joyX.joystickAxisCurrentState == 1) && (joyY.joystickAxisCurrentState == 1))
  { // X is right, Y is down
    _curValue = 8;
  }
  else
  {
    _curValue = 0; // Default to 0
  }
}

// _addBufferElement
// Does what it implies
void joystick::_addBufferElement(uint8_t _curValue)
{
  if (!_bufferuse)
  {
    return;
  }

  switch (_curValue)
  {
  case 0:
    break; // Do nothing, never EVER append 0s
  case 1:
    _buffer.push_back(1);
    break;
  case 2:
    _buffer.push_back(2);
    break;
  case 3:
    _buffer.push_back(3);
    break;
  case 4:
    _buffer.push_back(4);
    break;
  case 5:
    _buffer.push_back(1);
    _buffer.push_back(3);
    break;
  case 6:
    _buffer.push_back(1);
    _buffer.push_back(4);
    break;
  case 7:
    _buffer.push_back(2);
    _buffer.push_back(3);
    break;
  case 8:
    _buffer.push_back(2);
    _buffer.push_back(4);
    break;
  }
}