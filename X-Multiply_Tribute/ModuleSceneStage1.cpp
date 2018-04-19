#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "ModulePlayer.h"
#include "ModuleEnemies.h"
#include "ModuleInput.h"
#include "ModuleFadeToBlack.h"
#include "ModuleSceneStage2.h"
#include "ModuleSceneStageClear.h"
#include "ModuleParticles.h"
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
	startAnimation.PushBack({28, 24, 48, 106});
	startAnimation.PushBack({107, 24, 48, 105});
	startAnimation.PushBack({188, 24, 48, 103});
	startAnimation.PushBack({188, 24, 48, 103});
	startAnimation.PushBack({107, 24, 48, 105});
	startAnimation.PushBack({28, 24, 48, 106});

	startAnimation.loop = false;
	startAnimation.speed = 0.1f;

	startAnimationHook.PushBack({28, 156, 32 ,29});
	startAnimationHook.PushBack({71, 156, 32 ,24});
	startAnimationHook.PushBack({28, 156, 32 ,29 });
	startAnimationHook.loop = false;
	startAnimationHook.speed = 0.08f;


	return true;
}

bool ModuleSceneStage1::Start() {
	App->current_scene = this;
	App->player->Disable();
	App->particles->Enable();
	App->enemies->Enable();
	App->powerups->Enable();
	App->collision->Enable();

	right = false;
	up = false;
	down = false;
	left = false;
	shake = false;
	stopped = false;
	aux = 10;
	unhooked = false;
	startAnimationHook.setCurrentFrameIndex(0);


	textrect[2] = new SDL_Rect();
	textrect[2]->x = 28;
	textrect[2]->y = 24;
	textrect[2]->w = 48;
	textrect[2]->h = 106;

	textrect[4] = new SDL_Rect();
	textrect[4]->x = 120;
	textrect[4]->y = 159;
	textrect[4]->w = 35;
	textrect[4]->h = 14;

	injectionposition.x = 80;
	injectionposition.y = -100;
	injectionhookposition.x = 90;
	injectionhookposition.y = 76;
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
	App->enemies->AddEnemy(ENEMY_TYPES::HELLBALL, 525, 60, false);
	App->enemies->AddEnemy(ENEMY_TYPES::HELLBALL, 540, 60, false);
	//bottom
	App->enemies->AddEnemy(ENEMY_TYPES::HELLBALL, 450, 60, true);
	App->enemies->AddEnemy(ENEMY_TYPES::HELLBALL, 465, 60, true);
	App->enemies->AddEnemy(ENEMY_TYPES::HELLBALL, 480, 60, true);
	App->enemies->AddEnemy(ENEMY_TYPES::HELLBALL, 495, 60, true);
	App->enemies->AddEnemy(ENEMY_TYPES::HELLBALL, 510, 60, true);
	App->enemies->AddEnemy(ENEMY_TYPES::HELLBALL, 525, 60, true);
	App->enemies->AddEnemy(ENEMY_TYPES::HELLBALL, 540, 60, true);

	App->enemies->AddEnemy(ENEMY_TYPES::FLYINGWORM, 600, 60);
	App->enemies->AddEnemy(ENEMY_TYPES::FLYINGWORM, 630, 100);

	App->enemies->AddEnemy(ENEMY_TYPES::TENTACLESHOOTER, 530, 150);



	music = App->audio->LoadMusic("Assets/Audio/Music/02_Into_the_Human_Body_Stage_1.ogg");
	shipSpawn = App->audio->LoadFx("Assets/Audio/SFX/xmultipl-026.wav");



	Mix_PlayChannel(-1, shipSpawn, 0);

	App->render->ResetCamera();

	App->enemies->AddEnemy(ENEMY_TYPES::POWERUPSHIP, 470, 100, false, 3);
	App->enemies->AddEnemy(ENEMY_TYPES::POWERUPSHIP, 450, 100, false, 2);


	return true;
}

update_status ModuleSceneStage1::Update()
{
	checkCameraEvents();
	updateCamera();

	App->render->Blit(textures[0], 0, 0, textrect[0], 0.5f);
	App->render->Blit(textures[1], 0, 0, textrect[1]);
	injection();
	App->render->Blit(textures[2], injectionposition.x, injectionposition.y, textrect[2],0.5f);
	App->render->Blit(textures[3], 0, 224, textrect[3], 0.0f);

	if (App->input->keyboard[SDL_SCANCODE_RETURN] == KEY_STATE::KEY_DOWN) App->fade->FadeToBlack(this, App->stage2);
	if (App->input->keyboard[SDL_SCANCODE_F3] == KEY_STATE::KEY_DOWN) App->fade->FadeToBlack(this, App->stage_clear);


	return update_status::UPDATE_CONTINUE;
}

