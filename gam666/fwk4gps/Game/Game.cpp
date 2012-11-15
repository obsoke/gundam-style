#include "Game.h"
#include "World.h"
#include "Map.h"
#include "..\Texture.h"
#include "Utilities\ObjImporter.h"

Game::Game(void* handle, int show) {
  this->handle = handle;
  this->show = show;
  currentState = new World(this, DefaultMap());
  currentState->run();
}

Game::~Game() {
  ObjImporter::disposeLibrary();
  if (currentState) delete currentState;
}

void Game::update() {
  currentState->update();
}