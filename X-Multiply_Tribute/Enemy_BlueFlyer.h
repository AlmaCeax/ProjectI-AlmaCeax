#pragma once
#include "Enemy.h"
#include "Path.h"

class Enemy_BlueFlyer :
	public Enemy
{
public:
	Enemy_BlueFlyer(int x, int y);
	~Enemy_BlueFlyer();
	void TouchGround();
private:
	bool canShoot;
	bool falling = true;
	Animation idle;
	void Move();
	void Shoot();
	Path path;
	iPoint original_position;
};

