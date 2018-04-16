#ifndef __ModulePlayer_H__
#define __ModulePlayer_H__

#include "Module.h"
#include "Animation.h"
#include "Globals.h"
#include "p2Point.h"

struct SDL_Texture;
struct Mix_Chunk;
struct Collider;

class ModulePlayer : public Module
{
public:
	ModulePlayer();
	~ModulePlayer();

	bool Init();
	bool Start();
	bool CleanUp();
	void OnCollision(Collider*, Collider*);
	update_status Update();

	Mix_Chunk* baseshotsfx = nullptr;

	iPoint position;
	enum state { top, bot, idl };
	bool activePU[6];
	state state;
	int speed = 2;

private:
	
	SDL_Texture * graphics = nullptr;
	Animation* last_animation = nullptr;
	Animation idle;
	Animation up;
	Animation down;
	int cooldown = 0;

	Collider* collider;
};

#endif