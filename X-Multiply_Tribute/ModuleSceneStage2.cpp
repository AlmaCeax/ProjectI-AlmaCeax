#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "ModuleInput.h"
#include "ModulePlayer.h"
#include "ModuleFadeToBlack.h"
#include "ModuleSceneStageClear.h"
#include "SDL_mixer/include/SDL_mixer.h"
#include "ModuleSceneStage2.h"



ModuleSceneStage2::ModuleSceneStage2()
{
	for (int i = 0; i < 3; ++i)
	{
		bossTextures[i] = nullptr;
	}
}


ModuleSceneStage2::~ModuleSceneStage2()
{

}


bool ModuleSceneStage2::Start() {
	App->player->Enable();

	music = App->audio->LoadMusic("Assets/Audio/Music/05_Babe_Good-Lookin_Stage_2.ogg");
	bossTextures[0] = App->textures->Load("Assets/Sprites/Stages/Stage2/Background/BG02-1.png");
	bossTextures[1] = App->textures->Load("Assets/Sprites/Stages/Stage2/Background/BG02-2.png");
	bossTextures[2] = App->textures->Load("Assets/Sprites/Stages/Stage2/Background/BG02-3.png");

	Mix_PlayMusic(music, -1);
	return true;
}

bool ModuleSceneStage2::Init()
{
	boss[0] = {98,96,61,64};
	boss[1] = {0,148,512,216};
	boss[2] = {77,69,364,186};
	return true;
}

update_status ModuleSceneStage2::Update()
{

	if (App->stage2->IsEnabled()) {
		App->render->camera.x = 0;
	}

	if (App->input->keyboard[SDL_SCANCODE_SPACE] == 1) App->fade->FadeToBlack(this, App->stage_clear, 2);

	App->render->Blit(bossTextures[0], 85, 160, &boss[0], 0.5f);
	App->render->Blit(bossTextures[1], 146, 130, &boss[1], 0.5f);
	App->render->Blit(bossTextures[2], 658, 160, &boss[2], 0.5f); //the y is not correct
	return update_status::UPDATE_CONTINUE;
}

bool ModuleSceneStage2::CleanUp()
{
	App->player->Disable();

	for (int i = 0; i < 3; ++i)
	{
		App->textures->Unload(bossTextures[i]);
		bossTextures[i] = nullptr;
	}

	App->audio->UnloadMusic(music);
	music = nullptr;

	return true;
}

