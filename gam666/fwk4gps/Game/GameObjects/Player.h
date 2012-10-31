#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "..\GameObject.h"
#include "..\PhysicsObject.h"
#include "..\InputComponent.h"

class World;

class Player : public GameObject {
  InputComponent input;
public:
  Player(World* world);
  void update();
};

#endif