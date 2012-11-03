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
#include "..\APIDisplay.h" // for Viewport
#include "..\APIUserInput.h" // for Viewport
#include "..\iAPIWindow.h"

#include "Game.h"
#include "GameObject.h"
#include "Utils.h"
#include "btBulletDynamicsCommon.h"

#include "GameObjects\Player.h"
#include "GameObjects\Floor.h"

World::World(Game* game) : Coordinator(game->handle, game->show), numberOfPlayers(1) {
  this->game = game;
  physics = new PhysicsWorld(this);
}

void World::initialize() {
  numberOfPlayers = userInput->getDeviceCount(CONTROLLER);
  if (!numberOfPlayers) numberOfPlayers = 1;
  farcp = 10000.0f;
  nearcp = 80.0f;
  initializeLighting();
  initializeObjects();
  initializeHUD();
  createProjection();
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
  Colour gray(0.6f, 0.6f, 0.6f);
  Colour black(0, 0, 0);
  setAmbientLight(0.1f, 0.1f, 0.1f);
  defaultLight = CreateDistantLight(white, white, gray, true);
  defaultLight->rotate(Vector(1, 1, 0), 3.14f / 4);
}

void World::initializeObjects() {
  iObject* bg = CreateSprite(CreateGraphic(), '\xFF');
  bg->attach(CreateTexture(L"blue_nebula.jpg"));

  initializeFloors();

  for (int i=0; i<numberOfPlayers; ++i) {
    Player* player = new Player(this, i);
    if (!i) currentCam = player->getCamera();
    players.push_back(player);
    add(player);
  }
}

void World::initializeFloors() {
  addFloor(Vector(0, -10, 0), Vector(20, 1, 20));
  addFloor(Vector(500, -10, 0), Vector(5, 5, 5), Vector(100, 100, 100));
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

void World::updateWorld() {
  physics->update();
  for (int i=0, length=gameObjects.size(); i<length; ++i) {
    gameObjects[i]->update();
  }
}

void World::render() {
  updateWorld();
  for (unsigned i=0; i<players.size(); ++i) {
    setViewport(calcViewport(i));
    currentCam = players[i]->getCamera();
    Coordinator::render();
  }
}

Viewport World::calcViewport(int player) {
  int num = players.size();
  if (num == 3) ++num;
  if (num < 2) {
    return Viewport(0, 0, width, height);
  } else if (num == 2) {
    int w = width / 2, h = height, x = player * w;
    return Viewport(x, 0, w, h);
  } else {
    int w = width / 2, h = height / 2;
    int x = (player % (num / 2)) * w, y = (player / (num / 2)) * h;
    return Viewport(x, y, w, h);
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

void World::createProjection() {
    const Viewport& viewport = calcViewport(0);
    projection = ::projection(fov, (float)viewport.width / viewport.height, nearcp, farcp);
    display->setProjection(&projection);
}

World::~World() {
  for (int i=0, length=floors.size(); i<length; ++i) {
    delete floors[i];
  }
  for (int i=0, length=players.size(); i<length; ++i) {
    delete players[i];
  }
}