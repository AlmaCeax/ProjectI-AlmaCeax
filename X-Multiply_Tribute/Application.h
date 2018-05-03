#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include "Globals.h"

#define NUM_MODULES 17

class ModuleWindow;
class ModuleInput;
class ModuleTextures;
class ModuleRender;
class ModuleAudio;
class ModuleSceneStart;
class ModuleSceneStage1;
class ModuleSceneStage2;
class ModuleSceneStage3;
class ModulePlayer;
class ModuleFadeToBlack;
class ModuleParticles;
class ModuleCollision;
class ModuleEnemies;
class ModulePowerUPS;
class ModuleFonts;
class ModuleUI;
class Module;


class Application
{
public:

	Module* modules[NUM_MODULES] = { nullptr };
	ModuleWindow* window = nullptr;
	ModuleRender* render = nullptr;
	ModuleInput* input = nullptr;
	ModuleTextures* textures = nullptr;
	ModuleAudio* audio = nullptr;
	ModuleSceneStart* start = nullptr;
	ModuleSceneStage1* stage1 = nullptr;
	ModuleSceneStage2* stage2 = nullptr;
	ModuleSceneStage3* stage3 = nullptr;
	ModulePlayer* player = nullptr;
	ModuleEnemies* enemies = nullptr;
	ModulePowerUPS* powerups = nullptr;
	ModuleFadeToBlack* fade = nullptr;
	ModuleParticles* particles = nullptr;
	ModuleCollision* collision = nullptr;
	ModuleFonts* fonts = nullptr;
	ModuleUI* ui = nullptr;


	Module* current_scene = nullptr;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

};

// Global var made extern for Application ---
extern Application* App;

#endif // __APPLICATION_H__