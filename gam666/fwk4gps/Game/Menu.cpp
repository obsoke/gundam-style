#include "Menu.h"

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
#include "Map.h"
#include "btBulletDynamicsCommon.h"

#include "GameObjects\Player.h"
#include "GameObjects\Floor.h"

#include "Mesh.h"
#include "Utilities\ObjImporter.h"

Menu::Menu(Game* game) : Coordinator(game->apiObjects), 
  game(game), numberOfPlayers(1), map(map), skybox(nullptr) {
    //physics = new PhysicsWorld(this);	  
}


void Menu::initialize() {    
  loadingScreen();
  numberOfPlayers = userInput->getDeviceCount(CONTROLLER);
  if (!numberOfPlayers) numberOfPlayers = 1;
  //numberOfPlayers = 4;
  farcp = 10000.0f;
  nearcp = 80.0f;
  //initializeLighting();
  //initializeObjects();
  initializeHUD();
  createProjection();  
  showCursor(true);
  //display->beginDraw();
}

void Menu::loadingScreen() {  
  iObject* loadScr;  
  loadScr = CreateSprite(L"title.bmp");    
  
  display->beginDrawFrame(&view);  
  loadScr->render();  
  display->endDrawFrame();
}

void Menu::initializeHUD() {	
  //iHUD* hud = CreateHUD(CreateGraphic(), 0.1f, 0.1f, 0.43f, 0.43f, CreateTexture(HUD_IMAGE));
  //setTimerText(CreateText(Rectf(0.0f, 0.05f, 0.2f, 0.15f), hud, L"",
  //  TEXT_HEIGHT, TEXT_TYPEFACE, TEXT_LEFT));

  //CreateText(Rectf(0, 0.05f, 0.65f, 0.15f), hud, L" Camera: at ", position,
  //  Camera::getCurrent(), ' ', 1, 16, L"ARIAL", TEXT_CENTER);
  //CreateText(Rectf(0, 0.05f, 0.65f, 0.15f), hud, L" Camera: at ", position,
  //  Camera::getCurrent(), ' ', 1, 16, L"ARIAL", TEXT_CENTER);	
  iHUD* hud = CreateHUD(CreateGraphic(), 0.0f, 0.6f, 0.99f, 0.4f, CreateTexture(L"buttonBackground.bmp"));  
  hud->toggle();

  CreateText(Rectf(0.0f, 0, 1.0f, 0.16f), hud, L"START GAME", 32, L"ARIAL", TEXT_TOP | TEXT_NORMAL | TEXT_CENTER);
  //CreateText(Rectf(0.0f, 0, 1.0f, 0.16f), hud, L"START GAME", 32, L"ARIAL", TEXT_TOP | TEXT_NORMAL | TEXT_CENTER,
//	  (TEXT_A << 24 | 255 << 16 | 255 << 8 | TEXT_B));
  CreateText(Rectf(0.0f, 0.20f, 1.0f, 0.35f), hud, L"CHANGE LEVEL", 32, L"ARIAL", TEXT_TOP | TEXT_NORMAL | TEXT_CENTER);
  CreateText(Rectf(0.0f, 0.40f, 1.0f, 0.55f), hud, L"Current Level: DefaultMap", 32, L"ARIAL", TEXT_TOP | TEXT_NORMAL | TEXT_CENTER);
  //CreateText(Rectf(0.0f, 0.70f, 1.0f, 0.85f), hud, L"DefaultMap", 32, L"ARIAL", TEXT_TOP | TEXT_NORMAL | TEXT_CENTER);
  


  //CreateText(Rectf(0, 0.05f, 0.65f, 0.15f), hud, L"START GAME", position,
  //  Camera::getCurrent(), ' ', 100, 16, L"ARIAL", TEXT_CENTER);
}

void Menu::initializeLighting() {
  Colour white(1, 1, 1);
  Colour gray(0.6f, 0.6f, 0.6f);
  Colour black(0, 0, 0);
  setAmbientLight(0.1f, 0.1f, 0.1f);
  defaultLight = CreateDistantLight(white, white, gray, true);
  defaultLight->rotate(Vector(1, 1, 0), 3.14f / 4);
}

void Menu::initializeObjects() {
  const wchar_t* files[] = {
    L"space_sky_front.png", L"space_sky_left.png",
    L"space_sky_back.png", L"space_sky_right.png",
    L"space_sky_down.png", L"space_sky_up.png",
  };
  skybox = CreateSkybox(files);
  //map.create(this);
  
  const wchar_t* gundamTextures[] = {
    L"gundam-tex.png", L"gundam-tex-2.png",
    L"gundam-tex-3.png", L"gundam-tex-4.png"
  };

}

//handle menu inputs here
void Menu::updateWorld() {
	int testKey = userInput->getDeviceCount(KEYBOARD);
	int testCon = userInput->getDeviceCount(CONTROLLER);
	int testPoint = userInput->getDeviceCount(POINTER);
	bool testInput = userInput->pressed(HUD_SELECT);

	if (pressed(FIRE_WEAPON_1)) {
		//game->removeState(game->currentState());		
		stop();
	}
	if (ptrPressed()) {
		stop();
	}
}

void Menu::render() {
  updateWorld();
  //if (isChangingState == false) {    
    Coordinator::render();
  //}
  /*for (unsigned i=0; i<players.size(); ++i) {
    const Viewport& viewport = calcViewport(i);
    setViewport(viewport);
    for (unsigned j=0; j<sprites.size(); ++j)
      sprites[j]->translate((float)viewport.x, (float)viewport.y, 0);
    currentCam = players[i]->getCamera();
    Coordinator::render();
    for (unsigned j=0; j<sprites.size(); ++j)
      sprites[j]->translate((float)-viewport.x, (float)-viewport.y, 0);
  }*/
}


void Menu::add(GameObject* gameObject) {
  gameObjects.push_back(gameObject);
}

void Menu::remove(GameObject* gameObject) {
  for (int i = ((int)gameObjects.size()) - 1; i >= 0; --i) {
    if (gameObjects[i] == gameObject) {
      gameObjects.erase(gameObjects.begin() + i);
      Coordinator::remove(gameObject->model);
    }
  }
}

void Menu::remove(Projectile* projectile) {
  remove((GameObject*)projectile);
  ::remove(projectiles, projectile);
  delete projectile;
}


iObject* Menu::CreateSprite(const wchar_t* file, const Vector& position, unsigned char a) {
  iObject* sprite = ::CreateSprite(CreateGraphic(), a);
  sprite->attach(CreateTexture(file));
  sprite->translate(position.x, position.y, 0);
  sprites.push_back(sprite);
  return sprite;
}



Menu::~Menu() { 
  if (skybox) delete skybox;  
}