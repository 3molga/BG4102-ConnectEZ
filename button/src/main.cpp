#include <Arduino.h>
#include <ezButton.h>

#define BUZZER_PIN 12
ezButton button(5);  // create ezButton object that attach to pin 7;
int lastState = HIGH; 
int currentState = LOW; 

void setup() {
  Serial.begin(9600);
  button.setDebounceTime(50); // set debounce time to 50 milliseconds
  pinMode(BUZZER_PIN, OUTPUT);
}

void loop() {
  button.loop(); // MUST call the loop() function first
  digitalWrite(BUZZER_PIN, lastState);

  int btnState = button.getState();
  Serial.println(btnState);

  if(button.isPressed()){
    Serial.println("The button is pressed");
    currentState = LOW;
    lastState = LOW;
    // digitalWrite(BUZZER_PIN, lastState);
  }
  
  digitalWrite(BUZZER_PIN, currentState);
  delay(10);

  // if(button.isReleased())
  //   Serial.println("The button is released");
}