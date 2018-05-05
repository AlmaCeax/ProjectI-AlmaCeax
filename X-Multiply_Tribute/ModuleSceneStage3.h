#ifndef __ModuleSceneStage3_H__
#define __ModuleSceneStage3_H__

#include "ModuleStage.h"

struct SDL_Rect;

class ModuleSceneStage3 :
	public ModuleStage
{
private:
	SDL_Texture * textures[4];
	_Mix_Music* secondTrack = nullptr;
	_Mix_Music* bossTrack = nullptr;

	int x = 0;
	int y = 0;
	int timer;
	int bgalpha = 255;
	SDL_Rect rect[3];
	SDL_Rect coll;
	Animation door;

	void UpdateCamera();
	void BackgroundEvents();
	bool loadMap();
	void loadEnemies();
	void loadAudio();

public:
	update_status Update();
	bool CleanUp();
	bool Start();
	bool Init();
	ModuleSceneStage3();
	~ModuleSceneStage3();
};

#endif



