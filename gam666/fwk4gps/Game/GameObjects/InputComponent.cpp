#include "InputComponent.h"
#include "..\World.h"
#include "..\..\Model.h"       // for SPEED
#include "..\..\Translation.h" // for Action enumerations
#include "Player.h"
#include "..\..\Camera.h"
#include "..\..\MathDecl.h"
#include "..\..\MathDef.h"

void InputComponent::update(World* world, Player* object) {
  float delta = 1.5f, dx = 0, dy = 0, dz = 0, ang = 0, angy = 0;
  int id = object->id;
  int changeX = world->change(POSX, id);
  int changeY = world->change(POSY, id);
  int aimChangeX = world->change(RPOSX, id);
  int aimChangeY = world->change(RPOSY, id);

  //debug(aimChangeY);

  // add changes introduced through keyboard input
  ang -= aimChangeX / 5;
  if(aimChangeY > 60)
    aimChangeY = 60;
  angy += aimChangeY * 3.14f / 180 / 3;
  if (changeX < -50 || world->pressed(MOVE_LEFT, id))
    dx -= delta;
  if (changeX > 50 || world->pressed(MOVE_RIGHT, id))
    dx += delta;
  if (changeY > 50 || world->pressed(MOVE_BACKWARD, id))
    dz -= delta;
  if (changeY < -50 || world->pressed(MOVE_FORWARD, id))
    dz += delta;
  if (world->pressed(THRUSTER, id) && !object->thrusterCooldown) {
    dy += delta;
    object->useThruster(10);
  }
  if (world->pressed(FIRE_WEAPON_1, id)) {
    object->weaponSet[0]->fireProjectile();
  }

  Vector displacement = 
    (float) dx * 300 * object->orientation('x') +
    (float) dy * 800 * object->orientation('y') + 
    (float) dz * 300 * object->orientation('z');
  if (dx || dy || dz)
    object->applyForce(displacement.x, displacement.y, displacement.z);
  object->setAngularSpeed(angy, ang, 0);
  Camera* playerCamera = (Camera*)object->getCamera();
  Matrix* camMat = (Matrix*)playerCamera->getView();
  //debug(camMat->direction('x'));
  //playerCamera->rotate(object->orientation('x'), angy);
  playerCamera->rotatex(angy);
  debug(playerCamera->orientation('z'));
}