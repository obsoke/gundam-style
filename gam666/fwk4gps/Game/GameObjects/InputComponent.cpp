#include "InputComponent.h"
#include "..\World.h"
#include "..\..\Model.h"       // for SPEED
#include "..\..\Translation.h" // for Action enumerations
#include "Player.h"

void InputComponent::update(World* world, Player* object) {
    unsigned int now = world->getNow(), lastUpdate = world->getDelta();
    int delta = (int)((now - lastUpdate) * SPEED);
    int dx = 0, dy = 0, dz = 0, ang = 0;

    // add changes introduced through keyboard input
    if (world->pressed(MOVE_LEFT))
        ang += delta;
    if (world->pressed(MOVE_RIGHT))
        ang -= delta;
    if (world->pressed(MOVE_BACKWARD))
        dz -= delta;
    if (world->pressed(MOVE_FORWARD))
        dz += delta;
    if (world->pressed(THRUSTER) && !object->thrusterCooldown) {
        dy += delta;
        object->useThruster(10);
    }
    
    Vector displacement = 
      (float) dx * 300 * object->orientation('x') +
      (float) dy * 500 * object->orientation('y') + 
      (float) dz * 300 * object->orientation('z');
    if (dx || dy || dz)
      object->applyForce(displacement.x, displacement.y, displacement.z);
    object->setAngularSpeed(0, ang / 1.75f, 0);
}