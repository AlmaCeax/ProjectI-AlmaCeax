#include "Application.h"
#include "ModuleAudio.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleCollision.h"
#include "SDL_mixer\include\SDL_mixer.h"
#include "ModuleSceneStage3.h"



ModuleSceneStage3::ModuleSceneStage3()
{
	for (int i = 0; i < 3; ++i)
	{
		textures[i] = nullptr;
	}
}


ModuleSceneStage3::~ModuleSceneStage3()
{

}


bool ModuleSceneStage3::Start() {
	App->current_stage = this;

	App->player->Enable();
	App->ui->PlayerReady();
	App->player->position = { -100, 450 };

	App->render->SetCameraPosition(0, 336);

	music = App->audio->LoadMusic("Assets/Audio/Music/06_The_Rolling_Worms_Stage_3-1_.ogg");
	textures[0] = App->textures->Load("Assets/Sprites/Stages/Stage3/Backgroundbg03.png");
	textures[1] = App->textures->Load("Assets/Sprites/Stages/Stage3/Layerbg03.png");

	Mix_PlayMusic(music, -1);
	return true;
}

bool ModuleSceneStage3::Init()
{
	score_bonus = 20000;
	index = 3;
	rect[0] = { 0,0,5098,544 };
	rect[1] = { 0,0,5104,561 };
	return true;
}

update_status ModuleSceneStage3::Update()
{
	App->render->Blit(textures[0], 0, -158, &rect[0], 0.5f);
	App->render->Blit(textures[1], 0, 0, &rect[1]);
	return update_status::UPDATE_CONTINUE;
}

bool ModuleSceneStage3::CleanUp()
{
	App->player->Disable();
	App->collision->Disable();

	for (int i = 0; i < 3; ++i)
	{
		App->textures->Unload(textures[i]);
		textures[i] = nullptr;
	}

	App->audio->UnloadMusic(music);
	music = nullptr;

	return true;
}
