#ifndef __GREENEYE_H__
#define __GREENEYE_H__

#include "Enemy.h"

class Enemy_GreenEye :
	public Enemy
{
private:
	Animation greeneye;
	bool going_up_;
	int goingdown;
	int speed;
	int original_y;
	int original_x;

public:
	Enemy_GreenEye(int x, int y, bool going_up, int _speed);

	void Move();
};

#endif
