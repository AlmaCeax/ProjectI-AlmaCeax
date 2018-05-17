#ifndef __ALIEN_H__
#define __ALIEN_H__

#include "Enemy.h"

class Enemy_Alien :
	public Enemy
{
private:
	Animation walk;
	Animation shoot;
	Animation hitanim;
	Animation hitanimshoot;
	Animation anim;
	int original_y;
	int original_x;
	int shootTimer = 0;

public:
	Enemy_Alien(int x, int y);

	void Move();
};

#endif
