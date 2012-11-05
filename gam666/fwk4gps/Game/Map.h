#ifndef _MAP_H_
#define _MAP_H_

#include <vector>
#include "..\MathDecl.h"

class GameObject;
class World;
class iTexture;
class Floor;

class Map {
public:
  std::vector<GameObject*> objects;

  Floor* addFloor(World* world, const Vector& position, const Vector& tiles = Vector(1, 1, 1), 
    const Vector& tileSize = Vector(100, 10, 100), iTexture* tex = nullptr);
  virtual void create(World* world) = 0;
};

class DefaultMap : public Map {
public:
  void create(World* world);
};

#endif

