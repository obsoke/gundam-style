#ifndef _WORLD_H_
#define _WORLD_H_

#include <vector>
#include "..\Coordinator.h"
#include "GameObject.h"
#include "PhysicsWorld.h"
#include "Map.h"

class Game;
class Player;
class Projectile;
class Floor;
struct Viewport;

class World : public Coordinator {
	iObject* sprite; 
  iLight* defaultLight;
  iText* testText;
  iSound* music;
  int numberOfPlayers;
  AABB boundary;
  Map& map;
  void initializeLighting();
  void initializeObjects();
  void initializeHUD();
  void initializeMusic();
public:
  std::vector<GameObject*> gameObjects;
  std::vector<Floor*> floors;
  std::vector<Player*> players;
  std::vector<Projectile*> projectiles;
  std::vector<iObject*> sprites;
  std::vector<Vector> spawnPoints;

  std::vector<iText*> healths;
  std::vector<iText*> thrusters;
  std::vector<iText*> scores;
  std::vector<bool> cooldown;
  std::vector<iText*> respawnTimers;
  std::vector<bool> respawning;

  Frame* skybox;
  Game* game;
  PhysicsWorld* physics;

  World(Game* game, Map& map);
  ~World();
  void initialize();
  void loadingScreen();
  void add(GameObject* gameObject);
  void remove(GameObject* gameObject);
  void add(iObject* object) { Coordinator::add(object); };
  void remove(iObject* object) { Coordinator::remove(object); };
  void remove(Projectile* projectile);
  virtual void updateWorld();
  void updateHUD();
  void render();
  void addFloor(const Vector& position, const Vector& tiles = Vector(1, 1, 1), 
    const Vector& tileSize = Vector(100, 10, 100), iTexture* tex = nullptr);
  Viewport calcViewport(int player);
  void createProjection();
  unsigned int getNow() { return now; };
  unsigned int getLast() { return lastUpdate; };
  unsigned int getDelta() { return now - lastUpdate; };
  iObject* CreateSprite(const wchar_t* file, const Vector& position = Vector(0,0,0), 
    float scale = 1, unsigned char a = '\xFF');
  template<class T> 
    void checkProjectileCollision(const std::vector<T*>& objects);
  void checkBoundaryCollision();
  void setBoundary(AABB b) { boundary = b; }
  const AABB& getBoundary() { return boundary; }
  bool collidesWithFloors(const AABB& aabb);
  iText* createHUDText(const Viewport& viewport, float x, float y, const char* label, 
    unsigned colour = 0xFF2266FF, unsigned size = 22, bool centered = false);
};

#endif