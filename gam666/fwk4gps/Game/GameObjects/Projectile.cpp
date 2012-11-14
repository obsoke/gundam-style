#include "Projectile.h"
#include "..\World.h"
#include "../../MathDef.h"

Projectile::Projectile(World* world, Player* owner, iGraphic* model, float pSpeed) : 
    GameObject(world, model), owner(owner), pSpeed(pSpeed), damage(10),
    time(0), force(1000), life(5000) {
	if (owner) initializeFromOwner();
}

void Projectile::shoot()
{
	world->projectiles.push_back(this);
	world->add(this);
}

void Projectile::onCollision(Player* other)
{
  debug("PLAYER HIT\n");
	//override for special hit effects here
	world->remove(this);
}

void Projectile::onCollision(GameObject* other)
{
	//override for special hit effects here
	world->remove(this);
}

void Projectile::update() {
  ++time;
  if (time >= life) {
    world->remove(this);
  } else {
    speed = pSpeed * direction;
    GameObject::update();
  }
}

void Projectile::initializeFromOwner() {
	direction = owner->orientation('z');
	setMatrix(owner->rotation());
  setTranslation(owner->position());
}

Projectile::~Projectile() {

}