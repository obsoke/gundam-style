#ifndef _PHYSICS_OBJECT_H_
#define _PHYSICS_OBJECT_H_

#include <btBulletDynamicsCommon.h>
#include "..\MathDecl.h"

#define BULLET_SIZE 2

class PhysicsWorld;
class GameObject;

class PhysicsObject {
  PhysicsWorld* world;
  GameObject* object;
public:
  btCollisionShape* shape;
  btDefaultMotionState* motionState;
  btRigidBody* body;
  bool stayUpright;

  PhysicsObject(PhysicsWorld* world, GameObject* object, btRigidBody* body = nullptr, bool isStatic = false);
  ~PhysicsObject();
  void setTranslation(const Vector& v);
  void setSpeed(const Vector& v);
  void setRotation(const Vector& axis, float angle);
  void resetRotation();
  void setAngularSpeed(const Vector& v);
  void applyForce(const Vector& v, const Vector& offset = Vector(0,0,0));
  void applyImpulse(const Vector& v, const Vector& offset = Vector(0,0,0));
  void create();
  void remove();
  void update();
};

#endif