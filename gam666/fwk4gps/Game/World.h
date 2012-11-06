#ifndef _WORLD_H_
#define _WORLD_H_

#include <vector>
#include "..\Coordinator.h"
#include "GameObject.h"
#include "PhysicsWorld.h"

class Game;
class Player;
class Floor;

//random values to create the boundaries of the environment in a cube
#define WORLDX1 -100
#define WORLDY1 -500
#define WORLDZ1 -200
#define WORLDX2 200
#define WORLDY2 5000
#define WORLDZ2 2500

class World : public Coordinator {
  iLight* defaultLight;

  void initializeLighting();
  void initializeObjects();
  void initializeHUD();
  void initializeFloors();
public:
  std::vector<GameObject*> gameObjects;
  std::vector<Floor*> floors;
  Game* game;
  Player* player;
  PhysicsWorld physics;

  World(Game* game);
  ~World();
  void add(GameObject* gameObject);
  void remove(GameObject* gameObject);
  void add(iObject* object) { Coordinator::add(object); };
  void remove(iObject* object) { Coordinator::remove(object); };
  void update();
  void addFloor(const Vector& position, const Vector& tiles = Vector(1, 1, 1), 
    const Vector& tileSize = Vector(100, 10, 100), iTexture* tex = nullptr);
  unsigned int getNow() { return now; };
  unsigned int getDelta() { return lastUpdate; };
};

#endif