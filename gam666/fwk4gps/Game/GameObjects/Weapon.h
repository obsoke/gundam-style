#ifndef _WEAPON_H_
#define _WEAPON_H_
#include "../GameObject.h"
#include "Projectile.h"
#include <time.h>
#include "Player.h"

class Player;

class Weapon {
  //Projectile* projectile;
public:
  //Weapon Attributes
  int cooldownDuration;
  int maxHeat;
  int heatPerShot;
  int cooldownLeft;
  int currentHeat;
  float refireRate;
  clock_t cooldownTimer;
  Player* owner;

  //Projectile Attributes
  int projectileDamage;
  float projectileForce;
  float projectileSpeed;
  float projectileLife;
  std::vector<Projectile> projectiles;

  Weapon(int cdDuration, int mHeat, int hPerShot);
  void FireProjectile();
  bool CheckHeat();
  bool CheckCoolDown();
  ~Weapon();
};

#endif