#include "Player.h"

void Player::hitBoundary() {
	translate(speed.x * -1,speed.y * -1,speed.z * -1);	
}