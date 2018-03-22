#include "Application.h"
#include "ModuleMap.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "SDL/include/SDL_Rect.h"
#include "SDL_image/include/SDL_image.h"


ModuleMap::ModuleMap(): Module()
{
	for (int i = 0; i < NUM_LAYERS; ++i)
	{
		textures[i] = nullptr;
		rect[i] = nullptr;
	}
}

bool ModuleMap::Init()
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

update_status ModuleMap::Update()
{
	x += velocity;
	return update_status::UPDATE_CONTINUE;
}

update_status ModuleMap::PostUpdate()
{
	App->render->Blit(textures[0], x, y, nullptr);
	return update_status::UPDATE_CONTINUE;
}

bool ModuleMap::CleanUp()
{
	for (int i = NUM_LAYERS; i < 1; --i)
	{
		if(textures[i] != nullptr)SDL_DestroyTexture(textures[i]);
		delete rect[i];
	}
	return false;
}


ModuleMap::~ModuleMap()
{
}
