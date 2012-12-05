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
  iTexture* tex = CreateTexture(L"loading.png");
  iObject* loadScr = ::CreateSprite(CreateGraphic(tex->getWidth(), tex->getHeight()));
  loadScr->attach(tex);
  loadScr->translate(width - tex->getWidth() - 50.0f, height - tex->getHeight() - 50.0f, 0);
  display->beginDrawFrame(&view);  
  loadScr->render();  
  display->endDrawFrame();  
  present();
  remove(loadScr);
  delete loadScr;
}

void World::initializeHUD() {
  float space = 20;
  for (int i = 0; i < numberOfPlayers; i++) {
    const Viewport& v = calcViewport(i);
    float x = i == 1 || i == 3 ? (float)v.width : 0, 
          y = i == 2 || i == 3 ? (float)v.height : 0;
    sprites.push_back(CreateSprite(HUD_HEALTH, Vector((space / 2) + x, space + y), 0.33f));
    healths.push_back(createHUDText(v, (space * 2) + x, space + y + 2, ""));
    sprites.push_back(CreateSprite(HUD_THRUSTER, Vector((space / 2) + x, (space * 3) + y), 0.33f));
    thrusters.push_back(createHUDText(v, (space / 2) + x, (space * 3) + y, "", 0xFFFF0000, 34));
    scores.push_back(createHUDText(v, (space / 2) + x, v.height + y - (space * 2), "Score: "));
    respawnTimers.push_back(createHUDText(v, x, y, "", 0x992266FF, 34, true));
    cooldown.push_back(false);
  }
}

iText* World::createHUDText(const Viewport& viewport, float x, float y, 
                            const char* label, unsigned colour, unsigned size, bool centered) {
  iText* text = CreateText(Rectf(x, y, x + (float)viewport.width, y + (float)viewport.height), mainHUD, label, colour);
  text->useScreenCoords();
  text->setColour(colour);
  text->setStyle(size, centered ? TEXT_MIDDLE | TEXT_CENTER : TEXT_DEFAULT);
  text->outline();
  return text;
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
  Mesh* mesh = ObjImporter::import("gundam-style.obj");
  iGraphic* vertexList = mesh->build();
  for (int i=0; i<numberOfPlayers; ++i) {
    Player* player = new Player(this, i, vertexList);
    player->model->attach(CreateTexture(gundamTextures[i]));
    if (!i) currentCam = player->getCamera();
    players.push_back(player);
    add(player);
  }
  //sprite = CreateSprite(L"hudBackground.bmp", Vector(100, 100, 0.0f), 10);

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
  for (int i = ((int)gameObjects.size()) - 1; i >= 0; --i)
    gameObjects[i]->update();
  checkBoundaryCollision();
  physics->update();
  updateHUD();
}

void World::updateHUD() {
  for (unsigned j = 0; j < healths.size(); ++j) {
    healths[j]->set(players[j]->health);
    if (players[j]->thruster == 0) 
      cooldown[j] = true;
    if (players[j]->thruster < 250 && cooldown[j]) {
      thrusters[j]->set("X");
    } else {
      thrusters[j]->set("");
      cooldown[j] = false;
    }
    scores[j]->set(players[j]->kills);
    if (!players[j]->isAlive) {
      int x = 0;
    }
    int time = (int)(players[j]->respawnTimer.timeLimit - players[j]->respawnTimer.getTime());
    std::string timer = players[j]->isAlive ? "" : "Respawning in ";
    if (!players[j]->isAlive) {
      timer += toString((float)time);
      timer += "...";
    }
    respawnTimers[j]->set(timer.c_str());
  }
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

iObject* World::CreateSprite(const wchar_t* file, const Vector& position, float scale, unsigned char a) {
  iTexture* tex = CreateTexture(file);
  iObject* sprite = ::CreateSprite(CreateGraphic((int)(tex->getWidth() * scale), (int)(tex->getHeight() * scale)));
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