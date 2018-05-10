#ifndef __ENEMY_JUMPER_H__
#define __ENEMY_JUMPER_H__

#include "Enemy.h"

class Enemy_Jumper :
	public Enemy
{
public:
	Enemy_Jumper(int x, int y);
	~Enemy_Jumper();
	Collider* ground_collider;
	void SetDownAnim();
	void TouchGround();

private:
	int jumps = 1;
	bool going_down = true;
	Animation air;
	Animation down;
	Animation idle;
	void Move();
	bool jumping = true;
	void Jump();
	void Wait();
	Uint32 start_time;
	Uint32 total_time;
	fPoint speed = {-1,1};
};

#endif
