#ifndef __CYCLOP_H__
#define __CYCLOP_H__

#include "Enemy.h"

class Enemy_Cyclop :
	public Enemy
{
private:
	Animation anim;
	Animation hitanim;
	bool readyToShoot;
	int shootTimer = 0;

public:
	Enemy_Cyclop(int x, int y);

	void Move();
};

#endif
