#ifndef __ALIEN_H__
#define __ALIEN_H__

#include "Enemy.h"

class Enemy_Alien :
	public Enemy
{
private:
	Animation walkright;
	Animation walk;
	Animation shoot;
	Animation shootright;
	Animation hitanim;
	int original_y;
	int original_x;
	int shootTimer = 0;
	bool right = false;

public:
	Enemy_Alien(int x, int y);

	void Move();
};

#endif
