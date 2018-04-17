#ifndef __TENTACLESHOOTER_H__
#define __TENTACLESHOOTER_H__

#include "Enemy.h"

class Enemy_TentacleShooter :
	public Enemy
{
private:
	Animation anim;
	int shootTimer = 0;
public:
	Enemy_TentacleShooter(int x, int y);

	void Move();
};

#endif
