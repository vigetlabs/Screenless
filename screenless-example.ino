// Built with Screenless https://github.com/vigetlabs/Screenless/wiki/ - Free to use and modify
// Made at Viget Labs www.viget.com

#include "SLLED.h"
#include "SLbutton.h"
#include "SLbuzzer.h"

SLLED ibleds;
SLbutton  ibbutton;
SLbuzzer  ibbuzzer;

SYSTEM_MODE(AUTOMATIC);

void setup() {
    ibleds.initialize();
    ibbutton.initialize();
    bootUp();
}

void loop() {
    determineState();
    displayState();
}

void determineState() {
  ibbutton.updateState();
  ibleds.updateState();
  ibbuzzer.updateState();

  if (ibbutton.wasReleased(0) || ibbutton.wasReleased(1) || ibbutton.wasReleased(2) || ibbutton.wasReleased(3)) {
    Serial.println("A button was released");
  }
  if (ibbutton.wasHeld(0) || ibbutton.wasHeld(1) || ibbutton.wasHeld(2) || ibbutton.wasHeld(3)) {
    Serial.println("A button was held");
  }
  if (ibbutton.hasSequence("1122")) { Serial.println("Breath"); ibleds.createBreath(2000,8000,VORANGE,VBLUE); }
  if (ibbutton.hasSequence("2233")) { Serial.println("Runner"); ibleds.createRunner(VORANGE,1000,4000,CLOCKWISE); }
  if (ibbutton.hasSequence("3344")) { Serial.println("Quad"); int newQuad[] {1,1,0,0}; ibleds.createQuad(newQuad,6000);}
  if (ibbutton.hasSequence("3333")) { Serial.println("Song & Runner"); ibbuzzer.createSong("E5,8,G5,8,E6,8,C6,4,D6,4,G6,4"); ibleds.createRunner(BLUE,800,1600,CLOCKWISE); }
}

void displayState() {
  ibleds.show();
  ibbuzzer.show();
}

void bootUp() {
  Serial.begin(9600);
  Serial.println("Boot up");
}
