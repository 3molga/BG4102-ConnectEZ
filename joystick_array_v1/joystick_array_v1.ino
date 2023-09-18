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
ezButton buttonConfirm(1); // Fill this in later ya dumb cunt
ezButton buttonReturn(2);


// Defining variables
std::array<int, 2> userState; // Stores x and y coords of where the user currently is on the UI

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
  userInput = joystick.joystickReturnState();
  Serial.println(joystick.joystickMessageCheck());
  Serial.println(userInput);
  updateInputs(userInput, 3);
  Serial.println(printUserState(userState))
}

//------------------------------------FUNCTIONS------------------------------------
// Function to update user state (temporary, maybe move into class later)
// Takes user inputs and upper bound of array size (assume array is square for now?)
void updateInputs(std::array<int, 2> userInput, int upper_bound){
  for (int i = 0; i < userState.size(); i++){
    userState[i] = userState[i] + userInput[i];
    if (userState[i] < 0){
      userState[i] = 0;
    } else if (userState[i > upper_bound]){
      userState[i] = upper_bound;
    }
  }
}

// Funcion to convert user state into a string to print
std::string printUserState(std::array userState){
  std::string tempOutput = to_string(userState[0]);
  tempOutput = tempOutput + " , " + to_string(userState[1]);
  return tempOutput;
}