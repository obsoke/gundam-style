#ifndef _WEAPON_H_
#define _WEAPON_H_
#include "../GameObject.h"
#include "Projectile.h"
#include <time.h>
#include "Player.h"

class Player;

class Weapon {
public:
  //Weapon Attributes
  int maxHeat;
  int heatPerShot;
  int currentHeat;
  Player* owner;
  
  double cooldownLeft;
  double cooldownDuration;
  bool coolingDown;
  clock_t cooldownTimer;
  
  double refireLeft;
  double refireDelay;
  bool pausingForRefire;
  clock_t refireTimer;

  //Projectile Attributes
  std::vector<Projectile*> projectiles;

  Weapon(Player* o, int cdDuration, int mHeat, int hPerShot);
  void fireProjectile();
  bool checkOverHeat();
  void checkRefireTime();
  void checkCoolDown();
  ~Weapon();
};

#endif