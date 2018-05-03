#ifndef __ModuleSceneStage3_H__
#define __ModuleSceneStage3_H__

#include "Module.h"

struct SDL_Rect;
struct SDL_Texture;
struct _Mix_Music;
struct Mix_Chunk;

class ModuleSceneStage3:
	public Module
{
private:
	SDL_Texture * textures[3];
	_Mix_Music* music = nullptr;

	int x = 0;
	int y = 0;
	SDL_Rect rect[3];

public:
	update_status Update();
	bool CleanUp();
	bool Start();
	bool Init();
	ModuleSceneStage3();
	~ModuleSceneStage3();
};

#endif

