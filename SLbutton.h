#ifndef SLbutton_h
#define SLbutton_h

#include "application.h"

#define NUMBUTTONS 4 //also update buttonPins[NUMBUTTONS] below
#define DEBOUNCE 20 //quick and dirty debouncing
#define SHORTHOLD 1000
#define SEQUENCEWAIT 1000 //how long to wait before before presses before clearing the button press history

class SLbutton
{
  public:
    SLbutton();

    void
      initialize(),
      updateState();
    bool
      hasSequence(String sequence),
      wasReleased(int button),
      wasHeld(int button);
    int
      justPressedTime[NUMBUTTONS], //time of press in ms
      pressedTime[NUMBUTTONS], //time of press in ms
      justReleasedTime[NUMBUTTONS]; // time of release in ms
    long
      getLastEngagementTime();
    byte
        buttonPins[NUMBUTTONS] = {4, 5, 6, 7}, //which pins? Also, adjust define NUMBUTTONS above
        buttons[NUMBUTTONS] = {0, 1, 2, 3}, //the name (aka) of the buttons
        justpressed[NUMBUTTONS],
        pressed[NUMBUTTONS],
        justreleased[NUMBUTTONS],
        previousstate[NUMBUTTONS],
        currentstate[NUMBUTTONS],
        justPressedHistory[8];

  private:
    void
      _enableInputs(),
      _addPressToHistory(int button),
      _clearHistory();
    int
      _currentState       = 0, //0 = no clicks, 1 = one click, 2 = double click, 3 = click and hold
      _lastButtonPress    = 0;
    long
      _scrollStart        = 0,
      _currentTime        = 0,
      _lastEngagementTime = 0;
    bool
      _checkHistory(String sequence),
      _activelyScrolling  = false,
      _shouldRedraw       = false;
    String
      _justPressedHistoryString;
};

#endif
