#include "GameObject.h"
#include "World.h"
#include "..\Model.h"
#include "..\iObject.h"
#include "..\iGraphic.h"
#include "PhysicsObject.h"

GameObject::GameObject(World* world, iObject* object) : 
    world(world), isAlive(true) {
  if (!object) {
    // Default GameObject to a green box for now.
    Colour green(0.1f, 0.8f, 0.1f);
    Colour red(0.8f, 0.1f, 0.1f);
    iGraphic* boxg  = CreateBox(-10, -10, -20, 10, 10, 10);
    object = CreateObject(boxg, &Reflectivity(green));
    iGraphic* boxr  = CreateBox(-5, -5, -5, 20, 5, 5);
    iObject* child = CreateObject(boxr, &Reflectivity(red));
    child->translate(0, 0, 20);
    child->attachTo(object);
  }
  setModel(object);
}

GameObject::~GameObject() {
  delete physics;
}

void GameObject::update() {
  if (physics) {
    physics->update();
  } else {
    float angularLength = angularSpeed.length();
    Vector angularNormal = (1 / (angularLength + EPSILON)) * angularSpeed;
    rotate(angularNormal, angularLength);
    translate(speed.x, speed.y, speed.z);
  }
}

void GameObject::setModel(iObject* object) {
  model = object;
  model->attachTo(this);
}

void GameObject::setModel(iGraphic* graphic, Reflectivity* reflectivity) {
  if (!reflectivity) {
    reflectivity = &Reflectivity(Colour(1, 1, 1));
  }
  setModel(CreateObject(graphic, reflectivity));
}

void GameObject::setTranslation(const Vector& v, bool sendToPhysics) {
  Frame::setTranslation(v);
  if (physics && sendToPhysics) {
    physics->setTranslation(v);
  }
}

void GameObject::setSpeed(const Vector& v) {
  speed = v;
  if (physics) physics->setSpeed(v);
}

void GameObject::setAngularSpeed(const Vector& v) {
  angularSpeed = v;
  if (physics) physics->setAngularSpeed(v);
}

void GameObject::applyForce(const Vector& v) {
  if (physics) physics->applyForce(v);
}

void GameObject::setRotation(const Vector& axis, float angle, bool sendToPhysics) {
  // TODO: set rotation on the frame here... 
  if (physics && sendToPhysics) {
    physics->setRotation(axis, angle);
  }
}