#ifndef _WORLD_H_
#define _WORLD_H_

#include <vector>
#include "..\Coordinator.h"
#include "GameObject.h"
#include "PhysicsWorld.h"

class Game;
class Player;
class Floor;

class World : public Coordinator {
  iLight* defaultLight;

  void initializeLighting();
  void initializeObjects();
  void initializeHUD();
public:
  std::vector<GameObject*> gameObjects;
  Game* game;
  Player* player;
  Floor* floor;
  PhysicsWorld physics;

  World(Game* game);
  ~World();
  void add(GameObject* gameObject);
  void remove(GameObject* gameObject);
  void update();
  unsigned int getNow() { return now; };
  unsigned int getDelta() { return lastUpdate; };
};

#endif