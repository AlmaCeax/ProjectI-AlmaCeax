#ifndef __ZARIKASU_H__
#define __ZARIKASU_H__

#include "Enemy.h"

class Particle;

class Enemy_Zarikasu :
	public Enemy
{
private:
	Animation anim;
	Animation hitanim;
	iPoint originalShotPosition;
	int shootTimer = 0;
	bool shooting = false;
	Particle* shot = nullptr;
	int myid = 0;
	bool preparation;
	SDL_Texture* bckg_texture = nullptr;
	iPoint background_position;


public:
	Enemy_Zarikasu(int x, int y, int id);

	void Move();
	void Draw(SDL_Texture* sprites);
};

#endif