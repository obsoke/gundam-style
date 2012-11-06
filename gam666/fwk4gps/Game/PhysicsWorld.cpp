#include "PhysicsWorld.h"
#include "World.h"

PhysicsWorld::PhysicsWorld(World* parent) : parent(parent) {
  broadphase = new btDbvtBroadphase();
  collisionConfiguration = new btDefaultCollisionConfiguration();
  dispatcher = new btCollisionDispatcher(collisionConfiguration);
  solver = new btSequentialImpulseConstraintSolver;
  world = new btDiscreteDynamicsWorld(dispatcher,broadphase,solver,collisionConfiguration);
  world->setGravity(btVector3(0,-60,0));
}

PhysicsWorld::~PhysicsWorld() {
  delete world;
  delete solver;
  delete dispatcher;
  delete collisionConfiguration;
  delete broadphase;
}

void PhysicsWorld::update() {
  float delta = parent ? 1000.0f / (int)parent->getDelta() : 30;
  world->stepSimulation(1 / (delta / 2), 10);
}