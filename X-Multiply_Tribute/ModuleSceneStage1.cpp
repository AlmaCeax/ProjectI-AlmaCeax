#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "ModuleEnemies.h"
#include "ModuleInput.h"
#include "ModuleFadeToBlack.h"
#include "ModuleParticles.h"
#include "ModuleCollision.h"
#include "ModulePowerUPS.h"
#include "SDL_mixer/include/SDL_mixer.h"
#include "ModuleSceneStage1.h"
#include "ModuleSceneStage3.h"



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

	textrect[0] = { 0,0,4960,512 };
	textrect[1] = { 0,0,4960,512 };
	textrect[2] = { 28,24,48,106 };
	textrect[4] = { 120,159,35,14 };

	score_bonus = 10000;
	index = 1;
	return true;
}

bool ModuleSceneStage1::Start() {
	App->current_stage = this;
	App->player->Disable();
	App->enemies->Enable();
	App->powerups->Enable();
	App->collision->Enable();
	App->powerups->Enable();
	App->particles->Enable();
	App->ui->ui_visible = true;

	right = false;
	up = false;
	down = false;
	left = false;
	stopped = false;

	unhooked = false;
	first_time = true;
	startAnimationHook.setCurrentFrameIndex(0);


	injectionposition.x = 80;
	injectionposition.y = -100;
	injectionhookposition.x = 90;
	injectionhookposition.y = 76;
	injecting = true;

	startAnimation.setCurrentFrameIndex(0);

	if (!loadMap()) {
		return false;
	}
	loadEnemies();
	loadAudio();
	

	App->render->ResetCamera();


	return true;
}


update_status ModuleSceneStage1::Update()
{
	checkCameraEvents();
	updateCamera();

	App->render->Blit(textures[0], 0, 0, &textrect[0], 0.5f);
	App->render->Blit(textures[1], 0, 0, &textrect[1]);
	injection();
	App->render->Blit(textures[2], injectionposition.x, injectionposition.y, &textrect[2],0.5f);

	if (App->input->keyboard[SDL_SCANCODE_F8] == KEY_STATE::KEY_DOWN) App->fade->FadeToBlack(this, App->stage3);

	return update_status::UPDATE_CONTINUE;
}

bool ModuleSceneStage1::CleanUp()
{
	App->player->Disable();
	App->enemies->Disable();
	App->collision->Disable();
	App->powerups->Disable();
	App->particles->Disable();

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

	if (App->render->camera.x >= (4960 - SCREEN_WIDTH) * SCREEN_SIZE && !stopped) {
		stopped = true;
		App->ui->StageCleared();
	}
}

