#ifndef _PHYSICSWORLD_H_
#define _PHYSICSWORLD_H_

#include <btBulletDynamicsCommon.h>

class PhysicsWorld {
  btBroadphaseInterface* broadphase;
  btDefaultCollisionConfiguration* collisionConfiguration;
  btCollisionDispatcher* dispatcher;
  btSequentialImpulseConstraintSolver* solver;
public:
  btDiscreteDynamicsWorld* world;

  PhysicsWorld();
  ~PhysicsWorld();
  void update();
};

#endif