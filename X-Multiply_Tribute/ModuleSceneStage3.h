#ifndef __ModuleSceneStage3_H__
#define __ModuleSceneStage3_H__

#include "ModuleStage.h"
#include "Enemy.h"

struct SDL_Rect;

class ModuleSceneStage3 :
	public ModuleStage
{
private:
	SDL_Texture * textures[5];
	_Mix_Music* secondTrack = nullptr;
	Enemy* boss[4];

	int x = 0;
	int y = 0;
	int timer;
	int bgalpha = 255;
	bool collider = true;
	SDL_Rect rect[4];
	SDL_Rect coll;
	Animation door;
	bool bossdead = false;
	bool bossspawned = false;

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



