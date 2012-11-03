#ifndef _WORLD_H_
#define _WORLD_H_

#include <vector>
#include "..\Coordinator.h"
#include "GameObject.h"
#include "PhysicsWorld.h"

class Game;
class Player;
class Floor;
struct Viewport;

class World : public Coordinator {
  iLight* defaultLight;
  int numberOfPlayers;

  void initializeLighting();
  void initializeObjects();
  void initializeHUD();
  void initializeFloors();
public:
  std::vector<GameObject*> gameObjects;
  std::vector<Floor*> floors;
  std::vector<Player*> players;
  Game* game;
  PhysicsWorld physics;

  World(Game* game);
  ~World();
  void add(GameObject* gameObject);
  void remove(GameObject* gameObject);
  void add(iObject* object) { Coordinator::add(object); };
  void remove(iObject* object) { Coordinator::remove(object); };
  virtual void updateWorld();
  void render();
  void addFloor(const Vector& position, const Vector& tiles = Vector(1, 1, 1), 
    const Vector& tileSize = Vector(100, 10, 100), iTexture* tex = nullptr);
  Viewport calcViewport(int player);
  void createProjection();
  unsigned int getNow() { return now; };
  unsigned int getDelta() { return lastUpdate; };
};

#endif