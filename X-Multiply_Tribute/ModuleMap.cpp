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
	if (!loadMapTextures()) {
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
	int nextX, nextXLayer, nextYLayer = 0;
	x -= xVelocity;
	xLayer -= velocityLayer;
	yLayer -= yVelocityLayer;
	if (x <= -backgroundwidth) x = 0;

	App->render->Blit(textures[0], x, y, textrect[0]);
	nextX = x + backgroundwidth;
	App->render->Blit(textures[0], nextX, y, textrect[0]);

	App->render->Blit(textures[indexLayer], xLayer, yLayer, textrect[1]);
	nextXLayer = xLayer + backgroundwidth;
	App->render->Blit(textures[indexLayer+1], nextXLayer, nextYLayer, textrect[1]);
	
	if (xLayer <= -backgroundwidth) {
		xLayer = 0;
		indexLayer++;
		switch (indexLayer)
		{
			case 7: yVelocityLayer = 1; break;
			case 8: yVelocityLayer = 0; break;
			default: 
				break;
		}
	}

	//if (indexLayer == 7 || indexLayer == 8)
	//{
	//	nextYLayer = xLayer + 240;
	//	if (yLayer <= -240)
	//	{
	//		yLayer = 0;
	//	}
	//}

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

bool ModuleMap::loadMapTextures()
{
	LOG("Loading background textures");
	//Load all background textures
	textures[0] = App->textures->Load("Assets/Sprites/Stages/Stage1/Background/FirstLvlMap1.png");
	textures[1] = App->textures->Load("Assets/Sprites/Stages/Stage1/Background/Tilemap/Map1.png");
	textures[2] = App->textures->Load("Assets/Sprites/Stages/Stage1/Background/Tilemap/Map2.png");
	textures[3] = App->textures->Load("Assets/Sprites/Stages/Stage1/Background/Tilemap/Map3.png");
	textures[4] = App->textures->Load("Assets/Sprites/Stages/Stage1/Background/Tilemap/Map4.png");
	textures[5] = App->textures->Load("Assets/Sprites/Stages/Stage1/Background/Tilemap/Map5.png");
	textures[6] = App->textures->Load("Assets/Sprites/Stages/Stage1/Background/Tilemap/Map6.png");
	textures[7] = App->textures->Load("Assets/Sprites/Stages/Stage1/Background/Tilemap/Map7.png");
	textures[8] = App->textures->Load("Assets/Sprites/Stages/Stage1/Background/Tilemap/Map8.png");
	textures[9] = App->textures->Load("Assets/Sprites/Stages/Stage1/Background/Tilemap/Map9.png");
	//Set origin rectangles in the right coordinates
	textrect[0] = new SDL_Rect();
	textrect[0]->x = 0;
	textrect[0]->y = 129;
	textrect[0]->h = 240;
	textrect[0]->w = 512;
	textrect[1] = new SDL_Rect();
	textrect[1]->x = 0;
	textrect[1]->y = 0;
	textrect[1]->h = 240;
	textrect[1]->w = 512;

	if (textures[0] == nullptr) {
		return false;
	}
	else return true;
}