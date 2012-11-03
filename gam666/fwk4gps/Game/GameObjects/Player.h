#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "..\GameObject.h"
#include "..\PhysicsObject.h"
#include "InputComponent.h"

class World;
class iCamera;

class Player : public GameObject {
  InputComponent input;
  iCamera* camera;
  Vector cameraDistance;
public:
  int thrusterCooldown;
  int thruster;

  Player(World* world);
  ~Player();
  void update();
  void useThruster(int amount);
  void recoverThrusters();
  void createCamera();
  iCamera* getCamera() { return camera; };
};

#endif