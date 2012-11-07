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

int Player::hitBoundary() {
	Vector tmpPlr = position();
	Vector tmpSpd = speed;
	AABB tmpWrd = world->getBoundary();

	//which plane did we hit?
	if (tmpPlr.x < tmpWrd.minimum.x) {
		setTranslation(tmpWrd.minimum.x, tmpPlr.y, tmpPlr.z);
		//setSpeed(speed.x, speed.y, speed.z);
		//setSpeed(tmpSpd);
	}
	if (tmpPlr.x > tmpWrd.maximum.x)
		setTranslation(tmpWrd.maximum.x, tmpPlr.y, tmpPlr.z);
	if (tmpPlr.y < tmpWrd.minimum.y)
		setTranslation(tmpPlr.x, tmpWrd.minimum.y, tmpPlr.z);
	if (tmpPlr.y > tmpWrd.maximum.y)
		setTranslation(tmpPlr.x, tmpWrd.maximum.y, tmpPlr.z);
	if (tmpPlr.z < tmpWrd.minimum.z)
		setTranslation(tmpPlr.x, tmpPlr.y, tmpWrd.minimum.z);
	if (tmpPlr.z > tmpWrd.maximum.z)
		setTranslation(tmpPlr.x, tmpPlr.y, tmpWrd.maximum.z);
	return 1;
}

Player::~Player() {
  if (camera) delete camera;
}