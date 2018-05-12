#ifndef __ENEMY_JUMPER_H__
#define __ENEMY_JUMPER_H__

#include "Enemy.h"

class Enemy_Jumper :
	public Enemy
{
public:
	Enemy_Jumper(int x, int y,bool back, bool normal_spawn);
	~Enemy_Jumper();
	Collider* ground_collider;
	void SetDownAnim();
	void TouchGround();

private:
	int jumps = 2;
	bool going_down = true;
	bool canShoot = false;
	bool canBackJump = false;
	Animation air;
	Animation down;
	Animation idle;
	void OnCollision(Collider* collider);
	void Move();
	bool jumping = true;
	void Jump();
	void Shoot();
	void Wait();
	Uint32 start_time;
	Uint32 total_time;
	fPoint speed = {-1,1};
};

#endif
