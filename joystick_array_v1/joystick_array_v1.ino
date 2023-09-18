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
joystick joystick(2,4);
ezButton buttonConfirm(1); // Fill this in later ya dumb cunt
ezButton buttonReturn(2);

// Defining variables
std::array<int, 2> userState {0, 0}; // Stores x and y coords of where the user currently is on the UI
std::array<int, 2> userInput {0, 0};
int matrixSize = 3; // Arbitrary assumption for maximum size of array (+1)
bool userUpdateTrigger;

// Defining const
const String newLineBar = "-----------------------------------------------------------------------------------------------";

void setup() {
  Serial.begin(9600);
  joystick.joystickSetup();
  buttonConfirm.setDebounceTime(50);
  buttonReturn.setDebounceTime(50);
}

void loop() {
  buttonConfirm.loop();
  buttonReturn.loop();
  userUpdateTrigger = joystick.joystickStateTrigger();

  // If there is no update from joystick or either button, return early and don't execute the following code
  if (!(userUpdateTrigger|| buttonConfirm.isPressed() || buttonReturn.isPressed())){
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

  // Lowest priority state: only joystick input
  // Get and print joystick state and returned message
  userInput = joystick.joystickReturnState();
  Serial.println(joystick.joystickMessageCheck());
  Serial.println(printUserState(userInput));
  updateInputs(matrixSize);
  Serial.println(printUserState(userState));
  Serial.println(newLineBar);
}

//------------------------------------FUNCTIONS------------------------------------
// Function to update user state (temporary, maybe move into class later)
// Takes user inputs and upper bound of array size (assume array is square for now?)
void updateInputs(int upper_bound){
  for (int i = 0; i < userState.size(); i++){
    userState[i] = userState[i] + userInput[i];
    if (userState[i] < 0){
      userState[i] = 0;
    } else if (userState[i] > upper_bound){
      userState[i] = upper_bound;
    }
  }
}

// Funcion to convert user state into a string to print
String printUserState(std::array<int, 2> userState){
  std::string tempOutput = to_string(userState[0]);
  tempOutput = tempOutput + " , " + to_string(userState[1]);
  String tempOutputString = String(tempOutput.data());
  return tempOutputString;
}