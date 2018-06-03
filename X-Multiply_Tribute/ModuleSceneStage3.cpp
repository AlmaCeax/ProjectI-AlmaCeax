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
#include "SDL_mixer\include\SDL_mixer.h"
#include "ModuleSceneStage1.h"
#include "ModuleSceneStage3.h"



ModuleSceneStage3::ModuleSceneStage3()
{
	for (int i = 0; i < 3; ++i)
	{
		textures[i] = nullptr;
	}
}


ModuleSceneStage3::~ModuleSceneStage3()
{

}


bool ModuleSceneStage3::Start() {
	App->current_stage = this;

	App->collision->Enable();
	App->powerups->Enable();
	App->particles->Enable();
	App->enemies->Enable();
	App->ui->ui_visible = true;

	right = false;
	left = false;
	up = false;
	down = false;

	if (!loadMap()) {
		return false;
	}
	loadEnemies();
	loadAudio();

	App->ui->PlayerReady();

	return true;
}

bool ModuleSceneStage3::Init()
{
	score_bonus = 30000;
	index = 3;
	rect[0] = {0, 0, 2800, 208};
	rect[1] = {0, 0, 5760, 561};
	rect[2] = {0, 0, 775, 225};
	rect[3] = {0, 0, 322, 195};

	door.PushBack({ 0, 0, 57, 57 });
	door.PushBack({ 61, 0, 57, 57 });
	door.PushBack({ 124, 0, 57, 56 });
	door.PushBack({ 188, 0, 57, 55 });
	door.PushBack({ 251, 0, 56, 56 });
	door.PushBack({ 312, 0, 56, 56 });
	door.speed = 0.05f;
	door.loop = false;

	return true;
}

void ModuleSceneStage3::UpdateCamera()
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
			App->player->position.y -= 1;
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

void ModuleSceneStage3::BackgroundEvents()
{
	if (App->render->camera.x > 3806 && App->render->camera.x < 4530)
	{
		if (!up)
		{
			up = true;
			Mix_PlayMusic(secondTrack, -1);
		}

	}
	else if (up) {
		up = false;
	}

	if (App->render->camera.x > 3216 && App->render->camera.x < 4736)
	{
		if (bgalpha > 0)
		{
			bgalpha--;
		}
	}
	else {
		if (bgalpha < 255)
		{
			bgalpha++;
		}
	}

	if (App->render->camera.x == 5326)	Mix_PlayMusic(bossTrack, -1);


	if (App->render->camera.x >= 5364)
	{
		App->render->Blit(textures[3], 5354, 179, &door.GetCurrentFrame());
		App->render->Blit(textures[3], 5736, 178, &door.GetCurrentFrame());
		if (collider) {
			coll = { 4691,179,59,56 };
			App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
			collider = false;
		}
		
	}

	if (App->render->camera.x >= 5374)
	{
		right = false;
		if (!bossspawned)
		{
			EnemyInfo infozarikasu1;
			infozarikasu1.type = ENEMY_TYPES::ZARIKASU;
			infozarikasu1.x = 5735;
			infozarikasu1.y = 317;
			infozarikasu1.powerUpid = 1;
			boss[0] = App->enemies->SpawnEnemyRet(infozarikasu1);

			EnemyInfo infozarikasu2;
			infozarikasu2.type = ENEMY_TYPES::ZARIKASU;
			infozarikasu2.x = 5337;
			infozarikasu2.y = 317;
			infozarikasu2.powerUpid = 2;
			boss[1] = App->enemies->SpawnEnemyRet(infozarikasu2);

			EnemyInfo infozarikasu3;
			infozarikasu3.type = ENEMY_TYPES::ZARIKASU;
			infozarikasu3.x = 5735;
			infozarikasu3.y = 40;
			infozarikasu3.powerUpid = 3;
			boss[2] = App->enemies->SpawnEnemyRet(infozarikasu3);

			EnemyInfo infozarikasu4;
			infozarikasu4.type = ENEMY_TYPES::ZARIKASU;
			infozarikasu4.x = 5337;
			infozarikasu4.y = 40;
			infozarikasu4.powerUpid = 4;
			boss[3] = App->enemies->SpawnEnemyRet(infozarikasu4);

			bossspawned = true;
		}
	}

	if (App->render->camera.x + (App->render->camera.w * SCREEN_SIZE) == 4906) {
		EnemyInfo info;
		info.type = ENEMY_TYPES::JUMPER;
		info.x = 4755;
		info.y = 96;
		info.going_up = true;
		info.normal_spawn = true;
		App->enemies->SpawnEnemy(info);
		info.x = 4856;
		info.y = -100;
		App->enemies->SpawnEnemy(info);
		info.x = 4906;
		info.y = -200;
		App->enemies->SpawnEnemy(info);

		info.normal_spawn = false;
		info.y = 0;
		info.x = 4656;
		App->enemies->SpawnEnemy(info);
		info.y = -150;
		info.x = 4656;
		App->enemies->SpawnEnemy(info);
	}

	if (App->render->camera.x + (App->render->camera.w * SCREEN_SIZE) == 4756) {
		EnemyInfo info;
		info.type = ENEMY_TYPES::BLUEFLYER;
		info.x = 4706;
		info.y = 90;
		App->enemies->SpawnEnemy(info);
		info.x = 4681;
		info.y = 50;
		App->enemies->SpawnEnemy(info);
		info.x = 4756;
		info.y = 20;
		App->enemies->SpawnEnemy(info);
		info.x = 4656;
		info.y = -10;
		App->enemies->SpawnEnemy(info);
		info.x = 4716;
		info.y = -10;
		App->enemies->SpawnEnemy(info);
		info.x = 4776;
		info.y = -50;
		App->enemies->SpawnEnemy(info);
		info.x = 4736;
		info.y = -80;
		App->enemies->SpawnEnemy(info);
		info.x = 4806;
		info.y = -110;
		App->enemies->SpawnEnemy(info);
	}
	
}

