#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleFadeToBlack.h"
#include "ModuleSceneStart.h"
#include "ModuleUI.h"
#include "ModulePlayer.h"
#include "ModuleRender.h"
#include "ModuleFonts.h"
#include "ModuleSceneStageClear.h"
#include <math.h>


ModuleSceneStageClear::ModuleSceneStageClear()
{

}


ModuleSceneStageClear::~ModuleSceneStageClear()
{
}


bool ModuleSceneStageClear::Start() {

	App->current_scene = this;
	App->render->ResetCamera();

	App->player->position.x = (int)(-App->render->camera.x) + (App->player->position.x * SCREEN_SIZE);
	App->player->position.y = (int)(-App->render->camera.y) + App->player->position.y * SCREEN_SIZE;



	return true;
}

bool ModuleSceneStageClear::Init()
{

	return true;
}

update_status ModuleSceneStageClear::Update()
{
	if (moving_player) MovePlayer();

	return update_status::UPDATE_CONTINUE;
}

bool ModuleSceneStageClear::CleanUp()
{

	return true;
}

void ModuleSceneStageClear::MovePlayer() {

	distance = { (SCREEN_WIDTH*SCREEN_SIZE / 2) - App->player->position.x, 50 - App->player->position.y };
	int angle = atan2(distance.y, distance.x);
		

	App->player->position.x += cos(angle);
	App->player->position.y += sin(angle);

	App->player->BlitPlayer();

	App->ui->StageCleared();
}


