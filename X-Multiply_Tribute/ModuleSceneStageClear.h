#ifndef __ModuleSceneStageClear_H__
#define __ModuleSceneStageClear_H__

#include "Module.h"



struct SDL_Rect;
struct SDL_Texture;

class ModuleSceneStageClear :
	public Module
{
private:
	SDL_Texture* textures;
	SDL_Rect letters[9];

public:
	update_status Update();
	bool CleanUp();
	bool Start();
	bool Init();
	ModuleSceneStageClear();
	~ModuleSceneStageClear();
};
#endif

