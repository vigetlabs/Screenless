#ifndef ScreenlessLed_h
#define ScreenlessLed_h

#include "neopixel.h"
#include "math.h"

#define PIXEL_PIN     D3
#define PIXEL_COUNT   11
#define PIXEL_OFFSET  8
#define PIXEL_TYPE    WS2812B
#define BLANK         0x000000
#define WHITE         0x555555
#define RED           0xFF0000
#define GREEN         0x00FF00
#define YELLOW        0xe3773e //need to get real yellow
#define BLUE          0x0000FF
#define RED_GREEN     0xFFFF00
#define RED_BLUE      0xFF00FF
#define GREEN_BLUE    0x00FFFF
#define VBLUE         0x3e96b7
#define VORANGE       0xe3773e
#define BRIGHTNESS    25
#define CLOCKWISE     true
#define COUNTERCLOCK  false


//led effect types
#define NONE          0
#define BREATH        1
#define FADE          2
#define RUNNER        3
#define QUAD          4

class ScreenlessLed
{
  public:
    ScreenlessLed();

    void
      initialize(),
      updateState(),
      show(),
      setAllNow(long color),
      setAll(long color),
      setAllGRB(int red, int green, int blue),
      createBreath(int cycleLength, int duration, uint32_t sColor, uint32_t fColor, uint32_t fadeToColor = BLANK), //fade between two colors
      createFade(int duration, uint32_t sColor, uint32_t fColor),
      createRunner(uint32_t color, int speed, int duration, bool direction),
      setOneRGB(int led, int red, int green, int blue),
      setOne(int led, long color),
      createQuad(int data[4], int pace);


  private:
    void
      _determineBreathState(),
      _determineFadeState(),
      _determineRunnerState(),
      _determineQuadState();
    int
      _queueCount         = 0,
      _currentEffect      = 0,
      _led2Quad(int quadArrayIndex, int ledPosition);
    long
      _scrollStart        = 0,
      _currentTime        = 0;
    bool
      _activelyScrolling  = false,
      _shouldRedraw       = false;

    //effect variables
    int         _targetDuration;
    int         _targetStartTime;
    uint32_t    _targetSColor;
    uint32_t    _targetFColor;
    bool        _targetDirection = true;
    int         _targetData[4] {0,0,0,0};

    //breath variables
    int         _targetCycleLength;
    uint32_t    _targetFadeToColor;

    //fade variables

    //runner variables
    int       _targetSpeed;

    //quad variables
    int currentQuadStep = 0;
    int _targetSteps = 0;

};

#endif
