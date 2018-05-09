#ifndef __GREENEYE_H__
#define __GREENEYE_H__

#include "Enemy.h"

class Enemy_GreenEye :
	public Enemy
{
private:
	Animation greeneye;
	bool going_up = true;
	int original_y;
	int original_x;

public:
	Enemy_GreenEye(int x, int y);

	void Move();
};

#endif
