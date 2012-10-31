#include "InputComponent.h"
#include "World.h"
#include "..\Model.h"       // for SPEED
#include "..\Translation.h" // for Action enumerations

void InputComponent::update(World* world, GameObject* object) {
    unsigned int now = world->getNow(), lastUpdate = world->getDelta();
    int delta = (int)((now - lastUpdate) * SPEED);
    int dx = 0, dy = 0, dz = 0;

    // add changes introduced through keyboard input
    if (world->pressed(MOVE_LEFT))
        dx -= delta;
    if (world->pressed(MOVE_RIGHT))
        dx += delta;
    if (world->pressed(MOVE_BACKWARD))
        dz -= delta;
    if (world->pressed(MOVE_FORWARD))
        dz += delta;
    if (world->pressed(THRUSTER))
        dy += delta;
    
    if (dx || dy || dz)
      object->applyForce(dx * 25, dy * 25, dz * 25);
}