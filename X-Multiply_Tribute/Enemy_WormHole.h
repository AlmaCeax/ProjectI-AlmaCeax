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
	bool canSpawn = true;
public:
	Enemy_WormHole(int x, int y, bool up);

	void Move();
};

#endif
