#include "Application.h"
#include "ModuleSceneStage1.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "ModulePlayer.h"
#include "SDL/include/SDL_Rect.h"
#include "SDL_image/include/SDL_image.h"
#include "SDL_mixer/include/SDL_mixer.h"


ModuleSceneStage1::ModuleSceneStage1(): Module()
{
	for (int i = 0; i < NUM_LAYERS; ++i)
	{
		textures[i] = nullptr;
		rect[i] = nullptr;
	}
}

bool ModuleSceneStage1::Init()
{
	if (!loadMapTextures()) {
		return false;
	}else return true;
}

bool ModuleSceneStage1::Start() {
	App->player->Enable();
	right = true;
	up = false;
	down = false;
	left = false;
	music = App->audio->LoadMusic("Assets/Audio/Music/02_Into_the_Human_Body_Stage_1_.ogg");
	shipSpawn = App->audio->LoadFx("Assets/Audio/SFX/xmultipl-026.wav");
	Mix_PlayMusic(music, -1);
	Mix_PlayChannel(-1, shipSpawn, 0);
	return true;
}

update_status ModuleSceneStage1::Update()
{
	App->render->Blit(textures[0], x, y, textrect[0], 0.5f);
	App->render->Blit(textures[1], xLayer, yLayer, textrect[1]);
	if (App->render->camera.x < -8000 && App->render->camera.x > -10400)
	{
		down = true;
	}
	else if(down) down = false;

	return update_status::UPDATE_CONTINUE;
}

bool ModuleSceneStage1::CleanUp()
{
	for (int i = NUM_LAYERS; i < 1; --i)
	{
		App->textures->Unload(textures[i]);
		textures[i] = nullptr;
		rect[i] = nullptr;
	}
	App->audio->UnloadMusic(music);
	App->audio->UnloadSFX(shipSpawn);


	return true;
}


ModuleSceneStage1::~ModuleSceneStage1()
{
}

bool ModuleSceneStage1::loadMapTextures()
{
	LOG("Loading background textures");
	//Load all background textures
	textures[0] = App->textures->Load("Assets/Sprites/Stages/Stage1/Background/FirstLvlMap.png");
	textures[1] = App->textures->Load("Assets/Sprites/Stages/Stage1/Background/BG01.png");

	//Set origin rectangles in the right coordinates
	textrect[0] = new SDL_Rect();
	textrect[0]->x = 0;
	textrect[0]->y = 0;
	textrect[0]->h = 512;
	textrect[0]->w = 4960;
	textrect[1] = new SDL_Rect();
	textrect[1]->x = 0;
	textrect[1]->y = 0;
	textrect[1]->h = 512;
	textrect[1]->w = 4960;

	if (textures[0] == nullptr) {
		return false;
	}
	else return true;
}