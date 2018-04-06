#include "Application.h"
#include "ModuleMap.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "SDL/include/SDL_Rect.h"
#include "SDL_image/include/SDL_image.h"
#include "SDL_mixer/include/SDL_mixer.h"


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

bool ModuleMap::Start() {
	music = App->audio->LoadMusic("Assets/Audio/Music/02_Into_the_Human_Body_Stage_1_.ogg");
	shipSpawn = App->audio->LoadFx("Assets/Audio/SFX/xmultipl-026.wav");
	Mix_PlayMusic(music, -1);
	Mix_PlayChannel(-1, shipSpawn, 0);
	return true;
}

update_status ModuleMap::Update()
{
	return update_status::UPDATE_CONTINUE;
}

update_status ModuleMap::PreUpdate()
{
	App->render->Blit(textures[0], x, y, textrect[0], 0.5f);
	App->render->Blit(textures[1], xLayer, yLayer, textrect[1]);

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
	textures[0] = App->textures->Load("Assets/Sprites/Stages/Stage1/Background/FirstLvlMap.png");
	textures[1] = App->textures->Load("Assets/Sprites/Stages/Stage1/Background/BG01.png");
	//Set origin rectangles in the right coordinates
	textrect[0] = new SDL_Rect();
	textrect[0]->x = 0;
	textrect[0]->y = 0;
	textrect[0]->h = 512;
	textrect[0]->w = 4960;
	textrect[1] = new SDL_Rect();
	textrect[1]->x = 0;
	textrect[1]->y = 0;
	textrect[1]->h = 512;
	textrect[1]->w = 4960;

	if (textures[0] == nullptr) {
		return false;
	}
	else return true;
}