update_status ModuleSceneStage3::Update()
{
	App->render->setAlpha(textures[0], bgalpha);
	App->render->setAlpha(textures[2], bgalpha);

	App->render->Blit(textures[0], 10, 226, &rect[0], 0.65f);
	App->render->Blit(textures[2], 5002, 95, &rect[2]);
	App->render->Blit(textures[1], 0, 0, &rect[1]);
	if(bossdead)App->render->Blit(textures[4], 5412, 112, &rect[3]);

	BackgroundEvents();
	UpdateCamera();
	CheckpointsUpdate();

	if (App->input->keyboard[SDL_SCANCODE_F7] == KEY_STATE::KEY_DOWN) {
		App->fade->FadeToBlack(this, App->stage1, 0.0f);
		App->ui->current_checkpoint = 0;
	}

	if(!end)
	{
		if (bossdeads == 4)
		{
			App->particles->AddParticle(App->particles->multipleBigExplosion, 5560, 210, COLLIDER_TYPE::COLLIDER_NONE, { 0, 0 }, 0, 4, true);
			App->ui->StageCleared();
			end = true;
		}
	}

	return update_status::UPDATE_CONTINUE;
}

bool ModuleSceneStage3::CleanUp()
{
	App->player->Disable();
	App->collision->Disable();
	App->powerups->Disable();
	App->particles->Disable();
	App->enemies->Disable();

	for (int i = 0; i < 3; ++i)
	{
		App->textures->Unload(textures[i]);
		textures[i] = nullptr;
	}

	App->audio->UnloadMusic(music);
	App->audio->UnloadMusic(secondTrack);
	App->audio->UnloadMusic(bossTrack);
	music = nullptr;
	secondTrack = nullptr;
	bossTrack = nullptr;

	return true;
}


