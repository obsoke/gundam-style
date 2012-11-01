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
  farcp = 10000.0f;
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
  Colour blue(0, 1, 0);
  setAmbientLight(0.2f, 0.2f, 0.2f);
  defaultLight = CreateDistantLight(white, white, black, true);
  defaultLight->rotate(Vector(1, 1, 0), 3.14f / 4);
}

void World::initializeObjects() {
  Colour green(0.1f, 0.8f, 0.1f);
  Colour blue(0.1f, 0.1f, 0.9f);
  Reflectivity bluish(blue);

  Camera* camera = (Camera*)CreateCamera();
  //camera->useInput = true;

  iObject* bg = CreateSprite(CreateGraphic(), '\xFF');
  bg->attach(CreateTexture(L"blue_nebula.jpg"));

  initializeFloors();

  player = new Player(this);
  player->setTranslation(0, 10, 0);
  
  add(player);
  camera->attachTo(player);
  camera->translate(0, 40, -100);

}

void World::initializeFloors() {
  addFloor(Vector(0, -10, 0), Vector(20, 1, 20));
  addFloor(Vector(500, -10, 0), Vector(5, 18, 5));
  addFloor(Vector(0, 140, 0), Vector(5, 1, 5));
  addFloor(Vector(0, -10, 250), Vector(5, 9, 5));
}

void World::addFloor(const Vector& position, const Vector& tiles, const Vector& tileSize, iTexture* tex) {
  if (!tex) tex = CreateTexture(L"metalbare.jpg");
  Reflectivity white(Colour(1, 1, 1));
  iGraphic* box = CreateBox(0, 0, 0, tileSize.x, tileSize.y, tileSize.z);
  iObject* floorModel = CreateObject(box, &white);
  floorModel->attach(tex);
  floors.push_back(new Floor(this, floorModel, position, tiles));
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
  if (floor) delete floor;
  if (player) delete player;
}