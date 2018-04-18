#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleFadeToBlack.h"
#include "ModuleSceneStart.h"
#include "ModuleUI.h"
#include "ModuleFonts.h"
#include "ModuleSceneStageClear.h"



ModuleSceneStageClear::ModuleSceneStageClear()
{

}


ModuleSceneStageClear::~ModuleSceneStageClear()
{
}

bool ModuleSceneStageClear::Start() {

	App->current_scene = this;
	App->render->ResetCamera();


	return true;
}

bool ModuleSceneStageClear::Init()
{

	return true;
}

update_status ModuleSceneStageClear::Update()
{
	if (App->input->keyboard[SDL_SCANCODE_RETURN] == 1) App->fade->FadeToBlack(this, App->start, 2);

	App->fonts->BlitText(120, 120, App->ui->pink_font, "stage 1 cleared");

	return update_status::UPDATE_CONTINUE;
}

bool ModuleSceneStageClear::CleanUp()
{

	return true;
}


