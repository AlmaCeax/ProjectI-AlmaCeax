#ifndef __SNAKEBODY_H__
#define __SNAKEBODY_H__

#include "Enemy.h"

class Enemy_SnakeBody :
	public Enemy
{
private:
	Animation anim;
	Animation hitanim;
	bool tail;
	int startTime = 0;
public:
	Enemy_SnakeBody(int x, int y, int id);
	void Move();
	void OnDeath();
	void Shine();
	Enemy_SnakeBody* nextPart = nullptr;
	bool death = false;
};

#endif