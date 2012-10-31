#ifndef _WORLD_H_
#define _WORLD_H_

#include <vector>
#include "..\Coordinator.h"
#include "GameObject.h"
#include "Player.h"

class Game;

//random values to create the boundaries of the environment in a cube
#define WORLDX1 -290
#define WORLDY1 -300
#define WORLDZ1 -200
#define WORLDX2 320
#define WORLDY2 300
#define WORLDZ2 600


class World : public Coordinator {
  iLight* defaultLight;

  void initializeLighting();
  void initializeObjects();
  void initializeHUD();
public:
  std::vector<GameObject*> gameObjects;
  Game* game;
  Player* player;

  World(Game* game);
  ~World();
  void add(GameObject* gameObject);
  void remove(GameObject* gameObject);
  void update();
};

#endif