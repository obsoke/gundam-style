#include "Weapon.h"
#include "WeaponHoming.h"
#include "../GameObject.h"
#include "../World.h"
#include "Projectile.h"
#include "..\Utilities\ObjImporter.h"
#include "..\Mesh.h"
#include "..\..\iTexture.h"
#include "..\..\Sound.h"
#include "GameTimer.h"

#define CPS (float)CLOCKS_PER_SEC

WeaponHoming::WeaponHoming(Player* o, float cdDuration, int mHeat, int hPerShot) : Weapon(o, cdDuration, mHeat, hPerShot) {
}

void WeaponHoming::fireProjectile() {
  if(refireTimer.timerActive) {
    refireTimer.checkTimer();
  }

  if(!cooldownTimer.timerActive && !refireTimer.timerActive) {
    Mesh* mesh = ObjImporter::import("sphere.obj");
    mesh->buildScale = 20;
    Projectile* proj = new Projectile(owner->getWorld(), owner, mesh->getVertexList(), 20.0f, 150, 15, true, 5.0f);
    proj->translate(0, 20, 0);
    proj->model->setReflectivity(&Reflectivity(Colour(0, 0.8f, 0, 0.5f)));
	proj->isHoming = true;
    proj->shoot();
	//fireSound->play();
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