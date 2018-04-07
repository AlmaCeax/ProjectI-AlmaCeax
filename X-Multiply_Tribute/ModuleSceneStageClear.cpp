#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleFadeToBlack.h"
#include "ModuleSceneStart.h"
#include "ModuleSceneStageClear.h"



ModuleSceneStageClear::ModuleSceneStageClear()
{

}


ModuleSceneStageClear::~ModuleSceneStageClear()
{
}

bool ModuleSceneStageClear::Start() {
	textures = App->textures->Load("Assets/Sprites/UI/UI_1.png");

	return true;
}

bool ModuleSceneStageClear::Init()
{
	letters[0] = { 288,176,16,16 };//S
	letters[1] = { 304,176,16,16 };//T
	letters[2] = { 0,176,16,16 };//A
	letters[3] = { 96,176,16,16 };//G
	letters[4] = { 64,176,16,16 };//E
	letters[5] = { 32,176,16,16 };//C
	letters[6] = { 176,176,16,16 };//L
	letters[7] = { 272,176,16,16 };//R
	letters[8] = { 48,176,16,16 };//D

	return true;
}

update_status ModuleSceneStageClear::Update()
{
	if (App->input->keyboard[SDL_SCANCODE_SPACE] == 1) App->fade->FadeToBlack(this, App->start, 2);

	App->render->Blit(textures, 80, 125, &letters[0], 0.5f);//S
	App->render->Blit(textures, 96, 125, &letters[1], 0.5f);//T
	App->render->Blit(textures, 112, 125, &letters[2], 0.5f);//A
	App->render->Blit(textures, 128, 125, &letters[3], 0.5f);//G
	App->render->Blit(textures, 144, 125, &letters[4], 0.5f);//E
	App->render->Blit(textures, 192, 125, &letters[5], 0.5f);//C
	App->render->Blit(textures, 208, 125, &letters[6], 0.5f);//L
	App->render->Blit(textures, 224, 125, &letters[4], 0.5f);//E
	App->render->Blit(textures, 240, 125, &letters[2], 0.5f);//A
	App->render->Blit(textures, 256, 125, &letters[7], 0.5f);//R
	App->render->Blit(textures, 272, 125, &letters[4], 0.5f);//E
	App->render->Blit(textures, 288, 125, &letters[8], 0.5f);//D

	return update_status::UPDATE_CONTINUE;
}

bool ModuleSceneStageClear::CleanUp()
{
	App->textures->Unload(textures);
	textures = nullptr;
	return true;
}


