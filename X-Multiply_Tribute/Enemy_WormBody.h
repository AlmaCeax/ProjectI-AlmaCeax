#ifndef __WORMBODY_H__
#define __WORMBODY_H__

#include "Enemy.h"

class Enemy_WormBody :
	public Enemy
{
private:
	Animation anim;
	Animation hitanim;
	bool tail;
	int shootTimer = 0;
	bool up = false;
public:
	Enemy_WormBody(int x, int y, bool up);

	void Move();
};

#endif

