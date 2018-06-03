#ifndef __WORMBODY_H__
#define __WORMBODY_H__

#include "Enemy.h"

class Enemy_WormBody :
	public Enemy
{
private:
	Animation anim;
	Animation hitanim;
	bool tail;
	int shootTimer = 0;
	bool up = false;
	iPoint original_position;
	int curvetimer = 0;

public:
	Enemy_WormBody(int x, int y, bool up, bool tail);
	bool splited = false;
	void Move();
	iPoint lastposition;
	iPoint circleCenter;
	int left = 0;
	int radius = 40;
	bool startedcircle = false;
	float angle = 0;
	enum state { VERTICAL, HORIZONTAL, CIRCLE, CURVEL, CURVER, NONE, HEADDEAD };
	state state;
};

#endif

