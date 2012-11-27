#ifndef _MENU_H_
#define _MENU_H_

#include <vector>
#include "..\Coordinator.h"
#include "GameObject.h"
#include "Map.h"

class Game;
struct Viewport;

class Menu : public Coordinator {  
  int numberOfPlayers;
  void initializeHUD();  
  std::vector<iText*> menuItem;
  std::vector<char*> levelList;
  int currentItem;
  int currentLevel;
  bool isMovingUp;
  bool isMovingDown;
  bool isSelected;
public:
  std::vector<iObject*> sprites;  
  Game* game;  
  Menu(Game* game);  
  ~Menu();
  void initialize();
  void loadingScreen();
  virtual void updateMenu();
  void render();
  iObject* CreateSprite(const wchar_t* file, const Vector& position = Vector(0,0,0),
    unsigned char a = '\xFF');  
};

#endif