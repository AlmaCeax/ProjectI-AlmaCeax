#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "ModulePlayer.h"
#include "ModuleInput.h"
#include "ModuleFadeToBlack.h"
#include "ModuleSceneStage2.h"
#include "SDL_mixer/include/SDL_mixer.h"
#include "ModuleSceneStage1.h"


ModuleSceneStage1::ModuleSceneStage1()
{
	for (int i = 0; i < NUM_LAYERS; ++i)
	{
		textures[i] = nullptr;
	}
}

bool ModuleSceneStage1::Init()
{

	if (!loadMapTextures()) {
		return false;
	}

	startAnimation1.PushBack({28, 24, 48, 102});
	startAnimation1.PushBack({107, 24, 48, 105});
	startAnimation1.PushBack({188, 24, 48, 103});
	startAnimation1.PushBack({266, 24, 48, 112});
	startAnimation1.PushBack({335, 24, 48, 120});
	startAnimation1.PushBack({414, 24, 48, 120});
	startAnimation1.repeat = false;
	startAnimation1.speed = 0.09f;

	startAnimation2.PushBack({ 414, 24, 48, 120 });
	startAnimation2.PushBack({ 335, 24, 48, 120 });
	startAnimation2.PushBack({ 266, 24, 48, 112 });
	startAnimation1.PushBack({ 188, 24, 48, 103 });
	startAnimation2.PushBack({ 107, 24, 48, 105 });
	startAnimation2.PushBack({ 28, 24, 48, 102 });
	startAnimation2.repeat = false;
	startAnimation2.speed = 0.1f;

	injecting = true;

	return true;
}

bool ModuleSceneStage1::Start() {
	App->player->Disable();
	right = false;
	up = false;
	down = false;
	left = false;

	textrect[2] = new SDL_Rect();
	textrect[2]->x = 28; 
	textrect[2]->y = 24; 
	textrect[2]->w = 48; 
	textrect[2]->h = 102; 

	music = App->audio->LoadMusic("Assets/Audio/Music/02_Into_the_Human_Body_Stage_1.ogg");
	shipSpawn = App->audio->LoadFx("Assets/Audio/SFX/xmultipl-026.wav");
	Mix_PlayMusic(music, -1);
	Mix_PlayChannel(-1, shipSpawn, 0);
	return true;
}

void ModuleSceneStage1::checkCameraEvents()
{
	if (App->render->camera.x < -8000 && App->render->camera.x > -10300)
	{
		down = true;
	}
	else if (down) down = false;
}

void ModuleSceneStage1::updateCamera()
{
	if (App->stage1->IsEnabled()) {
		int speed = 3;

		if (right) {
			App->render->camera.x -= speed;
			App->player->position.x += 1;
		}
		if (left)App->render->camera.x += speed;
		if (up) {
			timer++;
			if (timer >= 3) {
				App->render->camera.y += speed;
				timer = 0;
			}
		}
		if (down) {
			timer++;
			if (timer >= 3) {
				App->render->camera.y -= speed;
				App->player->position.y += 1;
				timer = 0;
			}
		}
	}
}

update_status ModuleSceneStage1::Update()
{

	App->render->Blit(textures[0], 0, 0, textrect[0], 0.5f);
	App->render->Blit(textures[1], 0, 0, textrect[1]);

	if (yInjection == 0 && injecting)
	{
		textrect[2] = &startAnimation1.GetCurrentFrame();
		if (startAnimation1.isDone()) {
			App->player->Enable();
			injecting = false;
		}
	}
	else {
		if (!injecting)
		{
			if (startAnimation2.isDone()) {
				yInjection--;
				right = true;
			}else textrect[2] = &startAnimation2.GetCurrentFrame();
		}else yInjection++;
	}

	App->render->Blit(textures[2], xInjection, yInjection, textrect[2]);

	checkCameraEvents();
	updateCamera();

	if (App->input->keyboard[SDL_SCANCODE_SPACE] == 1) App->fade->FadeToBlack(this, App->stage2, 2);

	return update_status::UPDATE_CONTINUE;
}

bool ModuleSceneStage1::CleanUp()
{
	App->player->Disable();

	for (int i = 0; i < NUM_LAYERS; ++i)
	{
		App->textures->Unload(textures[i]);
		textures[i] = nullptr;
	}

	App->audio->UnloadMusic(music);
	music = nullptr;
	App->audio->UnloadSFX(shipSpawn);
	shipSpawn = nullptr;
	right = false;

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
	textures[2] = App->textures->Load("Assets/Sprites/Stages/Stage1/Background/injection.png");

	if (textures[0] == nullptr) {
		return false;
	}
	else return true;
}