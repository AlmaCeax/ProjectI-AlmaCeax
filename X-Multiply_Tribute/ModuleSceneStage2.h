#ifndef __ModuleSceneStage2_H__
#define __ModuleSceneStage2_H__

#include "Module.h"

struct SDL_Rect;
struct SDL_Texture;
struct _Mix_Music;
struct Mix_Chunk;

class ModuleSceneStage2 : 
	public Module
{
private :
	SDL_Texture * bossTextures[3];
	_Mix_Music* music = nullptr;

	int x = 0;
	int y = 0;
	SDL_Rect boss[3];

public:
	update_status Update();
	bool CleanUp();
	bool Start();
	bool Init();
	ModuleSceneStage2();
	~ModuleSceneStage2();
};

#endif

