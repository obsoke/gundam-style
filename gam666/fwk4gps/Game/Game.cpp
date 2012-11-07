#include "Game.h"
#include "World.h"
#include "Map.h"

Game::Game(void* handle, int show) {
  this->handle = handle;
  this->show = show;
  currentState = new World(this, DefaultMap());
  currentState->run();
}

void Game::update() {
  currentState->update();
}