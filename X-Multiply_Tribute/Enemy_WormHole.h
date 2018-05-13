#ifndef __WORMHOLE_H__
#define __WORMHOLE_H__

#include "Enemy.h"

class Enemy_WormHole :
	public Enemy
{
private:
	Animation anim;
	Animation hitanim;
	bool up = false;
public:
	Enemy_WormHole(int x, int y);

	void Move();
};

#endif
