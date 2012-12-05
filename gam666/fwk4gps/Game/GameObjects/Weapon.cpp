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
Weapon::Weapon(Player* o, float cdDuration, int mHeat, int hPerShot, float refireTime, iSound* fSound) : 
  cooldownTimer(cdDuration), refireTimer(refireTime) {
	owner = o;
	maxHeat = mHeat;
	heatPerShot = hPerShot;
	currentHeat = 0;

  fireSound = fSound;
}

void Weapon::fireProjectile() {
	if(refireTimer.timerActive) {
      refireTimer.checkTimer();
	}

  if(!cooldownTimer.timerActive && !refireTimer.timerActive) {
    fireSingleProjectile(Vector(3, 5, 0));
    fireSingleProjectile(Vector(-13, 5, 0));
    playSound();
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

void Weapon::fireSingleProjectile(const Vector& offset) {
    Vector pos = (Vector)offset * owner->rotation();
    Mesh* mesh = ObjImporter::import("shot.obj");
    mesh->buildScale = 30;
    Projectile* proj = new Projectile(owner->getWorld(), owner, mesh->getVertexList());
    proj->model->setReflectivity(&Reflectivity(Colour(0.8f, 0.8f, 0.3f, 0.8f)));
    proj->translate(pos.x, pos.y, pos.z);
    proj->isHoming = false;
    proj->shoot();
    proj->update();
    //fireSound->play();
}

void Weapon::playSound()
{
  fireSound->play();
}

bool Weapon::checkOverHeat() {
	return currentHeat > maxHeat;
}

Weapon::~Weapon() {

}