bool ModuleSceneStage3::loadMap()
{
	textures[0] = App->textures->Load("Assets/Sprites/Stages/Stage3/Backgroundbg03.png");
	textures[1] = App->textures->Load("Assets/Sprites/Stages/Stage3/Layerbg03.png");
	textures[2] = App->textures->Load("Assets/Sprites/Stages/Stage3/BackgroundFinalbg03.png");
	textures[3] = App->textures->Load("Assets/Sprites/Stages/Stage3/Door.png");
	textures[4] = App->textures->Load("Assets/Sprites/Stages/Stage3/ZarikasuDead.png");

	checkpoints[0] = { 0, 336 };
	checkpoints[1] = { 2211, 336 };
	checkpoints[2] = { 3756, 336 };
	checkpoints[3] = { 5004, 95 };
	checkpoints[4] = { -1, -1 };
	//656
	//top
	coll = { 0,336,3832,38 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	//Volcano
	coll = { 403,374,100,6 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	coll = { 412,380,74,8 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	coll = { 419,388,61,11 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	//Eye
	coll = { 641,374,28,7 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	//Eye
	coll = { 991,374,28,7 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	//Volcano
	coll = { 1043,374,100,6 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	coll = { 1052,380,74,8 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	coll = { 1059,388,61,11 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	//Eye
	coll = { 1375,374,28,7 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	//Volcano
	coll = { 1683,374,100,6 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	coll = { 1692,380,74,8 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	coll = { 1699,388,61,11 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	//Eye
	coll = { 2143,374,28,7 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	//Eye
	coll = { 2399,374,28,7 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	//Volcano
	coll = { 2579,374,100,6 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	coll = { 2588,380,74,8 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	coll = { 2595,388,61,11 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	//Eye
	coll = { 2783,374,28,7 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	//Eye
	coll = { 3039,374,28,7 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	//Eye
	coll = { 3167,374,28,7 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	//Ciclop
	coll = { 3228,374,180,9 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	coll = { 3270,383,135,18 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	coll = { 3293,401,50,27 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	coll = { 3309,428,50,8 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	coll = { 3319,436,52,12 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	//Eye
	coll = { 3551,374,28,7 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	//Eye
	coll = { 3679,374,28,7 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);


	//bottom
	coll = { 0,524,3832,38 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	//Volcano
	coll = { 147,518,100,6 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	coll = { 154,510,74,8 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	coll = { 163,499,61,11 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	//Volcano
	coll = { 403,518,100,6 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	coll = { 410,510,74,8 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	coll = { 421,499,61,11 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	//Volcano
	coll = { 659,518,100,6 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	coll = { 666,510,74,8 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	coll = { 675,499,61,11 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	//Eye
	coll = { 897,517,28,7 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	//Eye
	coll = { 1119,517,28,7 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	//Eye
	coll = { 1375,517,28,7 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	//Volcano
	coll = { 1555,518,100,6 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	coll = { 1562,510,74,8 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	coll = { 1571,499,61,11 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	//Eye
	coll = { 1887,517,28,7 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	//Eye
	coll = { 2049,517,28,7 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	//Volcano
	coll = { 2195,518,100,6 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	coll = { 2202,510,74,8 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	coll = { 2211,499,61,11 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	//Eye
	coll = { 2305,517,28,7 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	//Eye
	coll = { 2561,517,28,7 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	//Volcano
	coll = { 2836,518,100,6 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	coll = { 2843,510,74,8 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	coll = { 2852,499,61,11 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	//Eye
	coll = { 2945,517,28,7 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	//Ciclop
	coll = { 3124,516,180,8 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	coll = { 3126,496,135,20 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	coll = { 3177,469,50,27 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	coll = { 3177,461,50,8 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	coll = { 3162,449,52,12 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	//Eye
	coll = { 3427,517,28,7 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	//Ciclop
	coll = { 3508,516,180,8 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	coll = { 3510,496,135,20 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	coll = { 3556,469,50,27 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	coll = { 3556,461,50,8 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	coll = { 3546,449,52,12 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);

	//diagonal top
	int y = 273;
	int x = 3838;
	for (int i = 0; i < 4 ; i++)
	{
		coll = { x,y,128,61 };
		App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
		y -= 32;
		x += 128;
	}
	coll = { 4349,103,128,61 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	//diagonal bottom
	coll = { 4107,526,75,36 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	y = 490;
	x = 4181;
	for (int i = 0; i < 5; i++)
	{
		coll = { x,y,128,71 };
		App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
		y -= 30;
		x += 128;
	}
	coll = { 4732,309,121,61 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);

	//top
	coll = { 5007,95,758,16 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);

	//bottom
	coll = { 5007,303,758,16 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);

	//end
	coll = { 5352,111,59,68 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	coll = { 5352,235,59,68 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	coll = { 5736,111,29,192 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);


	if (textures[0] == nullptr) {
		return false;
	}
	else return true;
}

void ModuleSceneStage3::loadEnemies()
{

	App->enemies->AddEnemy(ENEMY_TYPES::POWERUPSHIP, 820, 430, false, 0, 7);
	App->enemies->AddEnemy(ENEMY_TYPES::POWERUPSHIP, 1300, 470, false, 0, 3);
	App->enemies->AddEnemy(ENEMY_TYPES::POWERUPSHIP, 2750, 470, false, 0, 3);
	App->enemies->AddEnemy(ENEMY_TYPES::POWERUPSHIP, 4100, 470, false, 0, 4);
	App->enemies->AddEnemy(ENEMY_TYPES::POWERUPSHIP, 4250, 400, false, 0, 3);
	App->enemies->AddEnemy(ENEMY_TYPES::POWERUPSHIP, 4350, 350, false, 0, 1);
	App->enemies->AddEnemy(ENEMY_TYPES::POWERUPSHIP, 4680, 321, false, 0, 6);
	App->enemies->AddEnemy(ENEMY_TYPES::POWERUPSHIP, 4880, 250, false, 0, 5);
	App->enemies->AddEnemy(ENEMY_TYPES::POWERUPSHIP, 5000, 220, false, 0, 3);

	App->enemies->AddEnemy(ENEMY_TYPES::BLUEPATROL, 1020, 500);
	App->enemies->AddEnemy(ENEMY_TYPES::BLUEPATROL, 1000, 500, false, 0, -1, false);
	App->enemies->AddEnemy(ENEMY_TYPES::BLUEPATROL, 2103, 500);
	App->enemies->AddEnemy(ENEMY_TYPES::BLUEPATROL, 2615, 500);

	App->enemies->AddEnemy(ENEMY_TYPES::CYCLOP, 3144, 450, false);
	App->enemies->AddEnemy(ENEMY_TYPES::CYCLOP, 3344, 392, true);
	App->enemies->AddEnemy(ENEMY_TYPES::CYCLOP, 3526, 450, false);

	App->enemies->AddEnemy(ENEMY_TYPES::JUMPER, 1228, 472, false, 0, -1, false);
	App->enemies->AddEnemy(ENEMY_TYPES::JUMPER, 3730, 484, false, 0, -1, false); 
	App->enemies->AddEnemy(ENEMY_TYPES::JUMPER, 4120, 450, true, 0, -1, true); 
	App->enemies->AddEnemy(ENEMY_TYPES::JUMPER, 4142, 438, true, 0, -1, true);
	App->enemies->AddEnemy(ENEMY_TYPES::JUMPER, 4215, 435, true, 0, -1, true);
	App->enemies->AddEnemy(ENEMY_TYPES::JUMPER, 4310, 421, true, 0, -1, true);
	App->enemies->AddEnemy(ENEMY_TYPES::JUMPER, 4465, 208, true, 0, -1, true);
	App->enemies->AddEnemy(ENEMY_TYPES::JUMPER, 4613, 193, true, 0, -1, true);

	//App->enemies->AddEnemy(ENEMY_TYPES::SNAKE, 600, 450);

	App->enemies->AddEnemy(ENEMY_TYPES::JUMPER, 5029, 268);
	App->enemies->AddEnemy(ENEMY_TYPES::JUMPER, 5067, 222);
	App->enemies->AddEnemy(ENEMY_TYPES::JUMPER, 5085, 224);
	App->enemies->AddEnemy(ENEMY_TYPES::JUMPER, 5085, 200);
	App->enemies->AddEnemy(ENEMY_TYPES::JUMPER, 5203, 224);
	App->enemies->AddEnemy(ENEMY_TYPES::JUMPER, 5245, 200);
	App->enemies->AddEnemy(ENEMY_TYPES::JUMPER, 5256, 180);
	App->enemies->AddEnemy(ENEMY_TYPES::JUMPER, 5294, 185);
	App->enemies->AddEnemy(ENEMY_TYPES::JUMPER, 5336, 180);


	//top
	EnemyInfo infoWormhole;
	infoWormhole.type = ENEMY_TYPES::WORMHOLE;
	infoWormhole.x = 167;
	infoWormhole.y = 344;
	infoWormhole.going_up = true;
	App->enemies->SpawnEnemy(infoWormhole);
	infoWormhole.x = 295;
	App->enemies->SpawnEnemy(infoWormhole);
	App->enemies->AddEnemy(ENEMY_TYPES::WORMHOLE, 551, 344, true);
	App->enemies->AddEnemy(ENEMY_TYPES::WORMHOLE, 807, 344, true);
	App->enemies->AddEnemy(ENEMY_TYPES::WORMHOLE, 1191, 344, true);
	App->enemies->AddEnemy(ENEMY_TYPES::WORMHOLE, 1447, 344, true);
	App->enemies->AddEnemy(ENEMY_TYPES::WORMHOLE, 1575, 344, true);
	App->enemies->AddEnemy(ENEMY_TYPES::WORMHOLE, 1831, 344, true);
	App->enemies->AddEnemy(ENEMY_TYPES::WORMHOLE, 1959, 344, true);
	App->enemies->AddEnemy(ENEMY_TYPES::WORMHOLE, 2215, 344, true);
	App->enemies->AddEnemy(ENEMY_TYPES::WORMHOLE, 2471, 344, true);
	App->enemies->AddEnemy(ENEMY_TYPES::WORMHOLE, 2855, 344, true);

	//bottom
	infoWormhole.y = 500;
	infoWormhole.going_up = false;
	App->enemies->SpawnEnemy(infoWormhole);
	App->enemies->AddEnemy(ENEMY_TYPES::WORMHOLE, 550, 500, false);
	App->enemies->AddEnemy(ENEMY_TYPES::WORMHOLE, 806, 500, false);
	App->enemies->AddEnemy(ENEMY_TYPES::WORMHOLE, 1190, 500, false);
	App->enemies->AddEnemy(ENEMY_TYPES::WORMHOLE, 1446, 500, false);
	App->enemies->AddEnemy(ENEMY_TYPES::WORMHOLE, 1702, 500, false);
	App->enemies->AddEnemy(ENEMY_TYPES::WORMHOLE, 1958, 500, false);
	App->enemies->AddEnemy(ENEMY_TYPES::WORMHOLE, 2470, 500, false);
	App->enemies->AddEnemy(ENEMY_TYPES::WORMHOLE, 2726, 500, false);

}

void ModuleSceneStage3::loadAudio()
{
	music = App->audio->LoadMusic("Assets/Audio/Music/06_The_Rolling_Worms_Stage_3-1_.ogg");
	secondTrack = App->audio->LoadMusic("Assets/Audio/Music/07_The_Rolling_Worms_Stage_3-2_.ogg");
	bossTrack = App->audio->LoadMusic("Assets/Audio/Music/03_Boss_Theme.ogg");
}