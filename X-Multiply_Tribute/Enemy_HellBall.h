#ifndef __HELLBALL_H__
#define __HELLBALL_H__

#include "Enemy.h"
#include "Path.h"

class Enemy_HellBall :
	public Enemy
{
private:
	Animation open_mouth;
	iPoint origin;

	bool going_up = true;
	float angle = 0;
	int radius = 30;


public:
	Enemy_HellBall(int x, int y, bool going_up);

	void Move();
};

#endif

