#include "Player.h"
#include "..\World.h"
#include "..\PhysicsObject.h"
#include "..\..\Camera.h"
#include "Weapon.h"
#include "WeaponSpread.h"
#include "WeaponHoming.h"
#include "Projectile.h"

Player::Player(World* world, int id, iGraphic* graphic) : 
    GameObject(world, graphic), thruster(300), id(id), 
    thrusterCooldown(0), health(200), cameraDistance(Vector(0, 40, -100)) { 
  createCamera();

  physics = new PhysicsObject(world->physics, this);
  physics->stayUpright = true;

  int cooldownDuration = 1;
  int maxHeat = 100;
  int heatPerShot = 10;
  weaponSet[0] = new Weapon(this, cooldownDuration, maxHeat, heatPerShot);
  weaponSet[1] = new WeaponSpread(this, cooldownDuration, maxHeat, heatPerShot);
  weaponSet[2] = new WeaponHoming(this, cooldownDuration, maxHeat, heatPerShot);
  setTranslation(findSpawnPoint());
};

void Player::createCamera() {
  camera = (Camera*)CreateCamera();
  camera->attachTo(this);
  camera->translate(cameraDistance.x, cameraDistance.y, cameraDistance.z);
  camera->rotate(Vector(1, 0, 0), 3.14f / 32);
}

void Player::update() {
	if(isAlive()) {
		recoverThrusters();
		input.update(world, this);
		weaponSet[0]->checkCoolDown();
		weaponSet[1]->checkCoolDown();
		weaponSet[2]->checkCoolDown();
	} else {
		physics->stayUpright = false;
	}
	GameObject::update();
}

void Player::useThruster(int amount) {
    thruster -= amount;
    if (thruster <= 0) {
      thruster = 0;
      thrusterCooldown = 200;
    }
}

void Player::recoverThrusters() {
  if (thruster < 250) ++thruster;
  if (thrusterCooldown) {
    if (thruster < 250) ++thruster;
    --thrusterCooldown;
  }
}

void Player::onCollision(Projectile* projectile) {
	health -= projectile->damage;
	applyForce(projectile->force * direction(getAABB().center(), projectile->getAABB().center()));
}

bool Player::isAlive(){
	return health > 0;
}

Vector Player::findSpawnPoint() {
  bool spawnPointFound = false;
  Vector spawnPoint;
  unsigned numSpawnPoints = world->spawnPoints.size();
  unsigned numPlayers = world->players.size();
  unsigned numPointsChecked = 0;
  unsigned pointToCheck = 0;
  std::vector<bool> pointsChecked(numSpawnPoints);

  for (unsigned i=0; i<numSpawnPoints; ++i)
    pointsChecked[i] = false;

  while (!spawnPointFound) {
    pointToCheck = rand() % numSpawnPoints;
    while (pointsChecked[pointToCheck])
      pointToCheck = rand() % numSpawnPoints;
    pointsChecked[pointToCheck] = true;
    const Vector& sp = world->spawnPoints[pointToCheck];
    const AABB& spawnArea = createSpawnArea(sp);
    setTranslation(sp, false);
    if (numPlayers < 2) spawnPointFound = true;
    
    for (unsigned i=0; i<numPlayers && !spawnPointFound; ++i) {
      Player* player = world->players[i];
      if (player != this && !player->getAABB().intersects(spawnArea))
        spawnPointFound = true;
    }

    ++numPointsChecked;
    if (spawnPointFound || numPointsChecked >= numSpawnPoints)
      spawnPoint = sp;
  }

  return spawnPoint;
}

Player::~Player() {
  if (camera) delete camera;
}