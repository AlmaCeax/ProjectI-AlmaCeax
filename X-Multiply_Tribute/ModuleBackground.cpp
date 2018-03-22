#include "Application.h"
#include "ModuleBackground.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "SDL/include/SDL_Rect.h"


ModuleBackground::ModuleBackground(): Module()
{
	for (int i = 0; i < NUM_LAYERS; ++i)
	{
		textures[i] = nullptr;
		rect[i] = nullptr;
	}
}

bool ModuleBackground::Init()
{
	//rect[0]->x = 0;
	//rect[0]->y = 0;
	//rect[0]->h = 512;
	//rect[0]->w = 512;
	textures[0] = App->texture->Load("../Assets/Sprites/Stages/Stage1/Background/FirstLvlMap1.png");

	if (textures[0] != nullptr) {
		return false;
	}else return true;
}

update_status ModuleBackground::Update()
{
	x += velocity;
	return update_status::UPDATE_CONTINUE;
}

update_status ModuleBackground::PostUpdate()
{
	App->render->Blit(textures[0], x, y, nullptr);
	return update_status::UPDATE_CONTINUE;
}

bool ModuleBackground::CleanUp()
{
	return false;
}


ModuleBackground::~ModuleBackground()
{
}
