#ifndef __ModuleSceneStage1_H__
#define __ModuleSceneStage1_H__

#include "Module.h"
#include "Globals.h"
#include "Animation.h"
#include "p2Point.h"

#define NUM_LAYERS 5

struct SDL_Rect;
struct SDL_Texture;
struct _Mix_Music;
struct Mix_Chunk;
struct Collider;

class ModuleSceneStage1 :
	public Module
{
private:
	int timer, startTime, aux;
	bool left, up, down, injecting, shake, stopped, unhooked;
	bool injectionleft = true;

	fPoint injectionposition = { 75.0f, -100.0f };
	fPoint injectionhookposition = { 90.0f, 76.0f };

	Animation startAnimation;
	Animation startAnimationHook;

	SDL_Rect* textrect[NUM_LAYERS];
	SDL_Texture* textures[NUM_LAYERS];

	_Mix_Music* music = nullptr;
	Mix_Chunk* shipSpawn = nullptr;


public:
	bool Init();
	bool loadMap();
	update_status Update();
	bool CleanUp();
	bool Start();
	void OnFade();

	bool first_time,right;

	void checkCameraEvents();
	void updateCamera();
	void injection();
	ModuleSceneStage1();
	~ModuleSceneStage1();
};

#endif
