#include "Game.h"
#include "World.h"
#include "Map.h"
#include "..\Texture.h"
#include "..\Object.h"
#include "..\APIObjects.h"
#include "Utilities\ObjImporter.h"

Game::Game(void* handle, int show) {
  this->handle = handle;
  this->show = show;
  apiObjects = new APIObjects(handle, show);
  addState(new World(this, DefaultMap()));
}

void Game::addState(iCoordinator* state) {
  iCoordinator* previous = currentState();
  if (previous) previous->stop();
  ::add(states, state);
  state->run(states.size() == 1);
}

void Game::removeState(iCoordinator* state) {
  state->stop();
  delete state;
  ::remove(states, state);
  iCoordinator* current = currentState();
  if (current) current->run();
}

void Game::toEntryPoint() {
  for (int i=((int)states.size())-1; i>=0; --i)
    removeState(states[i]);
}

Game::~Game() {
  ObjImporter::disposeLibrary();
  if (states.size())
    currentState()->stop();
  for (int i=((int)states.size())-1; i>=0; --i)
     delete states[i];
  delete apiObjects;
}