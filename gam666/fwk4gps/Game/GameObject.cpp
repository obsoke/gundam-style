#include "GameObject.h"
#include "..\Model.h"
#include "..\iObject.h"
#include "..\iGraphic.h"

GameObject::GameObject(iObject* object) : isAlive(true) {
  if (!object) {
    // Default GameObject to a green cube for now.
    Colour green(0.1f, 0.8f, 0.1f);
    iGraphic* boxg  = CreateBox(-10, -10, -10, 10, 10, 10);
    object = CreateObject(boxg, &Reflectivity(green));
  }
  setModel(object);
}

void GameObject::update() {
  float angularLength = angularSpeed.length();
  Vector angularNormal = (1 / angularLength) * angularSpeed;
  rotate(angularNormal, angularLength);
  translate(speed.x, speed.y, speed.z);
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