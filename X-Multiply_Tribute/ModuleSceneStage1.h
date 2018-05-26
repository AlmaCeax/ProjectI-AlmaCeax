#ifndef __ModuleSceneStage1_H__
#define __ModuleSceneStage1_H__

#include "ModuleStage.h"
#include "Animation.h"
#include "p2Point.h"

#define NUM_LAYERS 5

struct Enemy;

class ModuleSceneStage1 :
	public ModuleStage
{
private:
	int timer, startTime;
	bool injecting, unhooked;
	bool injectionleft = true;

	fPoint injectionposition = { 75.0f, -100.0f };
	fPoint injectionhookposition = { 90.0f, 76.0f };

	Animation startAnimation;
	Animation startAnimationHook;

	SDL_Rect textrect[NUM_LAYERS];
	SDL_Texture* textures[NUM_LAYERS];
	SDL_Rect coll;

	Mix_Chunk* shipSpawn = nullptr;

	void checkCameraEvents();
	void updateCamera();
	bool loadMap();
	void loadEnemies();
	void loadAudio();
	void injection();
	void BossUpdate();

	Enemy* boss;
	Uint32 start_time = 0;
	Uint32 total_time = 0;
	int ball_timer = 0;
	int wallboss_timer = 0;

	enum boss_phases {
		none,
		boss_spawn,
		boss_fight
	} current_phase = boss_phases::none;

public:
	bool Init();
	
	update_status Update();
	bool CleanUp();
	bool Start();
	void OnFade();

	bool first_time;
	
	ModuleSceneStage1();
	~ModuleSceneStage1();
};

#endif
