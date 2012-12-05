#include "Weapon.h"
#include "../GameObject.h"
#include "../World.h"
#include "Projectile.h"
#include "..\Utilities\ObjImporter.h"
#include "..\Mesh.h"
#include "..\..\iTexture.h"
#include "..\..\Sound.h"
#include "GameTimer.h"

#define CPS (float)CLOCKS_PER_SEC
//0.0001f * cps
Weapon::Weapon(Player* o, float cdDuration, int mHeat, int hPerShot, float refireTime) : 
  cooldownTimer(cdDuration), refireTimer(refireTime) {
	owner = o;
	maxHeat = mHeat;
	heatPerShot = hPerShot;
	currentHeat = 0;
	fireSound = CreateSound(L"sfx/pew.wav", false);
}

void Weapon::fireProjectile() {
	if(refireTimer.timerActive) {
      refireTimer.checkTimer();
	}

  if(!cooldownTimer.timerActive && !refireTimer.timerActive) {
    Mesh* mesh = ObjImporter::import("sphere.obj");
    mesh->buildScale = 20;
    Projectile* proj = new Projectile(owner->getWorld(), owner, mesh->getVertexList());
    proj->translate(0, 20, 0);
    proj->model->setReflectivity(&Reflectivity(Colour(0, 0.8f, 0, 0.5f)));
    proj->isHoming = false;
    proj->shoot();
    fireSound->play();
    currentHeat += heatPerShot;

    //Refire
    refireTimer.reset();

    //Check Overheat
    if(checkOverHeat()) {
      currentHeat = 0;
      cooldownTimer.reset();
    }
  }
}

bool Weapon::checkOverHeat() {
	return currentHeat > maxHeat;
}

Weapon::~Weapon() {

}