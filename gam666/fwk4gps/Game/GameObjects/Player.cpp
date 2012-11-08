#include "Player.h"
#include "..\World.h"
#include "..\PhysicsObject.h"
#include "..\..\Camera.h"

Player::Player(World* world, int id) : GameObject(world), 
    thruster(300), id(id), thrusterCooldown(0), 
    cameraDistance(Vector(0, 40, -100)) { 
  createCamera();
  physics = new PhysicsObject(world->physics, this);
  physics->stayUpright = true;
};

void Player::createCamera() {
  camera = (Camera*)CreateCamera();
  camera->attachTo(this);
  camera->translate(cameraDistance.x, cameraDistance.y, cameraDistance.z);
  camera->rotate(Vector(1, 0, 0), 3.14f / 32);
}

void Player::update() {
  recoverThrusters();
  input.update(world, this);
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

int Player::hitBoundary() {   //Player is completely outside boundary, move back inside
	AABB tmpWrd = world->getBoundary();
	AABB tmpPlr = getAABB();
	
	if (tmpPlr.maximum.x < tmpWrd.minimum.x) {  //outside the minimum xy plane
		tmpPlr.minimum.x -= (tmpPlr.maximum.x - tmpWrd.minimum.x);
		tmpPlr.maximum.x = tmpWrd.minimum.x;
	}
	if (tmpPlr.minimum.x > tmpWrd.maximum.x) {  //outside the maximum xy plane
		tmpPlr.maximum.x -= (tmpPlr.minimum.x - tmpWrd.maximum.x);
		tmpPlr.minimum.x = tmpWrd.maximum.x;
	}
	if (tmpPlr.maximum.z < tmpWrd.minimum.z) {  //outside the minimum zy plane
		tmpPlr.minimum.z -= (tmpPlr.maximum.z - tmpWrd.minimum.z);
		tmpPlr.maximum.z = tmpWrd.minimum.z;
	}
	if (tmpPlr.minimum.z > tmpWrd.maximum.z) {  //outside the maximum zy plane
		tmpPlr.maximum.z -= (tmpPlr.minimum.z - tmpWrd.maximum.z);
		tmpPlr.minimum.z = tmpWrd.maximum.z;
	}
	if (tmpPlr.maximum.y < tmpWrd.minimum.y) {  //outside the minimum xy plane
		tmpPlr.minimum.y -= (tmpPlr.maximum.y - tmpWrd.minimum.y);
		tmpPlr.maximum.y = tmpWrd.minimum.y;
	}
	if (tmpPlr.minimum.y > tmpWrd.maximum.y) {  //outside the maximum xy plane
		tmpPlr.maximum.y -= (tmpPlr.minimum.y - tmpWrd.maximum.y);
		tmpPlr.minimum.y = tmpWrd.maximum.y;
	}

	setTranslation(tmpPlr.center());

	return 1;
}

Player::~Player() {
  if (camera) delete camera;
}