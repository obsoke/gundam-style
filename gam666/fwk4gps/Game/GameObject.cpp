#include "GameObject.h"
#include "World.h"
#include "..\Model.h"
#include "..\iObject.h"
#include "..\iGraphic.h"
#include "PhysicsObject.h"
#include "Utils.h"

GameObject::GameObject(World* world, iObject* object, bool createDefaultModel) : 
    world(world), physics(nullptr), isAlive(true) {
  if (!object && createDefaultModel) object = buildDefaultModel();
  if (object) setModel(object);
}

GameObject::GameObject(World* world, iGraphic* graphic) : 
    world(world), physics(nullptr), isAlive(true) {
  if (graphic)
    setModel(CreateObject(graphic, &Reflectivity(Colour(1,1,1))));
  else
    setModel(buildDefaultModel());
}

iObject* GameObject::buildDefaultModel() {
  Colour red(0.8f, 0.1f, 0.1f);
  iGraphic* boxg  = CreateBox(-10, -10, -20, 10, 10, 10);
  iObject* object = CreateObject(boxg, &Reflectivity(randomColour()));
  iGraphic* boxr  = CreateBox(-5, -5, -5, 20, 5, 5);
  iObject* child = CreateObject(boxr, &Reflectivity(red));
  child->translate(0, 0, -20);
  child->attachTo(object);
  return object;
}

Colour GameObject::randomColour() {
  return Colour(
    1.0f / ((rand() % 2) + 2), 
    1.0f / ((rand() % 2) + 2), 
    1.0f / ((rand() % 2) + 2)
  );
}

GameObject::~GameObject() {
  delete physics;
  if (model) {
    world->remove(model);
    delete model;
  }
}

void GameObject::update() {
  if (physics) {
    physics->update();
  } else {
    float angularLength = angularSpeed.length();
    Vector angularNormal = (1 / (angularLength + EPSILON)) * angularSpeed;
    rotate(angularNormal, -angularLength / 10);
    translate(speed.x, speed.y, speed.z);
  }
}

void GameObject::setModel(iObject* object) {
  model = object;
  model->attachTo(this);
  model->rotate(Vector(0, 1, 0), 3.14f);
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
    physics->setTranslation(center());
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
  else setSpeed(v / 100);
}

void GameObject::setRotation(const Vector& axis, float angle, bool sendToPhysics) {
  // TODO: set rotation on the frame here... 
  if (physics && sendToPhysics) {
    physics->setRotation(axis, angle);
  }
}

void GameObject::resetRotation() { 
  if (physics) physics->resetRotation(); 
};

World* GameObject::getWorld()
{
	return world;
}

AABB GameObject::getAABB() {
  AABB aabb = model ? model->getAABB() : AABB();
  const Vector& p = position(), s = scale();
  aabb.minimum = (aabb.minimum * s) + p;
  aabb.maximum = (aabb.maximum * s) + p;
  return aabb;
};

bool GameObject::collides(GameObject* other) {
  return getAABB().intersects(other->getAABB());
};

bool GameObject::collides(const AABB& other) {
  return getAABB().intersects(other);
};

// Keep the game object within an AABB boundary
int GameObject::stayInBounds(const AABB& boundary) {
	AABB currentAABB = getAABB();
	
	if (currentAABB.maximum.x < boundary.minimum.x) {  //outside the minimum xy plane
		currentAABB.minimum.x -= (currentAABB.maximum.x - boundary.minimum.x);
		currentAABB.maximum.x = boundary.minimum.x;
	}
	if (currentAABB.minimum.x > boundary.maximum.x) {  //outside the maximum xy plane
		currentAABB.maximum.x -= (currentAABB.minimum.x - boundary.maximum.x);
		currentAABB.minimum.x = boundary.maximum.x;
	}
	if (currentAABB.maximum.z < boundary.minimum.z) {  //outside the minimum zy plane
		currentAABB.minimum.z -= (currentAABB.maximum.z - boundary.minimum.z);
		currentAABB.maximum.z = boundary.minimum.z;
	}
	if (currentAABB.minimum.z > boundary.maximum.z) {  //outside the maximum zy plane
		currentAABB.maximum.z -= (currentAABB.minimum.z - boundary.maximum.z);
		currentAABB.minimum.z = boundary.maximum.z;
	}
	if (currentAABB.maximum.y < boundary.minimum.y) {  //outside the minimum xy plane
		currentAABB.minimum.y -= (currentAABB.maximum.y - boundary.minimum.y);
		currentAABB.maximum.y = boundary.minimum.y;
	}
	if (currentAABB.minimum.y > boundary.maximum.y) {  //outside the maximum xy plane
		currentAABB.maximum.y -= (currentAABB.minimum.y - boundary.maximum.y);
		currentAABB.minimum.y = boundary.maximum.y;
	}

	setTranslation(currentAABB.center());

	return 1;
}

int GameObject::stayInBounds() {
  return stayInBounds(world->getBoundary());
}
