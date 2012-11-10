#include "Weapon.h"
#include "../GameObject.h"
#include "../World.h"
#include "Projectile.h"

Weapon::Weapon(int cdDuration, int mHeat, int hPerShot)
{
  cooldownDuration = cdDuration;
  maxHeat = mHeat;
  heatPerShot = hPerShot;
  cooldownLeft = 0;
  currentHeat = 0;
	
  projectileDamage = 10;
  projectileForce = 10;
  projectileSpeed = 10;
  projectileLife = 30;
}

void Weapon::FireProjectile()
{
	if(currentHeat < maxHeat && cooldownLeft <= 0){
		Projectile newProjectile(this);

		projectiles.push_back(newProjectile);
		newProjectile.ShootProjectile();
		currentHeat += maxHeat;

		if(!CheckHeat())
		{
			cooldownTimer = clock();
		}
	}
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