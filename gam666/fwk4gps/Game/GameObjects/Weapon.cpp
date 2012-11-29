#include "Weapon.h"
#include "../GameObject.h"
#include "../World.h"
#include "Projectile.h"
#include "..\Utilities\ObjImporter.h"
#include "..\Mesh.h"
#include "..\..\iTexture.h"
#include "GameTimer.h"

#define CPS (float)CLOCKS_PER_SEC

Weapon::Weapon(Player* o, float cdDuration, int mHeat, int hPerShot) : 
  cooldownTimer(cdDuration), refireTimer(0.0001f * CPS) {
	owner = o;
	maxHeat = mHeat;
	heatPerShot = hPerShot;
	currentHeat = 0;
}

void Weapon::fireProjectile() {
	if(refireTimer.timerActive) {
    refireTimer.checkTimer();
	}

  if(!cooldownTimer.timerActive && !refireTimer.timerActive) {
    Mesh* mesh = ObjImporter::import("sphere.obj");
    mesh->buildScale = 20;
		Projectile* proj = new Projectile(owner->getWorld(), owner, mesh->getVertexList(), 10);
    proj->translate(0, 20, 0);
    proj->model->setReflectivity(&Reflectivity(Colour(0, 0.8f, 0, 0.5f)));
		proj->shoot();
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