bool ModuleSceneStage1::CleanUp()
{
	App->player->Disable();
	App->enemies->Disable();
	App->particles->Disable();
	App->powerups->Disable();
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
	if (App->render->camera.x > 2666 * SCREEN_SIZE && App->render->camera.x < 3434 * SCREEN_SIZE)
	{
		down = true;
	}
	else if (down) down = false;

	if (App->render->camera.x >= (4960 - SCREEN_WIDTH) * SCREEN_SIZE) {
		stopped = true;
		App->fade->FadeToBlack(this, App->stage_clear);
	}
}

void ModuleSceneStage1::updateCamera()
{
	int speed = 1;
	if (stopped || App->player->dead) return;

	if (right) {
		App->render->camera.x += speed * SCREEN_SIZE;
		App->player->position.x += speed;
	}
	if (left)App->render->camera.x -= speed * SCREEN_SIZE;
	if (up) {
		timer++;
		if (timer >= 3) {
			App->render->camera.y -= speed * SCREEN_SIZE;
			timer = 0;
		}
	}
	if (down) {
		timer++;
		if (timer >= 3) {
			App->render->camera.y += speed * SCREEN_SIZE;
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
	textures[2] = App->textures->Load("Assets/Sprites/Stages/Stage1/Background/injection.png");
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


	//diagonal
	coll = { 2920,0,80,50 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	int y = 236;
	for (int x = 2850; x < 3552; x += 50)
	{
		coll = { x,y,50,50 };
		App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
		y += 17;
	}
	y = 0;
	for (int x = 3000; x < 3700; x += 50)
	{
		coll = { x,y,50,50 };
		App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
		y += 17;
	}

	// line down stage
	coll = { 3808,256,1152,12 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	coll = { 3552,468,1408,12 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);

	//volcano
	coll = { 3730,298,224,12 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	coll = { 3744,310,154,18 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	coll = { 3840,328,40,14 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);

	//heart
	coll = { 3956,444,90,24 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	//cut
	coll = { 3970,268,74,12 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	//cut
	coll = { 4224,456,74,12 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	//hand
	coll = { 4347,453,50,15 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	coll = { 4357,439,20,14 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	//heart
	coll = { 4340,268,90,24 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);

	//door
	coll = { 4496,268,120,24 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	coll = { 4496,444,120,24 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	coll = { 4516,292,100,24 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	coll = { 4516,420,100,24 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	coll = { 4521,316,100,24 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	coll = { 4521,396,100,24 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	coll = { 4551,340,60,12 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	coll = { 4551,384,60,12 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);

	if (textures[0] == nullptr) {
		return false;
	}
	else return true;
}
	

void ModuleSceneStage1::injection()
{
	if (injectionposition.y >= 0 && injecting)
	{
		if (startAnimation.GetCurrentFrameIndex() == 3)
		{
			startAnimation.hold = true;
			injecting = false;
		}
		textrect[2] = &startAnimation.GetCurrentFrame();
	}
	else {
		if (!injecting)
		{
			if (startAnimation.isDone())
			{
				injectionposition.y--;
			}else {
				textrect[2] = &startAnimation.GetCurrentFrame();
				if (injectionhookposition.y < 96 && !unhooked)
				{
					App->player->Enable();
					App->player->position.y = injectionhookposition.y + 16;
					startAnimationHook.hold = true;
					injectionhookposition.y++;
					//App->render->Blit(textures[2], injectionhookposition.x, injectionhookposition.y+16, textrect[4]);

				}
				else {
					startAnimationHook.hold = false;
					if (startAnimationHook.GetCurrentFrameIndex() == 1) {
						Mix_PlayMusic(music, -1);
						right = true;
						App->player->injecting = false;
						App->player->startBoost = true;
						App->particles->AddParticle(App->particles->playerBoost, App->player->position.x - 42, App->player->position.y, COLLIDER_NONE, { 3,0 });;
					}
					if(startAnimationHook.isDone())unhooked = true;
				}

				if (startAnimationHook.isDone())
				{
					if (injectionhookposition.y < 76)
					{
						startAnimation.hold = false;
					}
				}
				App->render->Blit(textures[2], injectionhookposition.x, injectionhookposition.y, &startAnimationHook.GetCurrentFrame(), 0.5f);
			}
			if (unhooked)injectionhookposition.y--;
		}
		else injectionposition.y++;
	}
}