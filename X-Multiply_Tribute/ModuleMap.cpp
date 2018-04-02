#include "Application.h"
#include "ModuleMap.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "SDL/include/SDL_Rect.h"
#include "SDL/include/SDL_Render.h"
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
	LOG("Loading background textures");
	textures[0] = App->textures->Load("Assets/Sprites/Stages/Stage1/Background/FirstLvlMap1.png");
	textrect[0] = new SDL_Rect();
	textrect[0]->x = 0;
	textrect[0]->y = 129;
	textrect[0]->h = 240;
	textrect[0]->w = 512;

	if (textures[0] == nullptr) {
		return false;
	}else return true;
}

update_status ModuleMap::Update()
{
	return update_status::UPDATE_CONTINUE;
}

update_status ModuleMap::PostUpdate()
{
	return update_status::UPDATE_CONTINUE;
}

update_status ModuleMap::PreUpdate()
{
	int nextX;
	x -= velocity;
	if (x <= -backgroundwidth) x = 0;

	App->render->Blit(textures[0], x, y, textrect[0]);
	nextX = x + backgroundwidth;
	App->render->Blit(textures[0], nextX, y, textrect[0]);

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
