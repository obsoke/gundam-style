#include "PhysicsWorld.h"

PhysicsWorld::PhysicsWorld() {
  broadphase = new btDbvtBroadphase();
  collisionConfiguration = new btDefaultCollisionConfiguration();
  dispatcher = new btCollisionDispatcher(collisionConfiguration);
  solver = new btSequentialImpulseConstraintSolver;
  world = new btDiscreteDynamicsWorld(dispatcher,broadphase,solver,collisionConfiguration);
  world->setGravity(btVector3(0,-10,0));
}

PhysicsWorld::~PhysicsWorld() {
  delete world;
  delete solver;
  delete dispatcher;
  delete collisionConfiguration;
  delete broadphase;
}

void PhysicsWorld::update() {
  world->stepSimulation(1/60.f,10);
}