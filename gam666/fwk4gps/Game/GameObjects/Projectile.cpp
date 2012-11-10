#include "Projectile.h"
#include "..\World.h"

Projectile::Projectile(Weapon* o) : GameObject(world)
{
	owner = o;
	Vector pVector(0.0, 0.0, 1.0);
	projectileVector = owner->projectileSpeed * projectileVector;
}

void Projectile::ShootProjectile()
{
	world->projectiles.push_back(this);
	world->add(this);
}

void Projectile::OnHit()
{
	//override for special hit effects here
	world->remove(this);
}

Projectile::~Projectile()
{

}