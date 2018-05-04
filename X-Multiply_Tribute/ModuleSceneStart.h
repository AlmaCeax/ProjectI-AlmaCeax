#ifndef __ModuleSceneStart_H__
#define __ModuleSceneStart_H__

#include "Module.h"

#define NUM_TEXTURES 2

struct SDL_Rect;
struct SDL_Texture;
struct _Mix_Music;
struct Mix_Chunk;

enum start_steps
{
	none,
	logo_movement,
	show_coins,
};

class ModuleSceneStart : public Module
{
private:
	SDL_Texture* graphics = nullptr;
	SDL_Rect title;
	SDL_Rect background;
	SDL_Rect company;
	_Mix_Music* music = nullptr;
	start_steps current_step = start_steps::logo_movement;
	
public:
	update_status Update();
	bool CleanUp();
	bool Start();
	ModuleSceneStart();
	~ModuleSceneStart();
	void OnFade();
};
#endif

