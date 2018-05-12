#ifndef __BLUEMOUTH_H__
#define __BLUEMOUTH_H__

#include "Enemy.h"

class Enemy_BlueMouth :
	public Enemy
{
private:
	Animation anim;
	Animation hitanim;
	int shootTimer = 0;
	bool up = false;
public:
	Enemy_BlueMouth(int x, int y, bool up);

	void Move();
};

#endif