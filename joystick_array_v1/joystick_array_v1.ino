// Version 2 of joystick code
// Includes: support for all 4 directions
// Prints inputs to serial port only if a change is detected
// Now with object-oriented programming (?)

// Defining classes
#include "joystick.h"
#include "joystickAxis.h"
#include "ezButton.h"
#include <iostream>
#include <array>
#include <string>
using namespace std;

// Creating objects
joystick joystick(13, 12);
ezButton buttonConfirm(); // Fill this in later ya dumb cunt
ezButton buttonReturn();


// Defining variables
std::array(int, 2) userState; // Stores x and y coords of where the user currently is on the UI

void setup() {
  Serial.begin(9600);
  joystick.joystickSetup();
  buttonConfirm.setDebounceTime(50);
  buttonReturn.setDebounceTime(50);
}

void loop() {
  buttonConfirm.loop();
  buttonReturn.loop();

  // If there is no update from joystick or either button, return early and don't execute the following code
  if (!(joystick.joystickStateTrigger() || buttonConfirm.isPressed() || buttonReturn.isPressed())){
    return;
  }

  // User input from button has higher precendence than joystick
  // If for whatever reason both confirm and return are pressed at the same time, return early
  if (buttonConfirm.isPressed() && buttonReturn.isPressed()){
    return;
  }

  // Else have individual actions for either confirm or return
  if (buttonConfirm.isPressed()){
    return;
  }

  if (buttonReturn.isPressed()){
    return;
  }

  // Lowest priority state: joystick input
  // Get and print joystick state and returned message
  userInputs = joystick.joystickReturnState();
  Serial.println(joystick.joystickMessageCheck());
  Serial.println(userInputs);
  updateInputs(userInputs);
  Serial.println(printUserState(userState))
}

//------------------------------------FUNCTIONS------------------------------------
// Function to update user state (temporary, maybe move into class later)
void updateInputs(std::array userInputs){
  // Assuming the grid is 2D, get the x and y coords of the current box
  int x_coord = userState[0];
  int y_coord = userState[1];

  // Update the coords accordingly
  x_coord = x_coord + userInputs[0];
  y_coord = y_coord + userInputs[1];

  // Check that the values are within the bounds
  // Will need to update this with some way to either dynamically get the grid size from some kinda object, or just define a static grid size
  // For now just check that it doesn't go negative
  if (x_coord < 0){
    x_coord = 0;
  } 

  if (y_coord < 0){
    y_coord = 0;
  }

  // Update user state
  userState[0] = x_coord;
  userState[1] = y_coord;
}

// Funcion to convert user state into a string to print
std::string printUserState(std::array userState){
  std::string tempOutput = to_string(userState[0]);
  tempOutput = tempOutput + " , " + to_string(userState[1]);
  return tempOutput;
}