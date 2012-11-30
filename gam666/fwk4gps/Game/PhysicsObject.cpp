#include "PhysicsObject.h"
#include "GameObject.h"
#include "PhysicsWorld.h"
#include "..\Utils.h"

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

btTransform toBtTransform(const Matrix& mat) {
  btMatrix3x3 rotation = btMatrix3x3(mat.m11, mat.m12, mat.m13, 
    mat.m21, mat.m22, mat.m23, mat.m31, mat.m32, mat.m33);
  return btTransform(rotation, btVector3(mat.m41, mat.m42, mat.m43));
}

PhysicsObject::PhysicsObject(PhysicsWorld* world, GameObject* object, btRigidBody* body, bool isStatic) :
shape(nullptr), motionState(nullptr), body(body), stayUpright(false), world(world), object(object) {
  if (!body) {
    // Create a default cube body
    AABB aabb = object->getAABB();
    shape = new btBoxShape(btVector3(aabb.width() / BULLET_SIZE, 
      aabb.height() / BULLET_SIZE, aabb.depth() / BULLET_SIZE));
    motionState = new btDefaultMotionState(toBtTransform(object->transform()));
    btVector3 fallInertia(0,0,0);
    btScalar mass = 0;
    if (!isStatic) {
      mass = 4;
      shape->calculateLocalInertia(mass,fallInertia);
    }
    btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass,motionState,shape,fallInertia);
    this->body = new btRigidBody(fallRigidBodyCI);
    this->setTranslation(object->center());
    if (isStatic)
      this->body->setFriction(1);
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
  if (stayUpright) {
    body->setAngularFactor(btVector3(0,1,0));
  } else {
    body->setAngularFactor(btVector3(1,1,1));
  }
  const btTransform& transform = body->getWorldTransform();
  Matrix mat = toMatrix(transform.getBasis(), transform.getOrigin());
  object->setMatrix(mat);
  body->setDamping(body->isActive() ? 0.3f : 0, 0);
}

void PhysicsObject::setTranslation(const Vector& v) {
  body->getWorldTransform().setOrigin(toBtVector(v));
}

void PhysicsObject::setSpeed(const Vector& v) {
  body->setLinearVelocity(toBtVector(v));
}

void PhysicsObject::setAngularSpeed(const Vector& v) {
  body->setActivationState(ACTIVE_TAG);
  body->setAngularVelocity(toBtVector(v));
}

void PhysicsObject::applyForce(const Vector& v, const Vector& offset) {
  body->setActivationState(ACTIVE_TAG);
  body->applyForce(toBtVector(v), toBtVector(offset));
}

void PhysicsObject::applyImpulse(const Vector& v, const Vector& offset) {
  body->setActivationState(ACTIVE_TAG);
  body->applyImpulse(toBtVector(v), toBtVector(offset));
}

void PhysicsObject::setRotation(const Vector& axis, float angle) {
  body->getWorldTransform().setRotation(btQuaternion(toBtVector(axis), angle));
  update();
}

void PhysicsObject::resetRotation() {
  body->getWorldTransform().setRotation(btQuaternion(0, 0, 0));
  update();
}