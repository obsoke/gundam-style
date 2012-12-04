#include "Weapon.h"
#include "WeaponHoming.h"
#include "../GameObject.h"
#include "../World.h"
#include "Projectile.h"
#include "..\Utilities\ObjImporter.h"
#include "..\Mesh.h"
#include "..\..\iTexture.h"

#define CPS (float)CLOCKS_PER_SEC

WeaponHoming::WeaponHoming(Player* o, int cdDuration, int mHeat, int hPerShot) : Weapon(o, cdDuration, mHeat, hPerShot) {
}

void WeaponHoming::fireProjectile() {
	if(pausingForRefire) {
		checkRefireTime();
	}

	if(!coolingDown && !pausingForRefire) {
    Mesh* mesh = ObjImporter::import("sphere.obj");
    mesh->buildScale = 20;
		Projectile* proj = new Projectile(owner->getWorld(), owner, mesh->getVertexList(), 10);
    proj->translate(0, 20, 0);
    proj->model->setReflectivity(&Reflectivity(Colour(0, 0.8f, 0, 0.5f)));
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