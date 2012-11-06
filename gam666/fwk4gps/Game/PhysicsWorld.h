#ifndef _PHYSICSWORLD_H_
#define _PHYSICSWORLD_H_

#include <btBulletDynamicsCommon.h>

class World;

class PhysicsWorld {
  World* parent;
  btBroadphaseInterface* broadphase;
  btDefaultCollisionConfiguration* collisionConfiguration;
  btCollisionDispatcher* dispatcher;
  btSequentialImpulseConstraintSolver* solver;
public:
  btDiscreteDynamicsWorld* world;

  PhysicsWorld(World* world);
  ~PhysicsWorld();
  void update();
};

#endif