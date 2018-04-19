#ifndef __POWERUPSHIP_H__
#define __POWERUPSHIP_H__

#include "Enemy.h"

struct PowerUP;

class Enemy_PowerUPShip :
	public Enemy
{
private:
	Animation anim;
	int original_y;
	int original_x;
	PowerUP* pu = nullptr;

public:
	Enemy_PowerUPShip(int x, int y, int powerUpid);
	void OnCollision(Collider* collider);
	void Move();
};

#endif
