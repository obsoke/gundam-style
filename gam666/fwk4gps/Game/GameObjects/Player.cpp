#include "Player.h"
#include "..\World.h"
#include "..\PhysicsObject.h"

Player::Player(World* world) : GameObject(world), 
    thruster(300), thrusterCooldown(0) { 
  physics = new PhysicsObject(&world->physics, this);
};

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