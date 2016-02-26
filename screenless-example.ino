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


  if (screenlessButton.wasReleased(0)) {
    screenlessLed.createFade(8000,VORANGE,VBLUE);
  }

  if (screenlessButton.wasReleased(1)) {
    screenlessLed.createBreath(2000,8000,VORANGE,VBLUE);
  }

  if (screenlessButton.wasReleased(2)) {
    screenlessLed.createRunner(VORANGE,1000,4000,CLOCKWISE);
  }

  if (screenlessButton.wasHeld(3)) {
    int newQuad[] {1,2,3,0};
    screenlessLed.createQuad(newQuad,6000);
    screenlessBuzzer.createSong("E5,8,G5,8,E6,8,C6,4,D6,4,G6,4");
  }

}

void displayState() {
  screenlessLed.show();
  screenlessBuzzer.show();
}

void bootUp() {
  Serial.begin(9600);
  Serial.println("Boot up");
}
