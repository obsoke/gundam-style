#ifndef _PROJECTILE_H_
#define _PROJECTILE_H_

#include "..\GameObject.h"
#include "Weapon.h"
#include "Player.h"

class Weapon;

class Projectile : public GameObject {
public:
  Player* owner;
  Vector direction;
  float pSpeed;
  int damage;
  float force;
  float life;
  unsigned time;

  Projectile(World* world, Player* owner = nullptr, 
    iGraphic* moddel = nullptr, float speed = 0);
  ~Projectile();
  void shoot();
  void update();
  bool isOwner(GameObject* owner) { return false; };
  bool isOwner(Player* player) { return player == owner; };
  void initializeFromOwner();
  void onCollision(GameObject* other);
  virtual void onCollision(Player* other);
  int hitBoundary() { life /= 1.1f; return 1; };
};

#endif