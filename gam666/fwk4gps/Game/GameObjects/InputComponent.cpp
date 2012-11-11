#include "InputComponent.h"
#include "..\World.h"
#include "..\..\Model.h"       // for SPEED
#include "..\..\Translation.h" // for Action enumerations
#include "Player.h"

void InputComponent::update(World* world, Player* object) {
  float delta = 1.5f, dx = 0, dy = 0, dz = 0, ang = 0;
  int id = object->id;
  int changeX = world->change(GF_CT_POSX, id);
  int changeY = world->change(GF_CT_POSY, id);

  // add changes introduced through keyboard input
  if (changeX < -50 || world->pressed(MOVE_LEFT, id))
    ang += delta;
  if (changeX > 50 || world->pressed(MOVE_RIGHT, id))
    ang -= delta;
  if (changeY > 50 || world->pressed(MOVE_BACKWARD, id))
    dz -= delta;
  if (changeY < -50 || world->pressed(MOVE_FORWARD, id))
    dz += delta;
  if (world->pressed(THRUSTER, id) && !object->thrusterCooldown) {
    dy += delta;
    object->useThruster(10);
  }
  if (world->pressed(FIRE_WEAPON_1, id)) {
    object->weaponSet[0]->FireProjectile();
  }

  Vector displacement = 
    (float) dx * 300 * object->orientation('x') +
    (float) dy * 800 * object->orientation('y') + 
    (float) dz * 300 * object->orientation('z');
  if (dx || dy || dz)
    object->applyForce(displacement.x, displacement.y, displacement.z);
  object->setAngularSpeed(0, ang / 1.75f, 0);
}