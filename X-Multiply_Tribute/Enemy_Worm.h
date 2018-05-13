#ifndef __WORM_H__
#define __WORM_H__

#include "Enemy.h"

class Enemy_Worm :
	public Enemy
{
private:
	Animation anim;
	Animation hitanim;
	int shootTimer = 0;
	bool up = false;
public:
	Enemy_Worm(int x, int y, bool up);

	void Move();
};

#endif
