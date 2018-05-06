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
	hide_coins, 
	hold
};

class ModuleSceneStart : public Module
{
private:
	int coin_timer = 0;
	bool font_color = false;

	SDL_Texture* graphics = nullptr;
	SDL_Rect title;
	SDL_Rect background;
	SDL_Rect company;
	_Mix_Music* music = nullptr;
	start_steps current_step = start_steps::logo_movement;

	Uint32 start_time = 0;
	Uint32 total_time = 0;
	int alpha = 0;
	
public:
	update_status Update();
	bool CleanUp();
	bool Start();
	void OnFade();
	ModuleSceneStart();
	~ModuleSceneStart();
};
#endif

