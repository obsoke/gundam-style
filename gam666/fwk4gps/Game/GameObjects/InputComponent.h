#ifndef _INPUTCOMPONENT_H_
#define _INPUTCOMPONENT_H_

class Player;
class World;

class InputComponent {
  bool left;
  bool down;
public:
  InputComponent() : left(false), down(true) { };
  void update(World* world, Player* object);
};

#endif