void ModuleSceneStage1::updateCamera()
{
	int speed = 1;
	if (stopped || !App->player->canMove) return;

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


	coll = { 0,212,2862,12 };
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
	coll = { 2920,0,80,30 };
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
		y += 15;
	}

	// line down stage
	coll = { 3808,256,1152,12 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	coll = { 3552,468,1408,12 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);

	//volcano
	coll = { 3730,268,224,30 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
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
	
void ModuleSceneStage1::loadEnemies()
{
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

	App->enemies->AddEnemy(ENEMY_TYPES::TENTACLESHOOTER, 530, 150);

	App->enemies->AddEnemy(ENEMY_TYPES::FLYINGWORM, 600, 60);
	App->enemies->AddEnemy(ENEMY_TYPES::FLYINGWORM, 615, 80);

	App->enemies->AddEnemy(ENEMY_TYPES::FLYINGWORM, 720, 80);
	App->enemies->AddEnemy(ENEMY_TYPES::FLYINGWORM, 720, 120);

	App->enemies->AddEnemy(ENEMY_TYPES::POWERUPSHIP, 750, 140, false, 3);

	App->enemies->AddEnemy(ENEMY_TYPES::HELLBALL, 770, 90, false);
	App->enemies->AddEnemy(ENEMY_TYPES::HELLBALL, 785, 90, false);
	App->enemies->AddEnemy(ENEMY_TYPES::HELLBALL, 800, 90, false);
	App->enemies->AddEnemy(ENEMY_TYPES::HELLBALL, 815, 90, false);
	App->enemies->AddEnemy(ENEMY_TYPES::HELLBALL, 830, 90, false);
	App->enemies->AddEnemy(ENEMY_TYPES::HELLBALL, 845, 90, false);
	App->enemies->AddEnemy(ENEMY_TYPES::HELLBALL, 860, 90, false);
	//bottom
	App->enemies->AddEnemy(ENEMY_TYPES::HELLBALL, 770, 90, true);
	App->enemies->AddEnemy(ENEMY_TYPES::HELLBALL, 785, 90, true);
	App->enemies->AddEnemy(ENEMY_TYPES::HELLBALL, 800, 90, true);
	App->enemies->AddEnemy(ENEMY_TYPES::HELLBALL, 815, 90, true);
	App->enemies->AddEnemy(ENEMY_TYPES::HELLBALL, 830, 90, true);
	App->enemies->AddEnemy(ENEMY_TYPES::HELLBALL, 845, 90, true);
	App->enemies->AddEnemy(ENEMY_TYPES::HELLBALL, 860, 90, true);

	App->enemies->AddEnemy(ENEMY_TYPES::TENTACLESHOOTER, 900, 175);

	App->enemies->AddEnemy(ENEMY_TYPES::POWERUPSHIP, 1100, 100, false, 2);
	App->enemies->AddEnemy(ENEMY_TYPES::POWERUPSHIP, 1130, 120, false, 1);

	App->enemies->AddEnemy(ENEMY_TYPES::FLYINGWORM, 1160, 50);
	App->enemies->AddEnemy(ENEMY_TYPES::FLYINGWORM, 1200, 60);
	App->enemies->AddEnemy(ENEMY_TYPES::FLYINGWORM, 1200, 100);

	App->enemies->AddEnemy(ENEMY_TYPES::TENTACLESHOOTER, 1315, 12, true);
	App->enemies->AddEnemy(ENEMY_TYPES::TENTACLESHOOTER, 1290, 150);

	App->enemies->AddEnemy(ENEMY_TYPES::TENTACLESHOOTER, 2270, 175);
	App->enemies->AddEnemy(ENEMY_TYPES::TENTACLESHOOTER, 2305, 175);
	App->enemies->AddEnemy(ENEMY_TYPES::TENTACLESHOOTER, 2400, 175);
	App->enemies->AddEnemy(ENEMY_TYPES::TENTACLESHOOTER, 2485, 175);

	App->enemies->AddEnemy(ENEMY_TYPES::TENTACLESHOOTER, 3110, 60, true);
	App->enemies->AddEnemy(ENEMY_TYPES::TENTACLESHOOTER, 3500, 185, true);
	App->enemies->AddEnemy(ENEMY_TYPES::TENTACLESHOOTER, 3670, 245, true);

	App->enemies->AddEnemy(ENEMY_TYPES::FLYINGWORM, 3240, 170);
	App->enemies->AddEnemy(ENEMY_TYPES::FLYINGWORM, 3240, 210);
	App->enemies->AddEnemy(ENEMY_TYPES::FLYINGWORM, 3260, 220);
	App->enemies->AddEnemy(ENEMY_TYPES::FLYINGWORM, 3260, 250);
	App->enemies->AddEnemy(ENEMY_TYPES::FLYINGWORM, 3300, 150);
	App->enemies->AddEnemy(ENEMY_TYPES::FLYINGWORM, 3400, 270);
	App->enemies->AddEnemy(ENEMY_TYPES::FLYINGWORM, 3460, 270);
	App->enemies->AddEnemy(ENEMY_TYPES::FLYINGWORM, 3460, 330);
	App->enemies->AddEnemy(ENEMY_TYPES::FLYINGWORM, 3480, 270);
	App->enemies->AddEnemy(ENEMY_TYPES::FLYINGWORM, 3500, 270);
	App->enemies->AddEnemy(ENEMY_TYPES::FLYINGWORM, 3500, 330);
	App->enemies->AddEnemy(ENEMY_TYPES::FLYINGWORM, 3520, 330);
	App->enemies->AddEnemy(ENEMY_TYPES::FLYINGWORM, 3540, 270);
	App->enemies->AddEnemy(ENEMY_TYPES::FLYINGWORM, 3540, 330);
	App->enemies->AddEnemy(ENEMY_TYPES::FLYINGWORM, 3560, 230);
	App->enemies->AddEnemy(ENEMY_TYPES::FLYINGWORM, 3560, 270);
	App->enemies->AddEnemy(ENEMY_TYPES::FLYINGWORM, 3580, 230);
	App->enemies->AddEnemy(ENEMY_TYPES::FLYINGWORM, 3580, 270);
	App->enemies->AddEnemy(ENEMY_TYPES::FLYINGWORM, 3600, 230);
	App->enemies->AddEnemy(ENEMY_TYPES::FLYINGWORM, 3600, 270);
	App->enemies->AddEnemy(ENEMY_TYPES::FLYINGWORM, 3600, 310);
	App->enemies->AddEnemy(ENEMY_TYPES::FLYINGWORM, 3620, 230);
	App->enemies->AddEnemy(ENEMY_TYPES::FLYINGWORM, 3620, 270);
	App->enemies->AddEnemy(ENEMY_TYPES::FLYINGWORM, 3620, 310);

}

void ModuleSceneStage1::loadAudio()
{
	music = App->audio->LoadMusic("Assets/Audio/Music/02_Into_the_Human_Body_Stage_1.ogg");
	shipSpawn = App->audio->LoadFx("Assets/Audio/SFX/xmultipl-026.wav");
}

void ModuleSceneStage1::injection()
{
	if (first_time) {
		if (injectionposition.y >= 0 && injecting)
		{
			if (startAnimation.GetCurrentFrameIndex() == 3)
			{
				startAnimation.hold = true;
				injecting = false;
			}
			textrect[2] = startAnimation.GetCurrentFrame();
		}
		else {
			if (!injecting)
			{
				if (startAnimation.isDone())
				{
					injectionposition.y--;
				}
				else {
					textrect[2] = startAnimation.GetCurrentFrame();
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
							if(!Mix_PlayingMusic()) Mix_PlayMusic(music, -1);
							right = true;
							App->player->injecting = false;
							App->player->startBoost = true;
						}
						if (startAnimationHook.isDone())unhooked = true;
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
			else { 
				if (!Mix_Playing(0)) Mix_PlayChannel(-1, shipSpawn, 0); 
				injectionposition.y++; 
			}
		}
	}
}

void ModuleSceneStage1::OnFade() {
	ModuleStage::OnFade();
	if (App->ui->is_continue) App->ui->PlayerReady();
}