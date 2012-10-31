#ifndef _PHYSICS_OBJECT_H_
#define _PHYSICS_OBJECT_H_

#include <btBulletDynamicsCommon.h>
#include "..\MathDecl.h"

#define BULLET_SIZE 1.5f

class PhysicsWorld;
class GameObject;

class PhysicsObject {
  PhysicsWorld* world;
  GameObject* object;
public:
  btCollisionShape* shape;
  btDefaultMotionState* motionState;
  btRigidBody* body;

  PhysicsObject(PhysicsWorld* world, GameObject* object, btRigidBody* body = nullptr, bool isStatic = false);
  ~PhysicsObject();
  void setTranslation(const Vector& v);
  void setSpeed(const Vector& v);
  void setRotation(const Vector& axis, float angle);
  void setAngularSpeed(const Vector& v);
  void applyForce(const Vector& v);
  void create();
  void remove();
  void update();
};

#endif