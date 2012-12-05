#include "Player.h"
#include "..\World.h"
#include "..\PhysicsObject.h"
#include "..\..\Camera.h"
#include "..\..\Sound.h" // Sound & Music
#include "Weapon.h"
#include "WeaponSpread.h"
#include "WeaponHoming.h"
#include "Projectile.h"

#define CPS (float)CLOCKS_PER_SEC

Player::Player(World* world, int id, iGraphic* graphic) : 
GameObject(world, graphic), thruster(300), id(id), 
  thrusterCooldown(0), health(50), kills(0), deaths(0), 
  isAlive(true), cameraDistance(Vector(0, 40, -150)),
  lifeTimer(0.0f), respawnTimer(10.0f) { 
    startingHealth = health;
    createCamera();
	initSounds();
    physics = new PhysicsObject(world->physics, this);
    physics->stayUpright = true;

  float cooldownDuration = 1.0f;
  int maxHeat = 100;
  int heatPerShot = 10;
  weaponSet[0] = new Weapon(this, cooldownDuration, maxHeat, heatPerShot);
  weaponSet[1] = new WeaponSpread(this, 2.0f, maxHeat, maxHeat / 2);
  weaponSet[2] = new WeaponHoming(this, 5.0f, maxHeat, maxHeat);
  setTranslation(findSpawnPoint());
};

void Player::initSounds() {
	jumpSound = CreateSound(L"sfx/jump.wav", false);
	deathSound = CreateSound(L"sfx/death.wav", false);
	beenHitSound = CreateSound(L"sfx/hit.wav", false);
}

void Player::createCamera() {
  camera = (Camera*)CreateCamera();
  camera->attachTo(this);
  camera->translate(cameraDistance.x, cameraDistance.y, cameraDistance.z);
  camera->rotate(Vector(1, 0, 0), 3.14f / 32);
}

void Player::update() {
  physics->stayUpright = isAlive;
  if(isAlive) {
    recoverThrusters();
    input.update(world, this);
    weaponSet[0]->cooldownTimer.checkTimer();
    weaponSet[1]->cooldownTimer.checkTimer();
    weaponSet[2]->cooldownTimer.checkTimer();
    } else {
      if (!respawnTimer.checkTimer()) respawn();
    }
    GameObject::update();
}

void Player::useThruster(int amount) {
  thruster -= amount;
  jumpSound->play();
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
  beenHitSound->play();
  applyForce(projectile->force * direction(getAABB().center(), projectile->getAABB().center()));
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

void Player::die() {
  deaths.push_back((clock() - lifeTimer.getTime()) / CPS);
  respawnTimer.reset();
  deathSound->play();
  physics->applyImpulse(Vector(0, 0, -750), Vector(0, 100, 0));
  isAlive = false;
}

void Player::respawn() {
  physics->resetRotation();
  setTranslation(findSpawnPoint());
  health = startingHealth;
  isAlive = true;
}

Player::~Player() {
  if (camera) delete camera;
}