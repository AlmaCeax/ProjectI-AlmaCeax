#ifndef __BROWNEYE_H__
#define __BROWNEYE_H__

#include "Enemy.h"

class Enemy_BrownEye :
	public Enemy
{
private:
	Animation shoot;
	Animation hitanim;
	int original_y;
	int original_x;
	int shootTimer = 0;

public:
	Enemy_BrownEye(int x, int y);

	void Move();
};

#endif
