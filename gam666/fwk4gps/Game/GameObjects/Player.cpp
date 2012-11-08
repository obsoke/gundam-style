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

Player::~Player() {
  if (camera) delete camera;
}