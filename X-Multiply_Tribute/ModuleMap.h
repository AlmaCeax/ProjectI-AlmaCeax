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
	int xLayer = 0;
	int velocityLayer = 2;
	int velocity = 1;
	int indexLayer = 1;
	bool isMoving = false;
	int backgroundwidth = 512;
	SDL_Rect* rect[NUM_LAYERS];
	SDL_Rect* textrect[NUM_LAYERS];
	SDL_Texture* textures[NUM_LAYERS];
public:
	bool Init();
	update_status Update();
	update_status PostUpdate();
	update_status PreUpdate();
	bool CleanUp();
	ModuleMap();
	~ModuleMap();
};

#endif
