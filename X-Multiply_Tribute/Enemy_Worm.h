#ifndef __WORM_H__
#define __WORM_H__

#include "Enemy.h"
#include <list>

class Enemy_Worm :
	public Enemy
{
private:
	Animation anim;
	Animation hitanim;
	int indexchild, spawntime, originY;
	int shootTimer = 0;
	bool up = false;
	Enemy* bodies[8];
public:
	Enemy_Worm(int x, int y, bool up);
	void OnDeath();
	void Move();
	void OnCollision(Collider * collider);
};

#endif
