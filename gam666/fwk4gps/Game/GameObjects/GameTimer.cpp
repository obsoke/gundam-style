#include "GameTimer.h"

#define CPS (float)CLOCKS_PER_SEC

GameTimer::GameTimer(float tLimit)
{
  timer = 0;
  timerActive = true;
  timeLeft = 0;
  timeLimit = tLimit;
}

float GameTimer::getTime()
{
  return (clock() - timer) / CPS;
}

bool GameTimer::checkTimer()
{
	if(((clock() - timer) / CPS >= timeLimit) && timerActive) {
		timer = 0;
		timeLeft = 0;
		timerActive = false;
	}
	else if(((clock() - timer) / CPS < timeLimit) && timerActive) {
		timeLeft = (clock() - timer) / CPS - timeLimit;

		if(timeLeft < 0)
			timeLeft = 0;
	}

  return timerActive;
}

bool GameTimer::reset()
{
  timerActive = true;
  timeLeft = timeLimit;
  timer = clock();

  return timerActive = true;
}