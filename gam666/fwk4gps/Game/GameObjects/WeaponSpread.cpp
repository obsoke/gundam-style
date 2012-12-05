#include "Weapon.h"
#include "WeaponSpread.h"
#include "../GameObject.h"
#include "../World.h"
#include "Projectile.h"
#include "..\Utilities\ObjImporter.h"
#include "..\Mesh.h"
#include "..\..\iTexture.h"
#include "..\..\Sound.h"
#include "GameTimer.h"

#define CPS (float)CLOCKS_PER_SEC

WeaponSpread::WeaponSpread(Player* o, float cdDuration, int mHeat, int hPerShot) : Weapon(o, cdDuration, mHeat, hPerShot) {
}

void WeaponSpread::fireProjectile() {
  if(refireTimer.timerActive) {
    refireTimer.checkTimer();
  }

  if(!cooldownTimer.timerActive && !refireTimer.timerActive) {
    Mesh* mesh = ObjImporter::import("sphere.obj");
    mesh->buildScale = 20;
    for (int i = 0; i < 5; i++) {
      Projectile* proj = new Projectile(owner->getWorld(), owner, mesh->getVertexList(), 20);			
      switch (i) {
        case 0:				
          proj->shiftDirection(0.1f);				
          break;
        case 1:				
          proj->shiftDirection(0.2f);
          break;
        case 2:				
          proj->shiftDirection(-0.1f);				
          break;
        case 3:				
          proj->shiftDirection(-0.2f);
          break;
        case 4:
          //proj->shiftDirection(Vector(0, 0, 0));
          break;
        }      
      proj->translate(0, 20, 0);
      proj->model->setReflectivity(&Reflectivity(Colour(0, 0.8f, 0, 0.5f)));
      proj->shoot();
    }
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