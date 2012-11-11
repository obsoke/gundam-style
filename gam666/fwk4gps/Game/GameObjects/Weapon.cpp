#include "Weapon.h"
#include "../GameObject.h"
#include "../World.h"
#include "Projectile.h"
#include "../Utils.h"

Weapon::Weapon(Player* o, int cdDuration, int mHeat, int hPerShot)
{
  owner = o;
  cooldownDuration = cdDuration;
  maxHeat = mHeat;
  heatPerShot = hPerShot;
  cooldownLeft = 0;
  currentHeat = 0;
}

void Weapon::FireProjectile()
{
  //if(currentHeat < maxHeat && cooldownLeft <= 0){
  Projectile* proj = new Projectile(owner->getWorld(), owner, 10);
  projectiles.push_back(proj);
  proj->setMatrix(owner->getMatrix());
  proj->scale(0.5f, 0.5f, 0.5f);
  proj->ShootProjectile();
  currentHeat += maxHeat;

  if(!CheckHeat())
  {
    cooldownTimer = clock();
  }
  //}
}

bool Weapon::CheckCoolDown()
{
  if(cooldownDuration - cooldownTimer <= 0)
  {
    cooldownTimer = NULL;
  }

  return (cooldownDuration - cooldownTimer) <= 0;
}

bool Weapon::CheckHeat()
{
  return currentHeat <= maxHeat;
}

Weapon::~Weapon()
{

}