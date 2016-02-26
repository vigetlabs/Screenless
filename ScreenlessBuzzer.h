#ifndef ScreenlessBuzzer_h
#define ScreenlessBuzzer_h

#include "application.h"
#include "math.h"

#define BUZZERPIN     D0

//led effect types
#define NONE          0
#define SONG          1
#define PLAY          2

class ScreenlessBuzzer
{
  public:
    ScreenlessBuzzer();

    void
      initialize(),
      updateState(),
      show(),
      createSong(String song = "C4");


  private:
    void
      _determineSongState();
    String
      _getCurrentNote();
    int
      _queueCount             = 0,
      _currentSoundEffect     = 0,
      _noteToTone(String note),
      _getSongLengthMS(String song);           //returns in ms
    long
      _scrollStart        = 0,
      _currentTime        = 0;
    bool
      _activelyScrolling  = false,
      _shouldRedraw       = false;

    //song variables
    bool _songCurrentlyPlaying   = false;
    int _songStartTime    = 0;
    int _currentTone      = 0;
    String _songString    = "C4";
    int _songBPM          = 60;
    String _currentNote   = "C4";
    char _inputStr[200];
    int _songLength       = 0; //in ms

};

#endif
