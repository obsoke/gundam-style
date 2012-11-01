#ifndef _FLOOR_H_
#define _FLOOR_H_

#include "..\GameObject.h"
#include <vector>

class World;

class Floor : public GameObject {
  std::vector<iObject*> tiles;
  AABB aabb;
public:
  Floor(World* world, iObject* object = nullptr, 
    const Vector& pos = Vector(0, -10, 0), 
    const Vector& tiles = Vector(1, 1, 1));
  ~Floor();
  AABB getAABB();
  AABB calcAABB();
  Vector center();
};

#endif