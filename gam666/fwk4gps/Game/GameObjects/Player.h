#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "..\GameObject.h"
#include "..\PhysicsObject.h"

class World;

class Player : public GameObject {

public:
  Player(World* world);
};

#endif