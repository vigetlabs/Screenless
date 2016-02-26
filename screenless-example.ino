// Built with Screenless https://github.com/vigetlabs/Screenless/wiki/ - Free to use and modify
// Made at Viget Labs www.viget.com

#include "ScreenlessLed.h"
#include "ScreenlessButton.h"
#include "ScreenlessBuzzer.h"

ScreenlessLed screenlessLed;
ScreenlessButton  screenlessButton;
ScreenlessBuzzer  screenlessBuzzer;

SYSTEM_MODE(AUTOMATIC);

void setup() {
    screenlessLed.initialize();
    screenlessButton.initialize();
    bootUp();
}

void loop() {
    determineState();
    displayState();
}

void determineState() {
  screenlessButton.updateState();
  screenlessLed.updateState();
  screenlessBuzzer.updateState();

  if (screenlessButton.wasReleased(0) || screenlessButton.wasReleased(1) || screenlessButton.wasReleased(2) || screenlessButton.wasReleased(3)) {
    Serial.println("A button was released");
  }
  if (screenlessButton.wasHeld(0) || screenlessButton.wasHeld(1) || screenlessButton.wasHeld(2) || screenlessButton.wasHeld(3)) {
    Serial.println("A button was held");
  }
  if (screenlessButton.hasSequence("123")) { Serial.println("Fade"); screenlessLed.createBreath(2000,8000,VORANGE,VBLUE); }
  if (screenlessButton.hasSequence("1122")) { Serial.println("Breath"); screenlessLed.createBreath(2000,8000,VORANGE,VBLUE); }
  if (screenlessButton.hasSequence("2233")) { Serial.println("Runner"); screenlessLed.createRunner(VORANGE,1000,4000,CLOCKWISE); }
  if (screenlessButton.hasSequence("3300")) { Serial.println("Quad"); int newQuad[] {1,1,0,0}; screenlessLed.createQuad(newQuad,6000);}
  if (screenlessButton.hasSequence("3333")) { Serial.println("Song & Runner"); screenlessBuzzer.createSong("E5,8,G5,8,E6,8,C6,4,D6,4,G6,4"); screenlessLed.createRunner(BLUE,800,1600,CLOCKWISE); }
}

void displayState() {
  screenlessLed.show();
  screenlessBuzzer.show();
}

void bootUp() {
  Serial.begin(9600);
  Serial.println("Boot up");
}
