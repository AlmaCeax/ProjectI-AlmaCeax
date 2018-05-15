#ifndef __ModulePlayer_H__
#define __ModulePlayer_H__

#include "Module.h"
#include "Animation.h"
#include "Globals.h"
#include "p2Point.h"
#include <math.h>

struct SDL_Texture;
struct Mix_Chunk;
struct Collider;

class Tentacle 
{
public:
	Collider* collider = nullptr;
	Animation anim;
	fPoint position;
	fPoint base_position;
	fPoint final_position;
	fPoint origin_position;
	float distance;
	int w, h = 0;
	enum movement { up, down, right, left, rest };

	Tentacle();
	~Tentacle();
	void MoveTentacle(movement movement, int location_position);
};

struct FireCircle {
	Tentacle* my_tentacle;
	fPoint position;
	fPoint shot_vector;
	void Move(float x, float y) {
		position = { my_tentacle->position.x-1,my_tentacle->position.y-8};
		shot_vector = { position.x - (x + 18), position.y -(y+7)};
	}
};

class ModulePlayer : public Module
{
public:
	ModulePlayer();
	~ModulePlayer();

	bool Init();
	bool Start();
	bool CleanUp();
	void OnCollision(Collider*, Collider*);
	update_status PreUpdate();
	update_status Update();
	void ResetPU();

	Mix_Chunk* deadsfx = nullptr;

	fPoint position;
	enum state { top, bot, idl };
	bool activePU[9];
	state state;
	fPoint speed = {2, 0};
	bool dead = false;
	bool canMove = false;
	bool injecting = true;
	bool startBoost = false;
	void BlitPlayer();
	Collider* collider;
	Tentacle tentacle;
	Tentacle tentacle2;
	FireCircle circle_1;
	FireCircle circle_2;
	SDL_Rect firecircle;
	//Alma Ceax

	Animation* current_animation = nullptr;
	Animation up;
	Animation down;
	Animation idle;
	Animation uptoidle;
	Animation downtoidle;

private:
	void Die();

	SDL_Texture * graphics = nullptr;
	Animation* last_animation = nullptr;
	Animation death;
	Animation ejecting;
	int cooldown = 0;
	int cooldownBombs = 0;
	float lastY;
	uint startime;
	bool movedDown = false;
	bool gamepadActived = false;

};

#endif