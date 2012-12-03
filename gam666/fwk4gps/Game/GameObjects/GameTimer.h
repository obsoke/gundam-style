#ifndef GAMETIMER_H
#define GAMETIMER_H

#include <time.h>

class GameTimer {
  clock_t timer;
public:
  bool timerActive;
  float timeLeft;
  float timeLimit;

  GameTimer(float tLimit);
  float getTime();
  bool checkTimer();
  bool reset();
};

#endif