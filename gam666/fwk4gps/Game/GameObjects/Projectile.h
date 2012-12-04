#ifndef _PROJECTILE_H_
#define _PROJECTILE_H_

#include "..\GameObject.h"
#include "Weapon.h"
#include "Player.h"

class Weapon;

class Projectile : public GameObject {
  void findTarget();
  void homeOnTarget();
public:
  Player* owner;
  Vector direction;
  float pSpeed;
  int damage;
  float force;
  float life;
  unsigned time;
  float maxHomeAngle;
  Player* target;
  bool isHoming;

  Projectile(World* world, Player* owner = nullptr, 
    iGraphic* model = nullptr, float speed = 0, 
    bool iHoming = false, float mHomeAngle = 10.0f);
  ~Projectile();
  void shoot();
  void update();
  bool isOwner(GameObject* owner) { return false; };
  bool isOwner(Player* player) { return player == owner; };
  void initializeFromOwner();
  void onCollision(GameObject* other);
  virtual void onCollision(Player* other);
  void shiftDirection(float rad);
  int hitBoundary() { life /= 1.1f; return 1; };
};

#endif