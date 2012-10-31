#include "Floor.h"
#include "..\PhysicsObject.h"
#include "..\World.h"

Floor::Floor(World* world, iObject* object, const Vector& pos) : GameObject(world, object) {
  setTranslation(pos, false);
  physics = new PhysicsObject(&(world->physics), this, nullptr, true);
}