#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "ModulePlayer.h"
#include "ModuleEnemies.h"
#include "ModuleInput.h"
#include "ModuleFadeToBlack.h"
#include "ModuleSceneStage2.h"
#include "ModuleCollision.h"
#include "SDL_mixer/include/SDL_mixer.h"
#include "ModuleSceneStage1.h"
#include "ModulePowerUPS.h"


ModuleSceneStage1::ModuleSceneStage1()
{
	for (int i = 0; i < NUM_LAYERS; ++i)
	{
		textures[i] = nullptr;
	}
}

bool ModuleSceneStage1::Init()
{
	startAnimation.PushBack({28, 28, 48, 102});
	startAnimation.PushBack({107, 28, 48, 105});
	startAnimation.PushBack({188, 28, 48, 103});
	startAnimation.PushBack({266, 28, 48, 113});
	startAnimation.PushBack({335, 28, 48, 121});
	startAnimation.PushBack({414, 28, 48, 123});
	startAnimation.PushBack({414, 28, 48, 122});

	startAnimation.PushBack({335, 157, 48, 120});
	startAnimation.PushBack({266, 157, 48, 112});
	startAnimation.PushBack({188, 157, 48, 102});
	startAnimation.PushBack({107, 157, 48, 105});
	startAnimation.PushBack({28, 157, 48, 102});

	startAnimation.loop = false;
	startAnimation.speed = 0.1f;


	return true;
}

bool ModuleSceneStage1::Start() {
	App->current_scene = this;

	App->player->Disable();
	App->collision->Enable();

	right = false;
	up = false;
	down = false;
	left = false;
	shake = false;
	aux = 10;

	textrect[2] = new SDL_Rect();
	textrect[2]->x = 28;
	textrect[2]->y = 28;
	textrect[2]->w = 48;
	textrect[2]->h = 102;

	xInjection = 80;
	yInjection = -100;
	injecting = true;

	startAnimation.setCurrentFrameIndex(0);

	if (!loadMap()) {
		return false;
	}
	//top
	App->enemies->AddEnemy(ENEMY_TYPES::HELLBALL, 450, 60, false);
	App->enemies->AddEnemy(ENEMY_TYPES::HELLBALL, 465, 60, false);
	App->enemies->AddEnemy(ENEMY_TYPES::HELLBALL, 480, 60, false);
	App->enemies->AddEnemy(ENEMY_TYPES::HELLBALL, 495, 60, false);
	App->enemies->AddEnemy(ENEMY_TYPES::HELLBALL, 510, 60, false);
	//bottom
	App->enemies->AddEnemy(ENEMY_TYPES::HELLBALL, 450, 60, true);
	App->enemies->AddEnemy(ENEMY_TYPES::HELLBALL, 465, 60, true);
	App->enemies->AddEnemy(ENEMY_TYPES::HELLBALL, 480, 60, true);
	App->enemies->AddEnemy(ENEMY_TYPES::HELLBALL, 495, 60, true);
	App->enemies->AddEnemy(ENEMY_TYPES::HELLBALL, 510, 60, true);

	App->enemies->AddEnemy(ENEMY_TYPES::FLYINGWORM, 600, 60);
	App->enemies->AddEnemy(ENEMY_TYPES::FLYINGWORM, 630, 100);

	App->enemies->AddEnemy(ENEMY_TYPES::TENTACLESHOOTER, 550, 150);



	music = App->audio->LoadMusic("Assets/Audio/Music/02_Into_the_Human_Body_Stage_1.ogg");
	shipSpawn = App->audio->LoadFx("Assets/Audio/SFX/xmultipl-026.wav");


	Mix_PlayMusic(music, -1);
	Mix_PlayChannel(-1, shipSpawn, 0);

	App->render->camera.x = 0;
	App->render->camera.y = 0;

	App->powerups->AddPowerUp(App->powerups->bomb, 300, 100, COLLIDER_POWER_UP);

	return true;
}

update_status ModuleSceneStage1::Update()
{
	checkCameraEvents();
	updateCamera();
	injection();

	if (shake)
	{
		if (aux == xInjection)
		{
			xInjection--;
		}
		else xInjection++;

		aux = xInjection;
	}

	App->render->Blit(textures[0], 0, 0, textrect[0], 0.5f);
	App->render->Blit(textures[1], 0, 0, textrect[1]);
	App->render->Blit(textures[2], xInjection, yInjection, textrect[2], 0.0001f);
	App->render->Blit(textures[3], 0, 224, textrect[3], 0.0f);


	if (App->input->keyboard[SDL_SCANCODE_RETURN] == 1) App->fade->FadeToBlack(this, App->stage2, 2);

	return update_status::UPDATE_CONTINUE;
}

