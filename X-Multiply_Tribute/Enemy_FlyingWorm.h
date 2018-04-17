#ifndef __FLYINGWORM_H__
#define __FLYINGWORM_H__

#include "Enemy.h"

class Enemy_FlyingWorm :
	public Enemy
{
private:
	Animation fly;
	bool going_up = true;
	float wave = -1.0f;
	int original_y;
	int original_x;
	int shootTimer = 0;

public:
	Enemy_FlyingWorm(int x, int y);

	void Move();
};

#endif
