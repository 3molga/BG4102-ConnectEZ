// Modified from:
// https://esp32io.com/tutorials/esp32-joystick

// Version 1 of joystick code
// Includes: support for all 4 directions + button presses
// Prints inputs to serial port only if a change is detected

#include <ezButton.h>

// Pin wiring
#define joy_X        13 // ESP32 pin D13
#define joy_Y        12 // ESP32 pin D12
#define joy_button   14 // ESP32 pin D14

// Define constant thresholds
#define LEFT_THRESHOLD  500
#define RIGHT_THRESHOLD 4500
#define UP_THRESHOLD    500
#define DOWN_THRESHOLD  4500
#define COMMAND_NO     0x00 //00000000
#define COMMAND_LEFT   0x01 //00000001
#define COMMAND_RIGHT  0x02 //00000010
#define COMMAND_UP     0x04 //00000100
#define COMMAND_DOWN   0x08 //00001000

// Silliness
#define CHECK_BIT(var,pos) ((var>>pos) & 1)

int valueX = 0 ; // to store the X-axis value
int valueY = 0 ; // to store the Y-axis value
int valueB = 0; // to store the button press
int command_bit = COMMAND_NO; // Initial definition of command_array as bit
int last_command = command_bit; // Initial definition to store last command_array

// Declaring functions
void returnJoystickMsg();
void joystickBitMsg();

ezButton button(joy_button);

void setup() {
  Serial.begin(9600) ;
  button.setDebounceTime(50);
}

void loop() {
  button.loop(); 

  // read X and Y analog values
  valueX = analogRead(joy_X);
  valueY = analogRead(joy_Y);
  valueB = button.getState();

  // converts the analog value to commands
  // reset commands
  command_bit = COMMAND_NO;

// check button press (escape clause)
  if (button.isPressed()) {
    String joystick_msg = "Button Pressed";
    Serial.println(joystick_msg);
    return;
  }

  // check left/right commands
  if (valueX < LEFT_THRESHOLD) {
    command_bit = command_bit | COMMAND_LEFT;
  } else if (valueX > RIGHT_THRESHOLD) {
    command_bit = command_bit | COMMAND_RIGHT;
  }

  // check up/down commands
  if (valueY < UP_THRESHOLD) {
    command_bit = command_bit | COMMAND_UP;
  } else if (valueY > DOWN_THRESHOLD) {
    command_bit = command_bit | COMMAND_DOWN;
  }

  // NOTE: there are multiple combinations of commands (based on bits)
  // Compare present command_bit state to previous command_bit state
  if (command_bit != last_command) {
    String joystick_msg = returnJoystickMsg(command_bit);
    Serial.println(joystick_msg);
  }

  // Save this state to compare with next state
  last_command = command_bit;
}

// Function to compare value of command_bit to reference, then return a string message to be passed to serial output
String returnJoystickMsg(int command_bit) {
  if (command_bit == 0) { //Return early if joystick is reset
    return "Joystick Neutral";
  }

  bool first_input = true;
  String output_msg = ""; // Initialize empty string for output

  // Compare each bit in command_bit
  for (int i = 0; i < 4; i++) {
    if (CHECK_BIT(command_bit, i)) {
      if (first_input == true){
        output_msg = joystickBitMsg(i);
        first_input = false;
      } else {
        output_msg = output_msg + " + " + joystickBitMsg(i);
      }
    }
  }
  return output_msg;
}

// Function to return specific message based on bit value of command_bit that is set
String joystickBitMsg(int bit_input) {
  if (bit_input == 3) { //Joystick toggled down
    return "Down";
  } else if (bit_input == 2) { //Joystick toggled up
    return "Up";
  } else if (bit_input == 1) { // Joystick toggled right
    return "Right";
  } else if (bit_input == 0) { //Joystick toggled left
    return "Left";
  } 
}