#include "Projectile.h"
#include "..\World.h"
#include "../../MathDef.h"
#include "../Utils.h"

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

void Projectile::OnHit()
{
	//override for special hit effects here
	world->remove(this);
}

void Projectile::update() {
  Matrix matrix;
  speed = projectileSpeed * projectileDirection;
  //const Vector& pos = position();
  //matrix.translate(pos.x, pos.y, pos.z);
  matrix.rotatex(projectileDirection.x);
  matrix.rotatey(projectileDirection.y);
  matrix.rotatez(projectileDirection.z);
  setMatrix(matrix);
  GameObject::update();
  debug(position());
}

Projectile::~Projectile()
{

}