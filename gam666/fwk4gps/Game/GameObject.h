#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_

#include "..\Object.h"
#include "..\Frame.h"

class World;
class PhysicsObject;

class GameObject : public Frame {
protected:
  World* world;
  PhysicsObject* physics;
public:
  iObject* model;
  Vector speed;
  Vector angularSpeed;
  bool isAlive;

  GameObject(World* world, iObject* object = nullptr);
  ~GameObject();
  void setModel(iObject* object);
  void setModel(iGraphic* graphic, Reflectivity* reflectivity = nullptr);
  void setTranslation(const Vector& v, bool sendToPhysics = true);
  void setTranslation(float x, float y, float z) { setTranslation(Vector(x,y,z)); };
  void setSpeed(const Vector& v);
  void setSpeed(float x, float y, float z) { setSpeed(Vector(x,y,z)); };
  void setAngularSpeed(const Vector& v);
  void setAngularSpeed(float x, float y, float z) { setAngularSpeed(Vector(x,y,z)); };
  void update();
};

#endif