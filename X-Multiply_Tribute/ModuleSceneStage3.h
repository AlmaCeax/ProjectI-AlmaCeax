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
	bool end = false;

	bool bossfight = false;
	bool bossspawned = false;
	Uint32 start_time = 0;
	Uint32 total_time = 0;

	void UpdateCamera();
	void BackgroundEvents();
	bool loadMap();
	void loadEnemies();
	void loadAudio();

public:
	bool bossdead = false;
	int bossdeads = 0;
	update_status Update();
	bool CleanUp();
	bool Start();
	bool Init();
	ModuleSceneStage3();
	~ModuleSceneStage3();
};

#endif



