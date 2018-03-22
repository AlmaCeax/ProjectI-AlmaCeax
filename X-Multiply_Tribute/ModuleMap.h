#ifndef __ModuleMap_H__
#define __ModuleMap_H__

#include "Module.h"
#include "Globals.h"

#define NUM_LAYERS 2

struct SDL_Rect;
struct SDL_Texture;

class ModuleMap :
	public Module
{
private:
	int x = 0;
	int y = 0;
	int velocity = 5;
	bool isMoving = false;
	SDL_Rect* rect[NUM_LAYERS];
	SDL_Texture* textures[NUM_LAYERS];
public:
	bool Init();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();
	ModuleMap();
	~ModuleMap();
};

#endif