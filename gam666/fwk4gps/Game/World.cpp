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
#include "Utils.h"
#include "btBulletDynamicsCommon.h"

#include "GameObjects\Player.h"
#include "GameObjects\Floor.h"

World::World(Game* game) : Coordinator(game->handle, game->show) {
  this->game = game;
  initializeLighting();
  initializeObjects();
  initializeHUD();
}

void World::initializeHUD() {
  iHUD* hud = CreateHUD(CreateGraphic(), 0.1f, 0.1f, 0.43f, 0.43f, CreateTexture(HUD_IMAGE));
  setTimerText(CreateText(Rectf(0.0f, 0.05f, 0.2f, 0.15f), hud, L"",
    TEXT_HEIGHT, TEXT_TYPEFACE, TEXT_LEFT));

  CreateText(Rectf(0, 0.05f, 0.65f, 0.15f), hud, L" Camera: at ", position,
    Camera::getCurrent(), ' ', 1, 16, L"ARIAL", TEXT_CENTER);
}

void World::initializeLighting() {
  Colour white(1, 1, 1);
  Colour black(0, 0, 0);
  setAmbientLight(0.2f, 0.2f, 0.2f);
  defaultLight = CreateDistantLight(white, white, black, true);
}

void World::initializeObjects() {
  Colour green(0.1f, 0.8f, 0.1f);
  Colour blue(0.1f, 0.1f, 0.9f);
  Reflectivity whiteish(Colour(1, 1, 1, 0.5f));
  Reflectivity bluish(blue);

  iCamera* camera = CreateCamera();
  camera->translate(-5, 0, -80);

  iObject* bg = CreateSprite(CreateGraphic(), '\xFF');
  bg->attach(CreateTexture(L"stonehenge.bmp"));
  
  iGraphic* longPlate = CreateBox(-30, -10, 0, 600, 10, 600);
  iObject* floorModel = CreateObject(longPlate, &whiteish);
  iTexture* check = CreateTexture(L"check.bmp");
  floorModel->attach(check);
  floor = new Floor(this, floorModel);

  player = new Player(this);
  player->setTranslation(0, 10, 0);
  player->setSpeed(10, 0, 20);
  player->setAngularSpeed(2, 2, 0);
  add(player);
}

void World::update() {
  physics.update();
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

World::~World() {
  if (player) delete player;
}