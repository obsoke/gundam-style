#ifndef _WORLD_H_
#define _WORLD_H_

#include <vector>
#include "..\Coordinator.h"
#include "GameObject.h"

class Game;

class World : public Coordinator {
  iLight* defaultLight;

  void initializeLighting();
  void initializeObjects();
public:
  std::vector<GameObject*> gameObjects;
  Game* game;
  
  World(Game* game);
  void add(GameObject* gameObject);
  void remove(GameObject* gameObject);
  void update();
};

#endif