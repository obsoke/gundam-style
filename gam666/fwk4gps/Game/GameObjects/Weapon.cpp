#include "Weapon.h"
#include "../GameObject.h"
#include "../World.h"
#include "Projectile.h"
#include "..\Utilities\ObjImporter.h"
#include "..\Mesh.h"
#include "..\..\iTexture.h"

#define CPS (float)CLOCKS_PER_SEC

Weapon::Weapon(Player* o, int cdDuration, int mHeat, int hPerShot) {
	owner = o;
	cooldownDuration = cdDuration;
	maxHeat = mHeat;
	heatPerShot = hPerShot;
	refireDelay = 0.00005 * CLOCKS_PER_SEC;
	cooldownLeft = 0;
	currentHeat = 0;
	cooldownTimer = 0;
	coolingDown = false;
	pausingForRefire = false;
}

void Weapon::fireProjectile() {
	if(pausingForRefire) {
		checkRefireTime();
	}

	if(!coolingDown && !pausingForRefire) {
		Mesh* mesh = ObjImporter::import("sphere.obj");
		mesh->buildScale = 20;
		Projectile* proj = new Projectile(owner->getWorld(), owner, mesh->getVertexList(), 10);
		proj->translate(0, 20, 0);
		proj->model->setReflectivity(&Reflectivity(Colour(0, 0.8f, 0, 0.5f)));
		proj->isHoming = false;
		proj->shoot();
		currentHeat += heatPerShot;

		//Refire
		pausingForRefire = true;
		refireLeft = refireDelay;
		refireTimer = clock();

		//Check Overheat
		if(checkOverHeat()) {
			coolingDown = true;
			currentHeat = 0;
			cooldownLeft = cooldownDuration;
			cooldownTimer = clock();
		}
	}
}

void Weapon::checkRefireTime() {
	if(((clock() - refireTimer) / CPS >= refireDelay) && pausingForRefire) {
		refireTimer = 0;
		refireLeft = 0;
		pausingForRefire = false;
	}
	else if(((clock() - refireTimer) / CPS < refireDelay) && pausingForRefire) {
		refireLeft = (clock() - refireTimer) / CPS - refireDelay;

		if(refireLeft < 0)
			refireLeft = 0;
	}
}

void Weapon::checkCoolDown() {
	if(((clock() - cooldownTimer) / CPS >= cooldownDuration) && coolingDown) {
		cooldownTimer = 0;
		cooldownLeft = 0;
		coolingDown = false;
	}
	else if(((clock() - cooldownTimer) / CPS < cooldownDuration) && coolingDown) {
		cooldownLeft = (clock() - cooldownTimer) / CPS - cooldownDuration;

		if(cooldownLeft < 0)
			cooldownLeft = 0;
	}
}

bool Weapon::checkOverHeat() {
	return currentHeat > maxHeat;
}

Weapon::~Weapon() {

}