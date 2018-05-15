#ifndef __WORM_H__
#define __WORM_H__

#include "Enemy.h"
#include "Path.h"

class Enemy_Worm :
	public Enemy
{
private:
	Animation anim;
	Animation hitanim;
	int indexchild, spawntime, originY;
	int shootTimer = 0;
	int radius = 15;
	float angle = 0;
	bool up = false;
	Path path;
	iPoint original_position;
	Enemy* bodies[8];
public:
	Enemy_Worm(int x, int y, bool up);
	void OnDeath();
	void Move();
	void Shine();
	void OnCollision(Collider * collider);
};

#endif
