#ifndef __SNAKE_H__
#define __SNAKE_H__

#include "Enemy.h"
#include "Enemy_SnakeBody.h"
#include "Path.h"

class Enemy_Snake :
	public Enemy
{
private:
	Animation anim;
	Animation hitanim;
	int idChildren;
	int shootTimer = 0;
	int radius = 40;
	float angle = 0;
	iPoint original_position;
	fPoint lastposition;
	iPoint circleCenter;
	int left = 0;
	bool startedcircle = false;
	Enemy_SnakeBody* nextPart = nullptr;
	enum State { START, CIRCLE, END, NONE };
	State state;
	bool createdBody = false;
public:
	Enemy_Snake(int x, int y, int id);
	void OnDeath();
	void Move();
	void Shine();
	void OnCollision(Collider * collider);
};

#endif
