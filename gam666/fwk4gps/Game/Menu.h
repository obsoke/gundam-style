#ifndef _MENU_H_
#define _MENU_H_

#include <vector>
#include "..\Coordinator.h"
#include "GameObject.h"
#include "Map.h"

class Game;
class Player;
class Projectile;
class Floor;
struct Viewport;

class Menu : public Coordinator {
  iLight* defaultLight;
  int numberOfPlayers;
  AABB boundary;
  Map& map;
  void initializeLighting();
  void initializeObjects();
  void initializeHUD();  
public:
  std::vector<GameObject*> gameObjects;
  std::vector<Floor*> floors;
  std::vector<Player*> players;
  std::vector<Projectile*> projectiles;
  std::vector<iObject*> sprites;
  std::vector<Vector> spawnPoints;

  Frame* skybox;
  Game* game;
  //PhysicsWorld* physics;

  Menu(Game* game);  
  ~Menu();
  void initialize();
  void loadingScreen();
  void add(GameObject* gameObject);
  void remove(GameObject* gameObject);
  void add(iObject* object) { Coordinator::add(object); };
  void remove(iObject* object) { Coordinator::remove(object); };
  void remove(Projectile* projectile);
  virtual void updateWorld();
  void render();
  iObject* CreateSprite(const wchar_t* file, const Vector& position = Vector(0,0,0),
    unsigned char a = '\xFF');
  
};

#endif