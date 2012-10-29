#include "Game.h"
#include "World.h"

Game::Game(void* handle, int show) {
  this->handle = handle;
  this->show = show;
  currentState = new World(this);
  currentState->run();
}

void Game::update() {
  currentState->update();
}