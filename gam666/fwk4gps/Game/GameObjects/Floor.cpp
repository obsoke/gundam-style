#include "Floor.h"
#include "..\PhysicsObject.h"
#include "..\World.h"

Floor::Floor(World* world, iObject* object) : GameObject(world, object) {
  Vector pos(0, -100, 0);
  setTranslation(pos, false);
  btStaticPlaneShape* floorShape = new btStaticPlaneShape(btVector3(0,1,0),1);
  btDefaultMotionState* floorMotionState = new btDefaultMotionState(
    btTransform(btQuaternion(0,0,0,1),btVector3(pos.x, pos.y, pos.y)));
  btRigidBody::btRigidBodyConstructionInfo
    groundRigidBodyCI(0,floorMotionState,floorShape,btVector3(0,0,0));
  btRigidBody* floorBody = new btRigidBody(groundRigidBodyCI);
  physics = new PhysicsObject(&(world->physics), this, floorBody);
  physics->shape = floorShape;
  physics->motionState = floorMotionState;
}