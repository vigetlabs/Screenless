#include "SLLED.h"

Adafruit_NeoPixel leds = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, PIXEL_TYPE);

SLLED::SLLED() {}

void SLLED::initialize() {
  leds.begin();
  leds.setBrightness(BRIGHTNESS);
}

void SLLED::updateState() {
  //determine what the current state (what colors go where). Called every loop.
  //check if there is a breath, if so, ask it what to do.
  if (_currentEffect == BREATH) { _determineBreathState(); }
  if (_currentEffect == FADE) { _determineFadeState(); }
  if (_currentEffect == RUNNER) { _determineRunnerState(); }
  if (_currentEffect == QUAD) { _determineQuadState(); }
}

void SLLED::show() {
  //show the state which was determined in updateState
  leds.show();
}

void SLLED::setAllNow(long color) {
  setAll(color);
  leds.show();
}

void SLLED::setAll(long color){
  for (int i=0; i<PIXEL_COUNT; i++) {
    leds.setPixelColor(i, color);
  }
}

void SLLED::setAllGRB(int red, int green, int blue) {
  for (int i=0; i<PIXEL_COUNT; i++) {
    leds.setPixelColor(i, red, green, blue);
  }
}

void SLLED::setOne(int led, long color) {
    leds.setPixelColor(led, color);
}

void SLLED::setOneRGB(int led, int red, int green, int blue) {
    leds.setPixelColor(led, red, green, blue);
}

// CREATE ----------------

void SLLED::createBreath(int cycleLength, int duration, uint32_t sColor, uint32_t fColor, uint32_t fadeToColor) {
  _currentEffect = BREATH;
  _targetCycleLength = cycleLength; //seconds per cycle of all LEDs
  _targetDuration = duration;
  _targetStartTime = millis();
  _targetSColor = sColor;
  _targetFColor = fColor;
  _targetFadeToColor = fadeToColor;
  _targetDirection = true;
}

void SLLED::createFade(int duration, uint32_t sColor, uint32_t fColor) {
  _currentEffect = FADE;
  _targetDuration = duration;
  _targetStartTime = millis();
  _targetSColor = sColor;
  _targetFColor = fColor;
}

void SLLED::createRunner(uint32_t color, int speed, int duration, bool direction) {
  _currentEffect = RUNNER;
  _targetSpeed = speed; //ms
  _targetDuration = duration; //ms
  _targetStartTime = millis();
  _targetSColor = color;
  _targetDirection = direction;
}

void SLLED::createQuad(int data[4], int pace) {
  _currentEffect = QUAD;
  _targetSteps = pace;
  //Handle Rotation (power cord out bottom)
  // normal: 0,1,2,3
  // clock: 2,3,0,1
  // cclock: 1,0,3,2

  int rotation[] {2,3,0,1}; //clockwise
  int tempQuad[] {0,0,0,0};
  tempQuad[rotation[0]] = data[0];
  tempQuad[rotation[1]] = data[1];
  tempQuad[rotation[2]] = data[2];
  tempQuad[rotation[3]] = data[3];

  //Assign to _targetData
  for (int i = 0; i < 4; i++) {
    _targetData[i] = tempQuad[i];
  }

}

//PRIVATE ----------------

void SLLED::_determineBreathState() {
  // is breath over? if so, remove the current effect
  if((_targetStartTime + _targetDuration) < millis()) {
    _currentEffect = NONE;
    createFade(1000, leds.getPixelColor(1), _targetFadeToColor); //get the color a current pixel and begin fading with this.
  } else {
    // if the breath is NOT over, figure out what to do
    int numOfCycles = _targetDuration / _targetCycleLength;
    float timePassed = millis() - _targetStartTime;
    float percentDone = timePassed / _targetDuration;
    float currentCycle = (percentDone*100)/(100/numOfCycles);
    float cyclePercentDone = fmod(currentCycle,1);
    static float previousCyclePercentDone;

    if (cyclePercentDone < previousCyclePercentDone) {
      //reverse breath direction
      if (_targetDirection == true) {
        _targetDirection = false;
      } else {
        _targetDirection = true;
      }
    } else {
      // extract the rgb colors from the hex that was passed for start and finish
      uint8_t Rstart = (uint8_t)(_targetSColor >> 16);
      uint8_t Gstart = (uint8_t)(_targetSColor >>  8);
      uint8_t Bstart = (uint8_t)_targetSColor;
      uint8_t Rfinish = (uint8_t)(_targetFColor >> 16);
      uint8_t Gfinish = (uint8_t)(_targetFColor >>  8);
      uint8_t Bfinish = (uint8_t)_targetFColor;
      uint8_t Rnew;
      uint8_t Gnew;
      uint8_t Bnew;
      if (_targetDirection) {
        Rnew = Rstart + (Rfinish - Rstart) * cyclePercentDone;
        Gnew = Gstart + (Gfinish - Gstart) * cyclePercentDone;
        Bnew = Bstart + (Bfinish - Bstart) * cyclePercentDone;
      } else {
        Rnew = Rfinish + (Rstart - Rfinish) * cyclePercentDone;
        Gnew = Gfinish + (Gstart - Gfinish) * cyclePercentDone;
        Bnew = Bfinish + (Bstart - Bfinish) * cyclePercentDone;
      }
      setAllGRB(Rnew, Gnew, Bnew);
    }
    previousCyclePercentDone = cyclePercentDone;
  }
}

