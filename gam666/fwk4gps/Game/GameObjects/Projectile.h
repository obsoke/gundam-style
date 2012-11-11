#ifndef _PROJECTILE_H_
#define _PROJECTILE_H_

#include "..\GameObject.h"
#include "Weapon.h"
#include "Player.h"

class Weapon;

class Projectile : public GameObject {
public:
  Player* owner;
  Vector projectileDirection;
  float projectileSpeed;
  int projectileDamage;
  float projectileForce;
  float projectileLife;

  Projectile(World* world, Player* o, float pSpeed);
  void shootProjectile();
  void update();
  bool isOwner(GameObject* owner) { return false; };
  bool isOwner(Player* player) { return player == owner; };
  void onCollision(GameObject* other);
  virtual void onCollision(Player* other);
  ~Projectile();
};

#endif