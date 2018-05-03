#ifndef __ModuleSceneStage3_H__
#define __ModuleSceneStage3_H__

#include "ModuleStage.h"

struct SDL_Rect;

class ModuleSceneStage3 :
	public ModuleStage
{
private:
	SDL_Texture * textures[2];
	_Mix_Music* music = nullptr;

	int x = 0;
	int y = 0;
	SDL_Rect rect[2];

public:
	update_status Update();
	bool CleanUp();
	bool Start();
	bool Init();
	ModuleSceneStage3();
	~ModuleSceneStage3();
};

#endif