void SLLED::_determineFadeState() {
  if((_targetStartTime + _targetDuration) < millis()) {
    _currentEffect = NONE;
  } else {
    float timePassed = millis() - _targetStartTime;
    float percentDone = timePassed / _targetDuration;

    uint8_t Rstart = (uint8_t)(_targetSColor >> 16);
    uint8_t Gstart = (uint8_t)(_targetSColor >>  8);
    uint8_t Bstart = (uint8_t)_targetSColor;
    uint8_t Rfinish = (uint8_t)(_targetFColor >> 16);
    uint8_t Gfinish = (uint8_t)(_targetFColor >>  8);
    uint8_t Bfinish = (uint8_t)_targetFColor;
    uint8_t Rnew;
    uint8_t Gnew;
    uint8_t Bnew;

    Rnew = Rstart + (Rfinish - Rstart) * percentDone;
    Gnew = Gstart + (Gfinish - Gstart) * percentDone;
    Bnew = Bstart + (Bfinish - Bstart) * percentDone;

    setAllGRB(Rnew, Gnew, Bnew);
  }
}

void SLLED::_determineRunnerState() {
  if((_targetStartTime + _targetDuration) < millis()) {
    _currentEffect = NONE;
    setAll(0x000000);
  } else {
    float timePassed = millis() - _targetStartTime;
    int timePerLed = _targetSpeed / PIXEL_COUNT; //90 ms
    int currentPosition = timePassed / timePerLed;
    uint32_t currentColor = _targetSColor;

    //make adjustments for positions beyond the pixel count;
    if (currentPosition >= PIXEL_COUNT) {
      int countRevolution = floor(currentPosition / PIXEL_COUNT); //we want to keep the numerator and not round.
      currentPosition = currentPosition - (PIXEL_COUNT*countRevolution);
    }
    if (_targetDirection == COUNTERCLOCK) {
      currentPosition = map(currentPosition, 0, PIXEL_COUNT-1, PIXEL_COUNT-1, 0); //subtract one from pixel count because the first pixel is zero (0) not 1
    }
    for (int i=0; i<PIXEL_COUNT; i++) {
      if (i == currentPosition) { currentColor = _targetSColor; } else { currentColor = 0x000000;} //todo: add runner tail
      leds.setPixelColor(i, currentColor);
    }

  }
}

void SLLED::_determineQuadState() {
  long color;
  int led1 = 0;
  int led2 = 1;

  for (int i=0; i<4; i++) { //loop through four items in the data array
    led1 = _led2Quad(i,0);
    led2 = _led2Quad(i,1);
    switch (_targetData[i]) {
        case 0:
          color = BLANK;
        break;
        case 1:
          color = GREEN;
        break;
        case 2:
          color = RED;
        break;
        case 3:
          color = YELLOW;
        break;
        case 4:
          color = RED_BLUE;
        break;
    }
    setOne(led1, color);
    setOne(led2, color);
  }
  // set blank pixels
  if (PIXEL_COUNT >= 11) {
    setOne(2, 0x000000);
    setOne(5, 0x000000);
    setOne(8, 0x000000);
  }
  if (PIXEL_COUNT == 12) {
    setOne(12, 0x000000);
  }
}

int SLLED::_led2Quad(int quadArrayIndex, int ledPosition){
  switch (quadArrayIndex) {
      case 0:
        return 0 + ledPosition;
      break;
      case 1:
        return 3 + ledPosition;
      break;
      case 2:
        return 6 + ledPosition;
      break;
      case 3:
        return 9 + ledPosition;
      break;
  }
}
