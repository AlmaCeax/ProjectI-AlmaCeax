#ifndef __ModuleSceneStage1_H__
#define __ModuleSceneStage1_H__

#include "Module.h"
#include "Globals.h"

#define NUM_LAYERS 2

struct SDL_Rect;
struct SDL_Texture;
struct _Mix_Music;
struct Mix_Chunk;

class ModuleSceneStage1 :
	public Module
{
private:
	SDL_Texture* textures[NUM_LAYERS];

	_Mix_Music* music = nullptr;
	Mix_Chunk* shipSpawn = nullptr;

public:
	bool Init();
	bool loadMapTextures();
	update_status Update();
	bool CleanUp();
	bool Start();
	ModuleSceneStage1();
	~ModuleSceneStage1();
};

#endif