bool ModuleSceneStage1::CleanUp()
{
	App->player->Disable();
	App->collision->Disable();

	for (int i = 0; i < NUM_LAYERS; ++i)
	{
		App->textures->Unload(textures[i]);
		textures[i] = nullptr;
	}


	App->audio->UnloadMusic(music);
	music = nullptr;
	App->audio->UnloadSFX(shipSpawn);
	shipSpawn = nullptr;
	right = false;

	return true;
}


void ModuleSceneStage1::checkCameraEvents()
{
	if (App->render->camera.x > 8000 && App->render->camera.x < 10304)
	{
		down = true;
	}
	else if (down) down = false;
}

void ModuleSceneStage1::updateCamera()
{
	int speed = 3;

	if (right) {
		App->render->camera.x += speed;
		App->player->position.x += 1;
	}
	if (left)App->render->camera.x -= speed;
	if (up) {
		timer++;
		if (timer >= 3) {
			App->render->camera.y -= speed;
			timer = 0;
		}
	}
	if (down) {
		timer++;
		if (timer >= 3) {
			App->render->camera.y += speed;
			App->player->position.y += 1;
			timer = 0;
		}
	}
}

ModuleSceneStage1::~ModuleSceneStage1()
{
}

bool ModuleSceneStage1::loadMap()
{
	LOG("Loading background textures");
	//Load all background textures
	textures[0] = App->textures->Load("Assets/Sprites/Stages/Stage1/Background/FirstLvlMap.png");
	textures[1] = App->textures->Load("Assets/Sprites/Stages/Stage1/Background/BG01.png");
	textures[2] = App->textures->Load("Assets/Sprites/Stages/Stage1/Background/injection1.png");
	textures[3] = App->textures->Load("Assets/Sprites/UI/UI_1.png");

	textrect[3] = new SDL_Rect();
	textrect[3]->x = 0;
	textrect[3]->y = 0;
	textrect[3]->w = 384;
	textrect[3]->h = 32;


	SDL_Rect coll = { 0,212,2862,12 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	coll = { 496,0,2033,12 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	
	//hand
	coll = { 411,197,50,15 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	coll = { 421,183,20,14 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	//heart
	coll = { 500,188,90,24 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	
	//cut
	coll = { 640,12,74,12 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);

	//hand
	coll = { 763,12,50,15 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	coll = { 773,27,20,14 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);

	//heart
	coll = { 1012,12,90,24 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	//cut
	coll = { 1024,200,74,12 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	//cut
	coll = { 1152,12,74,12 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	//heart
	coll = { 1268,188,90,24 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);

	//door
	coll = { 1425,12,120,24 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	coll = { 1425,188,120,24 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	coll = { 1445,36,100,24 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	coll = { 1445,164,100,24 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	coll = { 1450,60,100,24 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	coll = { 1450,140,100,24 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	coll = { 1480,84,60,12 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	coll = { 1480,128,60,12 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);

	//cut
	coll = { 1664,12,74,12 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	//hand
	coll = { 1787,197,50,15 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	coll = { 1797,183,20,14 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);

	//door
	coll = { 2010,12,200,34 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	coll = { 2010,178,200,34 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	coll = { 2040,46,140,24 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	coll = { 2040,154,140,24 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	coll = { 2010,70,130,24 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	coll = { 2010,130,130,24 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);

	//hand
	coll = { 2427,12,50,15 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	coll = { 2437,27,20,14 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);

	//heart
	coll = { 2548,188,90,24 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);

	//volcano
	coll = { 2638,200,224,12 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	coll = { 2694,182,154,18 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	coll = { 2721,168,40,14 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);

	//volcano
	coll = { 3730,268,224,12 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	coll = { 3736,280,154,18 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	coll = { 3758,298,40,14 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);

	//heart
	coll = { 3956,450,90,24 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	//cut
	coll = { 3970,268,74,12 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	//cut
	coll = { 4224,456,74,12 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	//hand
	coll = { 4347,452,50,15 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	coll = { 4357,438,20,14 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	//heart
	coll = { 4340,275,90,24 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);

	if (textures[0] == nullptr) {
		return false;
	}
	else return true;
}

void ModuleSceneStage1::injection()
{
	if (yInjection == -4 && injecting)
	{
		if (startAnimation.GetCurrentFrameIndex() == 6)
		{
			App->player->Enable();
			right = true;
			injecting = false;
			shake = true;
			aux = xInjection;
		}
		textrect[2] = &startAnimation.GetCurrentFrame();
	}
	else {
		if (!injecting)
		{
			if (startAnimation.isDone())
			{
				yInjection--;
			}
			else {
				textrect[2] = &startAnimation.GetCurrentFrame();
			}
		}
		else yInjection++;
	}
}