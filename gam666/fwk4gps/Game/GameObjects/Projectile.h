#ifndef _PROJECTILE_H_
#define _PROJECTILE_H_

#include "..\GameObject.h"
#include "Weapon.h"

class Weapon;

class Projectile : public GameObject {
public:
	Weapon* owner;
	Vector projectileVector;

	Projectile(Weapon* o);
	void ShootProjectile();
	void OnHit();
	~Projectile();
};

#endif