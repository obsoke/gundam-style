#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "GameObject.h"

class Player : public GameObject {

public:
  Player() : GameObject() { };
  void hitBoundary();  //implementation of GameObject virtual function
};

#endif