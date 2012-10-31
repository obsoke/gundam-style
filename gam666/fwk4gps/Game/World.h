#ifndef _WORLD_H_
#define _WORLD_H_

#include <vector>
#include "..\Coordinator.h"
#include "GameObject.h"
#include "Player.h"

class Game;

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