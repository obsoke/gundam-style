#include "Floor.h"
#include "..\PhysicsObject.h"
#include "..\World.h"
#include "..\Utils.h"

Floor::Floor(World* world, iObject* object, 
    const Vector& pos, const Vector& tiles) : 
      GameObject(world, nullptr, false) {
  AABB aabb = object->getAABB();
  Matrix transform = Matrix(object->transform());
  for (int x=0; x<tiles.x; ++x) {
    for (int y=0; y<tiles.y; ++y) {
      for (int z=0; z<tiles.z; ++z) {
        if (x || y || z) {
          object = object->clone();
          object->setMatrix(Matrix(transform));
        }
        object->translate(
            x * aabb.width(), 
            y * aabb.height(), 
            z * aabb.depth());
        object->attachTo(this);
        this->tiles.push_back(object);
      }
    }
  }
  translate(pos.x, pos.y, pos.z);
  calcAABB();
  physics = new PhysicsObject(world->physics, this, nullptr, true);
}

Floor::~Floor() {
  for (unsigned i=0; i<tiles.size(); ++i) {
    if (tiles[i]) delete tiles[i];
  }
}

AABB Floor::calcAABB() {
  AABB aabb;
  for (unsigned i=0; i<tiles.size(); ++i) {
    AABB child = tiles[i]->getAABB();
    const Vector& position = tiles[i]->position();
    Vector minimum = child.minimum + position;
    Vector maximum = child.maximum + position;
    if (!i) {
      aabb.minimum = minimum;
      aabb.maximum = maximum;
    }
    aabb.minimum.x = std::min(aabb.minimum.x, minimum.x);
    aabb.minimum.y = std::min(aabb.minimum.y, minimum.y);
    aabb.minimum.z = std::min(aabb.minimum.z, minimum.z);
    aabb.maximum.x = std::max(aabb.maximum.x, maximum.x);
    aabb.maximum.y = std::max(aabb.maximum.y, maximum.y);
    aabb.maximum.z = std::max(aabb.maximum.z, maximum.z);
  }
  this->aabb = aabb;
  return aabb;
}

AABB Floor::getAABB() {
  return aabb;
}

Vector Floor::center() {
  return getAABB().center();
}