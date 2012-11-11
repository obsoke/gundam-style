#ifndef _INPUTCOMPONENT_H_
#define _INPUTCOMPONENT_H_

class Player;
class World;

class InputComponent {
public:
  InputComponent() { };
  void update(World* world, Player* object);
};

#endif