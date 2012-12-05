#ifndef _WEAPONSPREAD_H_
#define _WEAPONSPREAD_H_

class Weapon;
class Player;

class WeaponSpread : public Weapon {
public:
	void fireProjectile();
	WeaponSpread(Player* o, float cdDuration, int mHeat, int hPerShot, float refireTime);
	~WeaponSpread();
};

#endif