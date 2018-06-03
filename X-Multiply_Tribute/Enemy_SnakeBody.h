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
	bool createdBody = false;
	int radius = 70;
	float angle = 0;
	iPoint circleCenter;
	iPoint original_position;
	int myid;
public:
	Enemy_SnakeBody(int x, int y, int id);
	void Move();
	void OnDeath();
	void Shine();
	enum State { START, CIRCLE, END, NONE };
	State state;
	bool death = false;
};

#endif