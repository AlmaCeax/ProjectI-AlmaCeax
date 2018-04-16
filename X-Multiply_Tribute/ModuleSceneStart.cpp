#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleSceneStage1.h"
#include "ModuleFadeToBlack.h"
#include "SDL_mixer/include/SDL_mixer.h"
#include "ModuleSceneStart.h"


update_status ModuleSceneStart::Update()
{
	if (App->input->keyboard[SDL_SCANCODE_SPACE] == 1) App->fade->FadeToBlack(this, App->stage1, 2);

	App->render->Blit(graphics, 0, 0, &background);
	App->render->Blit(graphics, 0, 0, &title);
	App->render->Blit(graphics, 116,200, &company);

	return update_status::UPDATE_CONTINUE;
}

bool ModuleSceneStart::CleanUp()
{
	App->textures->Unload(graphics);
	graphics = nullptr;

	App->audio->UnloadMusic(music);
	music = nullptr;

	return true;
}

bool ModuleSceneStart::Start()
{
	App->current_scene = this;
	graphics = App->textures->Load("Assets/Sprites/UI/UI_2.png");
	music = App->audio->LoadMusic("Assets/Audio/Music/01_X-Multiply_Title.ogg");
	Mix_PlayMusic(music, -1);

	App->render->camera.x = 0;
	App->render->camera.y = 0;

	return true;
}

ModuleSceneStart::ModuleSceneStart()
{
	title = {0,13,384,51};
	background = {61,240,384,256};
	company = {354,71,152,38};
}

ModuleSceneStart::~ModuleSceneStart()
{
}

