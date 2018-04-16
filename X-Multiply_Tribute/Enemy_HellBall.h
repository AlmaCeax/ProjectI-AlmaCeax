#pragma once
#include "Enemy.h"
#include "Path.h"

class Enemy_HellBall :
	public Enemy
{
private:
	Animation open_mouth;
	iPoint origin;

	bool going_up = true;
	int going_left = true;
	float angle = 0;

public:
	Enemy_HellBall(int x, int y, bool going_up);

	void Move();
};

