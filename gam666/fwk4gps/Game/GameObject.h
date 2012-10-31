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
  void setTranslation(float x, float y, float z, bool sendToPhysics = true) { 
    setTranslation(Vector(x,y,z), sendToPhysics); 
  };
  void setRotation(const Vector& axis, float angle, bool sendToPhysics = true);
  void setRotation(float x, float y, float z, float angle, bool sendToPhysics = true) { 
    setRotation(Vector(x,y,z), angle, sendToPhysics); 
  };
  void setSpeed(const Vector& v);
  void setSpeed(float x, float y, float z) { setSpeed(Vector(x,y,z)); };
  void applyForce(const Vector& v);
  void applyForce(float x, float y, float z) { applyForce(Vector(x,y,z)); };
  void setAngularSpeed(const Vector& v);
  void setAngularSpeed(float x, float y, float z) { setAngularSpeed(Vector(x,y,z)); };
  virtual void update();
};

#endif