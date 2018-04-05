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
	//Background variables
	int x = 0;
	int y = 0;
	int velocityBackground = 1;
	//Layers
	int xLayer = 0;
	int yLayer = 0;
	int xVelocityLayer = 2;
	int yVelocityLayer = 1;
	int indexLayer = 1;
	int indexRect = 1;
	bool isMovingY = false;
	bool isMovingX = true;
	int backgroundwidth = 512;
	SDL_Rect* rect[NUM_LAYERS];
	SDL_Rect* textrect[2];
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
