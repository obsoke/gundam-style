#ifndef _WEAPONSPREAD_H_
#define _WEAPONSPREAD_H_

class Weapon;
class Player;

class WeaponSpread : public Weapon {
public:
	void fireProjectile();
	WeaponSpread(Player* o, int cdDuration, int mHeat, int hPerShot);
	~WeaponSpread();
};

#endif