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
#include "..\Sound.h" // Sound & Music
#include "..\iAPIWindow.h"
#include "..\Utils.h"

#include "Game.h"
#include "GameObject.h"
#include "Map.h"
#include "btBulletDynamicsCommon.h"

#include "GameObjects\Player.h"
#include "GameObjects\Floor.h"

#include "Mesh.h"
#include "Utilities\ObjImporter.h"

World::World(Game* game, Map& map) : Coordinator(game->apiObjects), 
  game(game), numberOfPlayers(1), map(map), skybox(nullptr), physics(nullptr) {
    physics = new PhysicsWorld(this);
}

void World::initialize() {
  loadingScreen();
  numberOfPlayers = userInput->getDeviceCount(CONTROLLER);
  if (!numberOfPlayers) numberOfPlayers = 1;
  farcp = 10000.0f;
  nearcp = 80.0f;
  initializeLighting();
  initializeObjects();
  initializeHUD();
  initializeMusic();
  createProjection();
  showCursor(false);
  updateOnRender = false;
}

void World::loadingScreen() {
  iObject* loadScr = ::CreateSprite(L"loading.bmp", &Rect(0,0,width,height));  
  display->beginDrawFrame(&view);  
  loadScr->render();  
  display->endDrawFrame();  
  remove(loadScr);
  delete loadScr;
}

void World::initializeHUD() {
  /*testText = CreateText(0.1f, 0.1f, "DAT LABEL: ");
  testText->setColour(0xFFFF0000);
  testText->setStyle(26);
  testText->outline();*/
}

void World::initializeMusic() {
  // GUNDAM STYLE, YO
  music = CreateSound(L"music/battleSofter.wav");
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
    Player* player = new Player(this, i, vertexList);
    player->model->attach(CreateTexture(gundamTextures[i]));
    if (!i) currentCam = player->getCamera();
    players.push_back(player);
    add(player);
  }
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
  //testText->set("TESTING");
  checkProjectileCollision<Player>(players);
  checkProjectileCollision<Floor>(floors);
  for (int i = ((int)gameObjects.size()) - 1; i >= 0; --i)
    gameObjects[i]->update();
  checkBoundaryCollision();
  physics->update();
}

void World::render() {
  updateWorld();
  updateOther();
  setViewport(Viewport(0, 0, width, height));
  clearScreen();
  for (unsigned i=0; i<players.size(); ++i) {
    const Viewport& viewport = calcViewport(i);
    setViewport(viewport);
    currentCam = players[i]->getCamera();
    currentCam->update();
    Coordinator::render();
  }
  present();
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
  iTexture* tex = CreateTexture(file);
  iObject* sprite = ::CreateSprite(CreateGraphic(tex->getWidth(), tex->getHeight()), a);
  sprite->attach(tex);
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

bool World::collidesWithFloors(const AABB& aabb) {
  bool collided = false;
  for (unsigned i=0; i<floors.size() && !collided; ++i) {
    collided = floors[i]->getAABB().intersects(aabb);
  }
  return collided;
}

World::~World() { 
  if (skybox) delete skybox;
  if (physics) delete physics;
  if (music) delete music;
}