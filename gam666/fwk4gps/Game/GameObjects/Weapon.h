#ifndef _WEAPON_H_
#define _WEAPON_H_
#include "../GameObject.h"
#include "Projectile.h"
#include <time.h>
#include "Player.h"
#include "GameTimer.h"

class Player;
class iSound;

class Weapon {
	iSound* fireSound;
public:
  //Weapon Attributes
  int maxHeat;
  int heatPerShot;
  int currentHeat;
  Player* owner;
  
  GameTimer cooldownTimer;
  GameTimer refireTimer;

  Weapon(Player* o, float cdDuration, int mHeat, int hPerShot);
  virtual void fireProjectile();
  virtual bool checkOverHeat();
  ~Weapon();
};

#endif