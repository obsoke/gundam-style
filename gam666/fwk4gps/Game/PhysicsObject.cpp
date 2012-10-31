#include "PhysicsObject.h"
#include "GameObject.h"
#include "PhysicsWorld.h"

Vector toVector(const btVector3& v) {
  return Vector(v.x(), v.y(), v.z());
}

btVector3 toBtVector(const Vector& v) {
  return btVector3(v.x, v.y, v.z);
}

Matrix toMatrix(const btMatrix3x3& mat, const btVector3& m4) {
  const btVector3& m1 = mat[0];
  const btVector3& m2 = mat[1];
  const btVector3& m3 = mat[2];
  return Matrix(m1[0], m1[1], m1[2], 0,
                m2[0], m2[1], m2[2], 0,
                m3[0], m3[1], m3[2], 0,
                m4.x(), m4.y(), m4.z(), 1);
}

PhysicsObject::PhysicsObject(PhysicsWorld* world, GameObject* object, btRigidBody* body) :
    shape(nullptr), motionState(nullptr), body(body), world(world), object(object) {
  if (!body) {
    // Create a default cube body
    shape = new btBoxShape(btVector3(20,20,20));
    motionState = new btDefaultMotionState(btTransform
      (btQuaternion(0,0,0,1),toBtVector(object->position())));
    btScalar mass = 1;
    btVector3 fallInertia(0,0,0);
    shape->calculateLocalInertia(mass,fallInertia);
    btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass,motionState,shape,fallInertia);
    this->body = new btRigidBody(fallRigidBodyCI);
  }
  create();
}

PhysicsObject::~PhysicsObject() {
  remove();
  delete body;
  delete motionState;
  delete shape;
}

void PhysicsObject::create() {
  world->world->addRigidBody(body);
}

void PhysicsObject::remove() {
  world->world->removeRigidBody(body);
}

void PhysicsObject::update() {
  const btTransform& transform = body->getWorldTransform();
  Matrix mat = toMatrix(transform.getBasis(), transform.getOrigin());
  object->setMatrix(mat);
}

void PhysicsObject::setTranslation(const Vector& v) {
  body->getWorldTransform().setOrigin(toBtVector(v));
}

void PhysicsObject::setSpeed(const Vector& v) {
  body->setLinearVelocity(toBtVector(v));
}

void PhysicsObject::setAngularSpeed(const Vector& v) {
  body->setAngularVelocity(toBtVector(v));
}