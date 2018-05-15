#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "p2Point.h"
#include "Animation.h"
#include "Globals.h"

struct SDL_Texture;
struct Collider;
struct Mix_Chunk;

class Enemy
{
protected:
	Collider* collider = nullptr;
	uint points = 0;
	uint numExplosions = 1;
	bool flipX, flipY;
public:
	Animation * animation = nullptr;
	Animation * hitAnimation = nullptr;
	bool hit = false;
	iPoint position;
	iPoint speed = {0, 0};
	ENEMY_TYPES type = ENEMY_TYPES::NO_TYPE;
public:
	Enemy(int x, int y);
	virtual ~Enemy();

	const Collider* GetCollider() const;

	virtual void Move() {};
	virtual void Draw(SDL_Texture* sprites);
	virtual void OnCollision(Collider* collider);
	virtual void Shine();
};

#endif // __ENEMY_H__