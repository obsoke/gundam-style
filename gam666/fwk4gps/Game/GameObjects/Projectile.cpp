#include "Projectile.h"
#include "..\World.h"
#include "../../MathDef.h"

Projectile::Projectile(World* world, Player* o, float pSpeed) : GameObject(world)
{
	owner = o;
	
	projectileDirection = o->orientation('z');
	projectileSpeed = pSpeed;
}

void Projectile::ShootProjectile()
{
	world->projectiles.push_back(this);
	world->add(this);
}

void Projectile::onCollision(Player* other)
{
  debug("PLAYER HIT");
	//override for special hit effects here
	world->remove(this);
}

void Projectile::onCollision(GameObject* other)
{
	//override for special hit effects here
	world->remove(this);
}

void Projectile::update() {
  speed = projectileSpeed * projectileDirection;
  GameObject::update();
}

Projectile::~Projectile()
{

}