#include "Player.h"
#include "..\World.h"
#include "..\PhysicsObject.h"
#include "..\..\Camera.h"
#include "Weapon.h"
#include "Projectile.h"

Player::Player(World* world, int id, iGraphic* graphic) : 
    GameObject(world, graphic), thruster(300), id(id), 
    thrusterCooldown(0), health(300), cameraDistance(Vector(0, 40, -100)) { 
  createCamera();

  physics = new PhysicsObject(world->physics, this);
  physics->stayUpright = true;

  int cooldownDuration = 1;
  int maxHeat = 100;
  int heatPerShot = 10;
  weaponSet[0] = new Weapon(this, cooldownDuration, maxHeat, heatPerShot);
};

void Player::createCamera() {
  camera = (Camera*)CreateCamera();
  camera->attachTo(this);
  camera->translate(cameraDistance.x, cameraDistance.y, cameraDistance.z);
  camera->rotate(Vector(1, 0, 0), 3.14f / 32);
}

void Player::update() {
	if(isAlive()){
		recoverThrusters();
		input.update(world, this);
		weaponSet[0]->checkCoolDown();
	}
	else
	{
		physics->stayUpright = false;
	}
	GameObject::update();
}

void Player::useThruster(int amount) {
    thruster -= amount;
    if (thruster <= 0) {
      thruster = 0;
      thrusterCooldown = 200;
    }
}

void Player::recoverThrusters() {
  if (thruster < 250) ++thruster;
  if (thrusterCooldown) {
    if (thruster < 250) ++thruster;
    --thrusterCooldown;
  }
}

void Player::onCollision(Projectile* projectile) {
	health -= projectile->damage;
	applyForce(projectile->force * direction(getAABB().center(), projectile->getAABB().center()));
}

bool Player::isAlive(){
	return health > 0;
}

Player::~Player() {
  if (camera) delete camera;
}