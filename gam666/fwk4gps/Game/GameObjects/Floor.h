#ifndef _FLOOR_H_
#define _FLOOR_H_

#include "..\GameObject.h"

class World;

class Floor : public GameObject {

public:
  Floor(World* world, iObject* object = nullptr);
};

#endif