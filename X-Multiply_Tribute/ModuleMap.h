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
	int yLayer = 0;
	int velocityLayer = 2;
	int xVelocity = 1;
	int yVelocityLayer = 0;
	int indexLayer = 1;
	int indexRect = 1;
	bool isMoving = false;
	int backgroundwidth = 512;
	SDL_Rect* rect[NUM_LAYERS];
	SDL_Rect* textrect[3];
	SDL_Texture* textures[11];
public:
	bool Init();
	bool loadMapTextures();
	update_status Update();
	update_status PostUpdate();
	update_status PreUpdate();
	bool CleanUp();
	ModuleMap();
	~ModuleMap();
};

#endif
