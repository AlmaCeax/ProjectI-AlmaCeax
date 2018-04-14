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

public:
	enum state {top, bot, idl};
	state state;
	SDL_Texture * graphics = nullptr;
	Animation* last_animation = nullptr;
	Animation idle;
	Animation up;
	Animation down;
	iPoint position;
	Collider* collider;
};

#endif