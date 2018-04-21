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
	bool activePU[6];
	state state;
	fPoint speed = {2, 0};
	bool dead = false;
	bool canMove = false;
	bool injecting = true;
	bool startBoost = false;
	void BlitPlayer();
	Collider* collider;

private:
	void Die();

	SDL_Texture * graphics = nullptr;
	Animation* current_animation = nullptr;
	Animation* last_animation = nullptr;
	Animation idle;
	Animation up;
	Animation down;
	Animation death;
	Animation ejecting;
	Tentacle tentacle;
	Tentacle tentacle2;
	int cooldown = 0;
	uint startime;

};

#endif