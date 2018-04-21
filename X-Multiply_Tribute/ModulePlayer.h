#ifndef __ModulePlayer_H__
#define __ModulePlayer_H__

#include "Module.h"
#include "Animation.h"
#include "Globals.h"
#include "p2Point.h"

struct SDL_Texture;
struct Mix_Chunk;
struct Collider;

struct Tentacle
{
	Collider* collider = nullptr;
	Animation anim;
	fPoint position;
	int w, h = 0;

	Tentacle();
	~Tentacle();
	//void Update();
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

	Mix_Chunk* deadsfx = nullptr;

	fPoint position;
	enum state { top, bot, idl };
	bool activePU[8];
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

	Animation* current_animation = nullptr;
	Animation up;
	Animation down;
	Animation idle;

private:
	void Die();

	SDL_Texture * graphics = nullptr;
	Animation* last_animation = nullptr;
	Animation death;
	Animation ejecting;
	int cooldown = 0;
	int cooldownBombs = 0;
	uint startime;

};

#endif