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
	Animation* animation = nullptr;
	Collider* collider = nullptr;
	uint points = 0;

public:
	iPoint position;
	Mix_Chunk* deadsfx;
	ENEMY_TYPES type = ENEMY_TYPES::NO_TYPE;
public:
	Enemy(int x, int y);
	virtual ~Enemy();

	const Collider* GetCollider() const;

	virtual void Move() {};
	virtual void Draw(SDL_Texture* sprites);
	virtual void OnCollision(Collider* collider);
};

#endif // __ENEMY_H__