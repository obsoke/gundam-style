#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_

#include "..\Object.h"
#include "..\Frame.h"

class GameObject : public Frame {
  iObject* model;
public:
  Vector speed;
  Vector angularSpeed;
  bool isAlive;

  GameObject(iObject* object = nullptr);
  void setModel(iObject* object);
  void setModel(iGraphic* graphic, Reflectivity* reflectivity = nullptr);
  void update();
};

#endif