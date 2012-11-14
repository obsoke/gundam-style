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

#include "Mesh.h"
#include "Utilities\ObjImporter.h"

World::World(Game* game, Map& map) : Coordinator(game->handle, game->show), 
  game(game), numberOfPlayers(1), map(map) {
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
  const wchar_t* files[] = {
    L"space_sky_front.png", L"space_sky_left.png",
    L"space_sky_back.png", L"space_sky_right.png",
    L"space_sky_down.png", L"space_sky_up.png",
  };
  skybox = CreateSkybox(files);
  map.create(this);
  
  const wchar_t* gundamTextures[] = {
    L"gundam-tex.png", L"gundam-tex-2.png",
    L"gundam-tex-3.png", L"gundam-tex-4.png"
  };
  Mesh* mesh = ObjImporter::import("gundam.obj");
  iGraphic* vertexList = mesh->build();
  for (int i=0; i<numberOfPlayers; ++i) {
    // send in vertex list when mesh->build works
    Player* player = new Player(this, i, vertexList);
    player->model->attach(CreateTexture(gundamTextures[i]));
    if (!i) currentCam = player->getCamera();
    players.push_back(player);
    add(player);
  }
  delete mesh;
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
  checkProjectileCollision<Player>(players);
  checkProjectileCollision<Floor>(floors);
  physics->update();
  for (int i = ((int)gameObjects.size()) - 1; i >= 0; --i)
    gameObjects[i]->update();
  checkBoundaryCollision();
}

void World::render() {
  updateWorld();
  for (unsigned i=0; i<players.size(); ++i) {
    const Viewport& viewport = calcViewport(i);
    setViewport(viewport);
    for (unsigned j=0; j<sprites.size(); ++j)
      sprites[j]->translate((float)viewport.x, (float)viewport.y, 0);
    currentCam = players[i]->getCamera();
    Coordinator::render();
    for (unsigned j=0; j<sprites.size(); ++j)
      sprites[j]->translate((float)-viewport.x, (float)-viewport.y, 0);
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
  for (int i = ((int)gameObjects.size()) - 1; i >= 0; --i) {
    if (gameObjects[i] == gameObject) {
      gameObjects.erase(gameObjects.begin() + i);
      Coordinator::remove(gameObject->model);
    }
  }
}

void World::remove(Projectile* projectile) {
  remove((GameObject*)projectile);
  ::remove(projectiles, projectile);
  delete projectile;
}

void World::createProjection() {
  const Viewport& viewport = calcViewport(0);
  projection = ::projection(fov, (float)viewport.width / viewport.height, nearcp, farcp);
  display->setProjection(&projection);
}

iObject* World::CreateSprite(const wchar_t* file, const Vector& position, unsigned char a) {
  iObject* sprite = ::CreateSprite(CreateGraphic(), a);
  sprite->attach(CreateTexture(file));
  sprite->translate(position.x, position.y, 0);
  sprites.push_back(sprite);
  return sprite;
}

template<class T>
void World::checkProjectileCollision(const std::vector<T*>& objects) {
  for (int i = ((int)objects.size()) - 1; i >= 0; --i) {
    T* object = objects[i];
    for (int j = ((int)projectiles.size()) - 1; j >= 0; --j) {
      Projectile* projectile = projectiles[j];
      if (!projectile->isOwner(object) && object->collides(projectile)) {
        object->onCollision(projectile);
        projectile->onCollision(object);
      }
    }
  }
}

void World::checkBoundaryCollision() {
  for (int i = ((int)gameObjects.size()) - 1; i >= 0; --i) {
    if (!gameObjects[i]->collides(boundary)) {
      if (!gameObjects[i]->hitBoundary()) {
        remove(gameObjects[i]);
      }
    }
  }
}

World::~World() {
  for (unsigned i=0, length=floors.size(); i<length; ++i) {
    delete floors[i];
  }
  for (unsigned i=0, length=players.size(); i<length; ++i) {
    delete players[i];
  }
  for (unsigned i=0, length=sprites.size(); i<length; ++i) {
    delete sprites[i];
  }
}