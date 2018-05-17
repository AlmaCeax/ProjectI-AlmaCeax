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
	int radius = 40;
	float angle = 0;
	bool up = false;
	iPoint original_position;
	iPoint lastposition;
	iPoint circleCenter;
	int left = 0;
	bool startedcircle = false;
	Enemy* bodies[8];
	enum State {VERTICAL, HORIZONTAL, CIRCLE, CURVEL, CURVER, NONE};
	int curvetimer;
	bool imonright;
	State state;
	State laststate;
	bool isfirstcircle = true;
public:
	Enemy_Worm(int x, int y, bool up);
	void OnDeath();
	void Move();
	void Shine();
	void OnCollision(Collider * collider);
};

#endif
