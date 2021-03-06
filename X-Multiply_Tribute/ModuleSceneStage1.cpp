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
#include "SDL/include/SDL_timer.h"
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
	first_time = true;

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
	if (!first_time) App->ui->PlayerReady();;


	return true;
}


update_status ModuleSceneStage1::Update()
{
	checkCameraEvents();
	updateCamera();
	CheckpointsUpdate();
	BossUpdate();

	App->render->Blit(textures[0], 0, 0, &textrect[0], 0.5f);
	App->render->Blit(textures[1], 0, 0, &textrect[1]);
	injection();
	App->render->Blit(textures[2], injectionposition.x, injectionposition.y, &textrect[2],0.5f);

	if (App->input->keyboard[SDL_SCANCODE_F8] == KEY_STATE::KEY_DOWN) {
		App->fade->FadeToBlack(this, App->stage3, 0.0f);
		first_time = false;
		App->ui->current_checkpoint = 0;
	}

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
	App->audio->UnloadMusic(bossTrack);
	bossTrack = nullptr;
	App->audio->UnloadSFX(shipSpawn);
	shipSpawn = nullptr;

	right = false;

	return true;
}


void ModuleSceneStage1::checkCameraEvents()
{
	if (App->render->camera.x > 2666 * SCREEN_SIZE && App->render->camera.x < 3436 * SCREEN_SIZE)
	{
		down = true;
	}
	else if (down) down = false;

	if (App->render->camera.x >= (4960 - SCREEN_WIDTH) * SCREEN_SIZE && !stopped) {
		stopped = true;
		start_time = SDL_GetTicks();
		total_time = (Uint32)(4.0f * 1000.0f);
		Mix_PlayMusic(bossTrack, false);

		current_phase = boss_phases::boss_spawn;
	}

	
	
	if (App->render->camera.x == 1700 * SCREEN_SIZE)
	{
		right = false;
		wallboss_timer++;
		if (wallboss_timer == 2000 || wallboss) {
			right = true;
			wallboss = false;
		}
	}
	
}

