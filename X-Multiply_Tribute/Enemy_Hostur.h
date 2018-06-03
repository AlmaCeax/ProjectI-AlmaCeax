#ifndef __ENEMY_HOSTUR_H__
#define __ENEMY_HOSTUR_H__

#include "Enemy.h"
#include "Path.h"

class Enemy_Hostur :
	public Enemy
{
public:
	Enemy_Hostur(int x, int y);
	~Enemy_Hostur();

	Collider* colliders[5];
private:
	bool spawning;
	void Shoot();
	void Move();
	void Draw(SDL_Texture* sprites);
	SDL_Texture* bckg_texture;
	int fire_rate, shot_cd;
	enum shot_types {
		mid_shot,
		top_shot,
		bot_shot
	} current_shot = mid_shot;
	shot_types last_shot;


	SDL_Rect rect_1;
	SDL_Rect rect_2;
	SDL_Rect rect_3;
	SDL_Rect rect_4;
	SDL_Rect rect_5;

	iPoint background_position;
	Animation idle;
	Animation shoot;
	Animation hitanim;
};

#endif
