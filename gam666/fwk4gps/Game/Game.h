#ifndef _GAME_H_
#define _GAME_H_

/* Design Definition - Modelling Layer
 *
 * Design.h
 * fwk4gps version 4.0
 * gam666/dps901/gam670/dps905
 * June 25 2012
 * copyright (c) 2012 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include "..\iCoordinator.h"

class Game {
    Game(const Game& s);            // prevents copying
    Game& operator=(const Game& s); // prevents assignment
    iCoordinator* currentState;
  public:
    void* handle;
    int show;

    Game(void* handle, int show);
    ~Game();
    void initialize();
    void update();
};

#endif
