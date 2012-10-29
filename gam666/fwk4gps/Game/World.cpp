#include "World.h"

#include "..\iSound.h"      // for the Sound Interface
#include "..\iText.h"       // for the Text Interface
#include "..\iHUD.h"        // for the HUD Interface
#include "..\iLight.h"      // for the Light Interface
#include "..\iObject.h"     // for the Object Interface
#include "..\iTexture.h"    // for the Texture Interface
#include "..\Camera.h"      // for the Camera class
#include "..\iGraphic.h"    // for the Graphic Interface
#include "..\MathDef.h"     // for Matrix operations
#include "..\Model.h"       // for ROLL_SPEED
#include "..\Translation.h" // for Action enumerations
#include "Game.h"
#include "GameObject.h"

World::World(Game* game) : Coordinator(game->handle, game->show) {
  this->game = game;
  initializeLighting();
  initializeObjects();
}

void World::initializeLighting() {
  Colour white(1, 1, 1);
  Colour black(0, 0, 0);
  setAmbientLight(0.2f, 0.2f, 0.2f);
  defaultLight = CreateDistantLight(white, white, black, false);
  defaultLight->rotatex(1.57f); // rotate into the world -y direction
  defaultLight->rotatez(0.20f); // rotate 11 degrees towards  world +x 
}

void World::initializeObjects() {
  Colour green(0.1f, 0.8f, 0.1f);

  iCamera* camera = CreateCamera();
  camera->translate(-5, 0, -80);

  iObject* bg = CreateSprite(CreateGraphic(), '\xFF');
  bg->attach(CreateTexture(L"stonehenge.bmp"));

  iGraphic* boxg  = CreateBox(-10, -10, -10, 10, 10, 10, green);
  iObject* greenBox = CreateObject(boxg);
  greenBox->translate(-8, -20, 40);
}

void World::update() {
  for (int i=0, length=gameObjects.size(); i<length; ++i) {
    gameObjects[i]->update();
  }
}

void World::add(GameObject* gameObject) {
  gameObjects.push_back(gameObject);
}

void World::remove(GameObject* gameObject) {
  for (int i=0, length=gameObjects.size(); i<length; ++i) {
    if (gameObjects[i] == gameObject) {
      gameObjects.erase(gameObjects.begin() + i);
    }
  }
}