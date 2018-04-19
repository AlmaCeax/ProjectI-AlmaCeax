#ifndef __POWERUPSHIP_H__
#define __POWERUPSHIP_H__

#include "Enemy.h"

class Enemy_PowerUPShip :
	public Enemy
{
private:
	Animation anim;
	int original_y;
	int original_x;

public:
	Enemy_PowerUPShip(int x, int y);
	void OnCollision(Collider* collider);
	void Move();
};

#endif
