#ifndef __BLUEPATROL_H__
#define __BLUEPATROL_H__

#include "Enemy.h"

class Enemy_BluePatrol :
	public Enemy
{
private:
	Animation walk;
	Animation hitanim;
	int original_y;
	int original_x;
	int shootTimer = 0;

public:
	Enemy_BluePatrol(int x, int y);

	void Move();
};

#endif
