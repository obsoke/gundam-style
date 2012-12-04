#ifndef _WEAPON_H_
#define _WEAPON_H_
#include "../GameObject.h"
#include "Projectile.h"
#include <time.h>
#include "Player.h"
#include "GameTimer.h"

class Player;

class Weapon {
public:
  //Weapon Attributes
  int maxHeat;
  int heatPerShot;
  int currentHeat;
  Player* owner;
  
  GameTimer cooldownTimer;
  GameTimer refireTimer;

  Weapon(Player* o, float cdDuration, int mHeat, int hPerShot);
  void fireProjectile();
  bool checkOverHeat();
  void checkRefireTime();
  void checkCoolDown();
  ~Weapon();
};

#endif