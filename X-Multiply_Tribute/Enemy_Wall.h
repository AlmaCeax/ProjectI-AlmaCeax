#ifndef __WALL_H__
#define __WALL_H__

#include "Enemy.h"

class Enemy_Wall :
	public Enemy
{
private:
	Animation anim1;
	Animation anim2;
	Animation anim3;
	Animation anim4;
	Animation hitanim1;
	Animation hitanim3;
	Animation hitanim4;
	int original_y;
	int original_x;
	int shootTimer = 0;
	float rangle3 = 0;
	float rangle4 = 0;
	SDL_Point center3;
	SDL_Point center4;
	Collider* collider3 = nullptr;
	Collider* collider4 = nullptr;
	int coll3;
	int coll4;
	

public:
	Enemy_Wall(int x, int y);
	~Enemy_Wall();

	void Move();
	void Draw(SDL_Texture* sprites);
};

#endif