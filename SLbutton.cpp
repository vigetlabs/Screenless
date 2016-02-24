#include "SLbutton.h"

SLbutton::SLbutton() {}

void SLbutton::initialize() {
  std::fill_n(previousstate, NUMBUTTONS, 1); //set the default state for all buttons manually.
  _enableInputs();
}

void SLbutton::updateState() {
  //check all of the buttons
  static long lasttime;
  int index;

  //loop through all buttons and update states
  for (index = 0; index < NUMBUTTONS; index++) {
      justreleased[index] = 0;
      pressed[index] = 0;
      justpressed[index] = 0;

      currentstate[index] = digitalRead(buttonPins[index]);

    if (currentstate[index] == LOW && previousstate[index] == HIGH) {
      //just pressed
      justpressed[index] = 1;
      justPressedTime[index] = millis();
      _lastEngagementTime = millis();
      _addPressToHistory(buttons[index]);
    }
    if (currentstate[index] == LOW && previousstate[index] == LOW) {
      //pressed
      pressed[index] = 1;
      pressedTime[index] = millis();
      _lastEngagementTime = millis();
    }
    if (currentstate[index] == HIGH && previousstate[index] == LOW && (_lastEngagementTime + DEBOUNCE) > millis()) { //quick and dirty debouncing
      //just released
      justreleased[index] = 1;
      justReleasedTime[index] = millis();
      _lastEngagementTime = millis();
    }
    previousstate[index] = currentstate[index];
  }
}

bool SLbutton::wasReleased(int button) {
  if (justreleased[button] == 1) {
    return 1;
  } else {
    return 0;
  }
}

bool SLbutton::wasHeld(int button) {
  if (pressed[button] && (pressedTime[button] - justPressedTime[button]) >= SHORTHOLD) {
    justPressedTime[button] = millis();     //clear the hold
    return 1;
  } else {
    return 0;
  }
}

bool SLbutton::hasSequence(String sequence) {
  if (_checkHistory(sequence)) { //if history has this sequence respond with 1
    _clearHistory(); //also clear the history because the sequence has been matched
    return 1;
  } else {
    return 0;
  }
}

long SLbutton::getLastEngagementTime() {
  return _lastEngagementTime;
}

//PRIVATE ///////////////////////////////////////////////////////////

bool SLbutton::_checkHistory(String sequence) {
  if (_justPressedHistoryString.indexOf(sequence) != -1) {
    return 1;
  } else {
    return 0;
  }
}

void SLbutton::_clearHistory(){
  _justPressedHistoryString = ""; // clear the string
  std::fill_n(justPressedHistory, 8, 0); // clear the array
}

void SLbutton::_addPressToHistory(int button) {
  int i;
  _justPressedHistoryString = ""; //empty the string to rebuild it from scratch
  //Clear the history after some time
  if ((_lastButtonPress + SEQUENCEWAIT) < millis()) {
    _clearHistory();
  }
  //move everything in the history array over one
  for (i = 0; i < 7; i++) {
    justPressedHistory[i] = justPressedHistory[i + 1];
    _justPressedHistoryString = _justPressedHistoryString + justPressedHistory[i];
  }
  //and add the most recent button press
  justPressedHistory[7] = button;
  _justPressedHistoryString = _justPressedHistoryString + String(button);
  _lastButtonPress = millis();
}

void SLbutton::_enableInputs() {
  int i;
  for (i=0; i< NUMBUTTONS; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
  }
}
