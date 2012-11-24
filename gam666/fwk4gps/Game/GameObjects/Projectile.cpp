#include "Projectile.h"
#include "..\World.h"
#include "../../MathDef.h"
#include "../../Camera.h"

#define AIMASSISTDISTANCE 60

Projectile::Projectile(World* world, Player* owner, iGraphic* model, float pSpeed) : 
    GameObject(world, model), owner(owner), pSpeed(pSpeed), damage(10),
    time(0), force(1000), life(5000), target(nullptr), isHoming(false) {
	if (owner) initializeFromOwner();
}

void Projectile::shoot()
{
	world->projectiles.push_back(this);
	world->add(this);
}

void Projectile::onCollision(Player* other)
{
  //debug("PLAYER HIT\n");
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

void Projectile::findTarget() {
  Camera* ownerCamera = (Camera*)owner->getCamera();
  float minDistance = 9999999.0f;
  for(unsigned i = 0; i < world->players.size(); i++) {
    Vector playerWorldPosition = world->players[i]->position();
    if(world->players[i] != owner) {
      float dotPlayerOwner = dot(playerWorldPosition - owner->position(), ownerCamera->orientation('z'));
      float normPlayerOwner = sqrt(dot(playerWorldPosition - owner->position(), playerWorldPosition - owner->position()));
      float dist = sqrt(normPlayerOwner * normPlayerOwner - dotPlayerOwner * dotPlayerOwner);

      if(((dist < AIMASSISTDISTANCE) && (dotPlayerOwner > 0)) || isHoming ) {
        if(dist < minDistance) {
          minDistance = dist;
          target = world->players[i];
        }
      }
    }
  }
}

void Projectile::initializeFromOwner() {
  findTarget();

  if(target) {
    direction = (target->position() - owner->position()) / sqrt(dot(target->position() - owner->position(), target->position() - owner->position()));
  } else {
	  direction = owner->orientation('z');
  }
	setMatrix(owner->rotation());
  setTranslation(owner->position());
}

Projectile::~Projectile() {

}