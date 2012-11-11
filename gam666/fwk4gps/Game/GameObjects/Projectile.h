#ifndef _PROJECTILE_H_
#define _PROJECTILE_H_

#include "..\GameObject.h"
#include "Weapon.h"
#include "Player.h"

class Weapon;

class Projectile : public GameObject {
public:
	Player* owner;
	Weapon* weapon;
	Vector projectileDirection;
	float projectileSpeed;
    int projectileDamage;
    float projectileForce;
    float projectileLife;

	Projectile::Projectile(World* world, Player* o, float pSpeed);
	void ShootProjectile();
	void OnHit();
	void update();
	~Projectile();
};

#endif