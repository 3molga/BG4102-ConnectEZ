/* Header file for "joystick" class, meant to store information about the overall joystick
 It will construct 2 joystickAxis objects for each pin on the ESP32
 Include these lines by default
*/
#ifndef joystick_h
#define joystick_h
#include <Arduino.h>
#include <vector>
// #include "joystickAxis.h"

// Class declaration
class joystick
{

  // Public stuff
public:
  // Public class for joystickAxis
  class joystickAxis
  {

    // Public stuff
  public:
    // Constructor
    joystickAxis(int joystickAxisPin, bool joystickAxisXY); // Creates object for each pin

    // Functions
    void joystickAxisLoop();               // Main loop to check state
    String returnJoystickAxisToggledMsg(); // Returns message

    // Variables
    int joystickAxisCurrentState; // joystickAxis current state
    String joystickAxisMessage;   // joystickAxis message

    // Private stuff
  private:
    // Variables
    int _joystickAxisPin;   // Pin int
    bool _joystickAxisXY;   // joystickAxis X or Y (0/1)
    int _joystickAxisValue; // joystickAxis current value

    // Functions
    int _joystickAxisToggleCheck(); // Checks if joystickAxis is toggled
    void _joystickAxisUpdateMsg();  // Updates message from joystickAxis
  };

    // Constructor and joystickAxis objects
  joystick(int joystickXPin, int joystickYPin); // Creates object from X and Y pins
  joystickAxis joyX;
  joystickAxis joyY;
  String joystickMessage;

  // Functions
  void joystickSetup();
  bool joystickStateTrigger();
  String joystickMessageCheck();
  std::vector<int> joystickReturnState();

  // Private stuff
private:
  // Variables
  std::vector<int> _joystickCurValues;
  std::vector<int> _joystickPrevValues;
  unsigned long _joystickRecentStateUpdateTime;

  // Functions
  void _joystickUpdateState();
};

#endif