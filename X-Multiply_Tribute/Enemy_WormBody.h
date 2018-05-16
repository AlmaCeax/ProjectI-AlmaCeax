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
	enum state { VERTICAL, HORIZONTAL, CIRCLE, CURVE, NONE, HEADDEAD };
	state state;
};

#endif

