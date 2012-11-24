#include "Map.h"
#include "World.h"

#include "GameObjects\Floor.h"
#include "..\Graphic.h"
#include "..\Texture.h"

Floor* Map::addFloor(World* world, const Vector& position, const Vector& tiles, 
  const Vector& tileSize, iTexture* tex) {
    Floor* floor = nullptr;
    if (!tex) tex = CreateTexture(L"metalbare.jpg");
    Reflectivity white(Colour(1, 1, 1));
    iGraphic* box = CreateBox(0, 0, 0, tileSize.x, tileSize.y, tileSize.z);
    iObject* floorModel = CreateObject(box, &white);
    floorModel->attach(tex);
    floor = new Floor(world, floorModel, position, tiles);
    world->floors.push_back(floor);
    return floor;
}

float random(float range, float min = 0) {
  return range ? (rand() % (int)range) + min : 0;
}

void Map::createSpawns(World* world, const Vector& range, const Vector& minimum, int numberOfPoints) {
  for (int i=0; i<numberOfPoints; ++i) {
    Vector spawnPoint(Vector(random(range.x, minimum.x), 
      random(range.y, minimum.y), random(range.z, minimum.z)));
    bool properSpawnPoint = false;
    while (!properSpawnPoint) {
      properSpawnPoint = !world->collidesWithFloors(
        createSpawnArea(spawnPoint, 10));
      if (!properSpawnPoint)
        spawnPoint.y += 10;
    }
    world->spawnPoints.push_back(spawnPoint);
  }
}

void DefaultMap::create(World* world) {
  addFloor(world, Vector(0, -10, 0), Vector(20, 1, 20));
  addFloor(world, Vector(500, -10, 0), Vector(5, 5, 5), Vector(100, 100, 100));
  addFloor(world, Vector(0, 240, 0), Vector(5, 1, 5));
  addFloor(world, Vector(0, -10, 250), Vector(5, 9, 5));

  world->setBoundary(AABB(
    Vector(-20, -500, -30), 
    Vector(1900, 800, 1900)
  ));

  createSpawns(world, Vector(1800, 0, 1800), Vector(0, 10, 0));
}