void ModuleSceneStage1::updateCamera()
{
	int speed = 1;
	if (stopped || !App->player->canMove) return;

	if (right) {
		App->render->camera.x += speed * SCREEN_SIZE;
		App->player->position.x += speed*SCREEN_SIZE;
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

	checkpoints[0] = {0,0};
	checkpoints[1] = { 1326,0 };
	checkpoints[2] = { 2441,0 };
	checkpoints[3] = { 3800,256 };
	checkpoints[4] = { -1,-1 };


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

	App->enemies->AddEnemy(ENEMY_TYPES::POWERUPSHIP, 700, 140, false, 0, 3);

	App->enemies->AddEnemy(ENEMY_TYPES::FLYINGWORM, 720, 80);
	App->enemies->AddEnemy(ENEMY_TYPES::FLYINGWORM, 720, 120);

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

	App->enemies->AddEnemy(ENEMY_TYPES::POWERUPSHIP, 1090, 120, false,0, 2);
	App->enemies->AddEnemy(ENEMY_TYPES::POWERUPSHIP, 1130, 90, false, 0, 7);

	App->enemies->AddEnemy(ENEMY_TYPES::ALIEN, 1100, 156);

	App->enemies->AddEnemy(ENEMY_TYPES::POWERUPSHIP, 1250, 130, false, 0, 1);

	App->enemies->AddEnemy(ENEMY_TYPES::FLYINGWORM, 1160, 50);
	App->enemies->AddEnemy(ENEMY_TYPES::FLYINGWORM, 1200, 60);
	App->enemies->AddEnemy(ENEMY_TYPES::FLYINGWORM, 1200, 100);

	App->enemies->AddEnemy(ENEMY_TYPES::TENTACLESHOOTER, 1315, 12, true);
	App->enemies->AddEnemy(ENEMY_TYPES::TENTACLESHOOTER, 1290, 150);

	App->enemies->AddEnemy(ENEMY_TYPES::ALIEN, 1600, 156);

	App->enemies->AddEnemy(ENEMY_TYPES::WALL, 1865, 25);

	App->enemies->AddEnemy(ENEMY_TYPES::TENTACLESHOOTER, 2270, 175);
	App->enemies->AddEnemy(ENEMY_TYPES::TENTACLESHOOTER, 2305, 175);
	App->enemies->AddEnemy(ENEMY_TYPES::TENTACLESHOOTER, 2400, 175);
	App->enemies->AddEnemy(ENEMY_TYPES::TENTACLESHOOTER, 2485, 175);

	App->enemies->AddEnemy(ENEMY_TYPES::POWERUPSHIP, 2500, 120, false, 0, 3);

	App->enemies->AddEnemy(ENEMY_TYPES::BLUEMOUTH, 2726, 164, true);
	App->enemies->AddEnemy(ENEMY_TYPES::BLUEMOUTH, 2751, 189, true);
	App->enemies->AddEnemy(ENEMY_TYPES::BLUEMOUTH, 2795, 181, true);
	App->enemies->AddEnemy(ENEMY_TYPES::BLUEMOUTH, 2807, 191, true);

	App->enemies->AddEnemy(ENEMY_TYPES::TENTACLESHOOTER, 3110, 60, true);

	App->enemies->AddEnemy(ENEMY_TYPES::POWERUPSHIP, 3200, 130, false, 0, 7);

	App->enemies->AddEnemy(ENEMY_TYPES::TENTACLESHOOTER, 3500, 185, true);

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

	App->enemies->AddEnemy(ENEMY_TYPES::TENTACLESHOOTER, 3670, 245, true);

	App->enemies->AddEnemy(ENEMY_TYPES::BLUEMOUTH, 3740, 286, false);
	App->enemies->AddEnemy(ENEMY_TYPES::BLUEMOUTH, 3752, 296, false);
	App->enemies->AddEnemy(ENEMY_TYPES::BLUEMOUTH, 3796, 294, false);
	App->enemies->AddEnemy(ENEMY_TYPES::BLUEMOUTH, 3821, 319, false);
	
	

	App->enemies->AddEnemy(ENEMY_TYPES::POWERUPSHIP, 3800, 350, false, 0, 1);

	App->enemies->AddEnemy(ENEMY_TYPES::GREENEYE, 4312, 259, false, 5);
	App->enemies->AddEnemy(ENEMY_TYPES::GREENEYE, 4312, 280, false, 5);
	App->enemies->AddEnemy(ENEMY_TYPES::GREENEYE, 4312, 394, true, 5);
	App->enemies->AddEnemy(ENEMY_TYPES::GREENEYE, 4312, 427, true, 5);
	App->enemies->AddEnemy(ENEMY_TYPES::GREENEYE, 4212, 259, false, 3);
	App->enemies->AddEnemy(ENEMY_TYPES::GREENEYE, 4212, 427, true, 3);

	App->enemies->AddEnemy(ENEMY_TYPES::GREENEYE, 4322, 259, false, 5);
	App->enemies->AddEnemy(ENEMY_TYPES::GREENEYE, 4322, 280, false, 5);
	App->enemies->AddEnemy(ENEMY_TYPES::GREENEYE, 4322, 394, true, 5);
	App->enemies->AddEnemy(ENEMY_TYPES::GREENEYE, 4322, 427, true, 5);
	App->enemies->AddEnemy(ENEMY_TYPES::GREENEYE, 4222, 259, false, 3);
	App->enemies->AddEnemy(ENEMY_TYPES::GREENEYE, 4222, 427, true, 3);

	App->enemies->AddEnemy(ENEMY_TYPES::GREENEYE, 4332, 259, false, 5);
	App->enemies->AddEnemy(ENEMY_TYPES::GREENEYE, 4332, 280, false, 5);
	App->enemies->AddEnemy(ENEMY_TYPES::GREENEYE, 4332, 394, true, 5);
	App->enemies->AddEnemy(ENEMY_TYPES::GREENEYE, 4332, 427, true, 5);
	App->enemies->AddEnemy(ENEMY_TYPES::GREENEYE, 4232, 259, false, 3);
	App->enemies->AddEnemy(ENEMY_TYPES::GREENEYE, 4232, 427, true, 3);

	App->enemies->AddEnemy(ENEMY_TYPES::GREENEYE, 4342, 259, false, 5);
	App->enemies->AddEnemy(ENEMY_TYPES::GREENEYE, 4342, 280, false, 5);
	App->enemies->AddEnemy(ENEMY_TYPES::GREENEYE, 4342, 394, true, 5);
	App->enemies->AddEnemy(ENEMY_TYPES::GREENEYE, 4342, 427, true, 5);
	App->enemies->AddEnemy(ENEMY_TYPES::GREENEYE, 4242, 259, false, 3);
	App->enemies->AddEnemy(ENEMY_TYPES::GREENEYE, 4242, 427, true, 3);

	App->enemies->AddEnemy(ENEMY_TYPES::GREENEYE, 4312, 279, false, 5);
	App->enemies->AddEnemy(ENEMY_TYPES::GREENEYE, 4312, 300, false, 5);
	App->enemies->AddEnemy(ENEMY_TYPES::GREENEYE, 4312, 414, true, 5);
	App->enemies->AddEnemy(ENEMY_TYPES::GREENEYE, 4312, 447, true, 5);
	App->enemies->AddEnemy(ENEMY_TYPES::GREENEYE, 4212, 279, false, 3);
	App->enemies->AddEnemy(ENEMY_TYPES::GREENEYE, 4212, 447, true, 3);

	App->enemies->AddEnemy(ENEMY_TYPES::GREENEYE, 4322, 279, false, 5);
	App->enemies->AddEnemy(ENEMY_TYPES::GREENEYE, 4322, 300, false, 5);
	App->enemies->AddEnemy(ENEMY_TYPES::GREENEYE, 4322, 414, true, 5);
	App->enemies->AddEnemy(ENEMY_TYPES::GREENEYE, 4322, 447, true, 5);
	App->enemies->AddEnemy(ENEMY_TYPES::GREENEYE, 4222, 279, false, 3);
	App->enemies->AddEnemy(ENEMY_TYPES::GREENEYE, 4222, 447, true, 3);

	App->enemies->AddEnemy(ENEMY_TYPES::GREENEYE, 4332, 279, false, 5);
	App->enemies->AddEnemy(ENEMY_TYPES::GREENEYE, 4332, 300, false, 5);
	App->enemies->AddEnemy(ENEMY_TYPES::GREENEYE, 4332, 414, true, 5);
	App->enemies->AddEnemy(ENEMY_TYPES::GREENEYE, 4332, 447, true, 5);
	App->enemies->AddEnemy(ENEMY_TYPES::GREENEYE, 4232, 279, false, 3);
	App->enemies->AddEnemy(ENEMY_TYPES::GREENEYE, 4232, 447, true, 3);

	App->enemies->AddEnemy(ENEMY_TYPES::GREENEYE, 4342, 279, false, 5);
	App->enemies->AddEnemy(ENEMY_TYPES::GREENEYE, 4342, 300, false, 5);
	App->enemies->AddEnemy(ENEMY_TYPES::GREENEYE, 4342, 414, true, 5);
	App->enemies->AddEnemy(ENEMY_TYPES::GREENEYE, 4342, 447, true, 5);
	App->enemies->AddEnemy(ENEMY_TYPES::GREENEYE, 4242, 279, false, 3);
	App->enemies->AddEnemy(ENEMY_TYPES::GREENEYE, 4242, 447, true, 3);

	App->enemies->AddEnemy(ENEMY_TYPES::BROWNEYE, 4413, 275);
	App->enemies->AddEnemy(ENEMY_TYPES::BROWNEYE, 4413, 340);
	App->enemies->AddEnemy(ENEMY_TYPES::BROWNEYE, 4477, 340);
	App->enemies->AddEnemy(ENEMY_TYPES::BROWNEYE, 4413, 410);

	App->enemies->AddEnemy(ENEMY_TYPES::JUMPER, 2322, 22);
	App->enemies->AddEnemy(ENEMY_TYPES::JUMPER, 2372, 22);
	App->enemies->AddEnemy(ENEMY_TYPES::JUMPER, 2478, 130);
	App->enemies->AddEnemy(ENEMY_TYPES::JUMPER, 2552, 113);
	App->enemies->AddEnemy(ENEMY_TYPES::JUMPER, 3161, 277, false,0, -1, false);
}

void ModuleSceneStage1::loadAudio()
{
	music = App->audio->LoadMusic("Assets/Audio/Music/02_Into_the_Human_Body_Stage_1.ogg");
	bossTrack = App->audio->LoadMusic("Assets/Audio/Music/03_Boss_Theme.ogg");
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
					if (injectionposition.y <= -100) first_time = false;
				}
				else {
					textrect[2] = startAnimation.GetCurrentFrame();
					if (injectionhookposition.y < 96 && !unhooked)
					{
						App->player->Enable();
						App->player->position.x = 85;
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

void ModuleSceneStage1::BossUpdate() {
	Uint32 now = SDL_GetTicks() - start_time;
	switch (current_phase) {
	case boss_phases::boss_spawn: {
		if (ball_timer == 0) {
			App->particles->AddParticle(App->particles->redBall, 4897, 457, COLLIDER_ENEMY_DESTRUCTIBLE_SHOT, { 0.5,-5 });
			App->particles->AddParticle(App->particles->redBall, 4897, 457, COLLIDER_ENEMY_DESTRUCTIBLE_SHOT, { -0.5,-5 });
			App->particles->AddParticle(App->particles->redBall, 4897, 457, COLLIDER_ENEMY_DESTRUCTIBLE_SHOT, { 1,-5 });
			App->particles->AddParticle(App->particles->redBall, 4897, 457, COLLIDER_ENEMY_DESTRUCTIBLE_SHOT, { -1,-5 });
			App->particles->AddParticle(App->particles->redBall, 4897, 457, COLLIDER_ENEMY_DESTRUCTIBLE_SHOT, { 1,-2 });
			App->particles->AddParticle(App->particles->redBall, 4897, 457, COLLIDER_ENEMY_DESTRUCTIBLE_SHOT, { -1,-2 });
			ball_timer = 20;
		}
		else {
			ball_timer--;
		}
		if (now >= total_time) {
			//spawnboss
			EnemyInfo info;
			info.type = HOSTUR;
			info.x = 4793;
			info.y = 480;
			boss = App->enemies->SpawnEnemyRet(info);
			total_time = (Uint32)(91.0f * 1000.0f);
			current_phase = boss_phases::boss_fight;
		}
	}break;
	case boss_phases::boss_fight: 
		if (now >= total_time) {
			current_phase = none;
			App->enemies->Kill(boss);
			App->ui->StageCleared();
		}
		break;
	}
}