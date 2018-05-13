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
	rect[0] = {0, 0, 2260, 208};
	rect[1] = {0, 0, 5104, 561};
	rect[2] = {0, 0, 775, 225};

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
	if (App->render->camera.x > 3150 && App->render->camera.x < 3874)
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

	if (App->render->camera.x > 2560 && App->render->camera.x < 4080)
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

	if (App->render->camera.x == 4670)	Mix_PlayMusic(bossTrack, -1);


	if (App->render->camera.x >= 4708)
	{
		App->render->Blit(textures[3], 4692, 179, &door.GetCurrentFrame());
		App->render->Blit(textures[3], 5074, 178, &door.GetCurrentFrame());
		if (collider) {
			coll = { 4691,179,59,56 };
			App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
			collider = false;
		}
		
	}

	if (App->render->camera.x >= 4718)
	{
		right = false;
	}

	if (App->render->camera.x + (App->render->camera.w * SCREEN_SIZE) == 4250) {
		EnemyInfo info;
		info.type = ENEMY_TYPES::JUMPER;
		info.x = 4099;
		info.y = 96;
		info.going_up = true;
		info.normal_spawn = true;
		App->enemies->SpawnEnemy(info);
		info.x = 4200;
		info.y = -100;
		App->enemies->SpawnEnemy(info);
		info.x = 4250;
		info.y = -200;
		App->enemies->SpawnEnemy(info);

		info.normal_spawn = false;
		info.y = 0;
		info.x = 4000;
		App->enemies->SpawnEnemy(info);
		info.y = -150;
		info.x = 4000;
		App->enemies->SpawnEnemy(info);
	}

	if (App->render->camera.x + (App->render->camera.w * SCREEN_SIZE) == 4100) {
		EnemyInfo info;
		info.type = ENEMY_TYPES::BLUEFLYER;
		info.x = 4050;
		info.y = 90;
		App->enemies->SpawnEnemy(info);
		info.x = 4025;
		info.y = 50;
		App->enemies->SpawnEnemy(info);
		info.x = 4100;
		info.y = 20;
		App->enemies->SpawnEnemy(info);
		info.x = 4000;
		info.y = -10;
		App->enemies->SpawnEnemy(info);
		info.x = 4060;
		info.y = -10;
		App->enemies->SpawnEnemy(info);
		info.x = 4120;
		info.y = -50;
		App->enemies->SpawnEnemy(info);
		info.x = 4080;
		info.y = -80;
		App->enemies->SpawnEnemy(info);
		info.x = 4150;
		info.y = -110;
		App->enemies->SpawnEnemy(info);
	}
	
}

update_status ModuleSceneStage3::Update()
{
	App->render->setAlpha(textures[0], bgalpha);
	App->render->setAlpha(textures[2], bgalpha);

	App->render->Blit(textures[0], 0, 226, &rect[0], 0.65f);
	App->render->Blit(textures[2], 4346, 95, &rect[2]);
	App->render->Blit(textures[1], 0, 0, &rect[1]);

	BackgroundEvents();
	UpdateCamera();
	CheckpointsUpdate();

	if (App->input->keyboard[SDL_SCANCODE_F7] == KEY_STATE::KEY_DOWN) {
		App->fade->FadeToBlack(this, App->stage1, 0.0f);
		App->ui->current_checkpoint = 0;
	}

	return update_status::UPDATE_CONTINUE;
}

bool ModuleSceneStage3::CleanUp()
{
	App->player->Disable();
	App->collision->Disable();
	App->powerups->Disable();
	App->particles->Disable();

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

	checkpoints[0] = { 0, 336 };
	checkpoints[1] = { 1555, 336 };
	checkpoints[2] = { 3100, 336 };
	checkpoints[3] = { 4348, 95 };
	checkpoints[4] = { -1, -1 };

	//top
	coll = { 0,336,3176,38 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	//Crater
	coll = { 140,374,95,11 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	//Crater
	coll = { 268,374,95,11 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	//Volcano
	coll = { 395,374,100,6 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	coll = { 404,380,74,8 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	coll = { 411,388,61,11 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	//Eye
	coll = { 505,374,28,7 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	//Crater
	coll = { 652,374,95,11 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	//Eye
	coll = { 854,374,28,7 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	//Volcano
	coll = { 906,374,100,6 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	coll = { 915,380,74,8 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	coll = { 922,388,61,11 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	//Crater
	coll = { 1035,374,95,11 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	//Eye
	coll = { 1238,374,28,7 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	//Crater
	coll = { 1290,374,95,11 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	//Crater
	coll = { 1418,374,95,11 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	//Volcano
	coll = { 1545,374,100,6 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	coll = { 1554,380,74,8 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	coll = { 1561,388,61,11 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	//Crater
	coll = { 1674,374,95,11 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	//Crater
	coll = { 1801,374,95,11 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	//Eye
	coll = { 2004,374,28,7 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	//Eye
	coll = { 2132,374,28,7 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	//Crater
	coll = { 2185,374,95,11 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	//Eye
	coll = { 2387,374,28,7 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	//Eye
	coll = { 2515,374,28,7 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	//Ciclop
	coll = { 2572,374,180,9 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	coll = { 2614,383,135,18 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	coll = { 2637,401,50,27 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	coll = { 2653,428,50,8 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	coll = { 2663,436,52,12 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	//Eye
	coll = { 2899,374,28,7 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	//Eye
	coll = { 3027,374,28,7 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);




	//bottom
	coll = { 0,524,3176,38 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	//Volcano
	coll = { 140,518,100,6 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	coll = { 147,510,74,8 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	coll = { 156,499,61,11 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	//Crater
	coll = { 269,513,95,11 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	//Volcano
	coll = { 396,518,100,6 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	coll = { 403,510,74,8 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	coll = { 412,499,61,11 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	//Volcano
	coll = { 524,518,100,6 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	coll = { 531,510,74,8 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	coll = { 540,499,61,11 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	//Crater
	coll = { 653,513,95,11 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	//Eye
	coll = { 760,517,28,7 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	//Eye
	coll = { 982,517,28,7 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	//Crater
	coll = { 1036,513,95,11 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	//Eye
	coll = { 1238,517,28,7 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	//Crater
	coll = { 1291,513,95,11 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	//Volcano
	coll = { 1418,518,100,6 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	coll = { 1425,510,74,8 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	coll = { 1434,499,61,11 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	//Crater
	coll = { 1547,513,95,11 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	//Eye
	coll = { 1749,517,28,7 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	//Crater
	coll = { 1802,513,95,11 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	//Eye
	coll = { 1910,517,28,7 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	//Crater
	coll = { 2058,513,95,11 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	//Volcano
	coll = { 2185,518,100,6 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	coll = { 2192,510,74,8 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	coll = { 2201,499,61,11 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	//Eye
	coll = { 2293,517,28,7 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	//Ciclop
	coll = { 2468,516,180,8 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	coll = { 2470,496,135,20 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	coll = { 2521,469,50,27 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	coll = { 2521,461,50,8 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	coll = { 2506,449,52,12 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	//Eye
	coll = { 2771,517,28,7 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	//Ciclop
	coll = { 2852,516,180,8 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	coll = { 2854,496,135,20 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	coll = { 2900,469,50,27 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	coll = { 2900,461,50,8 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	coll = { 2890,449,52,12 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);

	//diagonal top
	int y = 273;
	int x = 3182;
	for (int i = 0; i < 4 ; i++)
	{
		coll = { x,y,128,61 };
		App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
		y -= 32;
		x += 128;
	}
	coll = { 3693,103,128,61 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	//diagonal bottom
	coll = { 3451,526,75,36 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	y = 490;
	x = 3525;
	for (int i = 0; i < 5; i++)
	{
		coll = { x,y,128,71 };
		App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
		y -= 30;
		x += 128;
	}
	coll = { 4076,309,121,61 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);

	//top
	coll = { 4346,95,758,16 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);

	//bottom
	coll = { 4346,303,758,16 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);

	//end
	coll = { 4691,111,59,68 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	coll = { 4691,235,59,68 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);
	coll = { 5075,111,29,192 };
	App->collision->AddCollider(coll, COLLIDER_WALL, nullptr);


	if (textures[0] == nullptr) {
		return false;
	}
	else return true;
}

void ModuleSceneStage3::loadEnemies()
{

	App->enemies->AddEnemy(ENEMY_TYPES::BLUEPATROL, 869, 500);
	App->enemies->AddEnemy(ENEMY_TYPES::BLUEPATROL, 850, 500, false, -1, false);

	App->enemies->AddEnemy(ENEMY_TYPES::CYCLOP, 2488, 450, false);
	App->enemies->AddEnemy(ENEMY_TYPES::CYCLOP, 2688, 392, true);
	App->enemies->AddEnemy(ENEMY_TYPES::CYCLOP, 2870, 450, false);

	App->enemies->AddEnemy(ENEMY_TYPES::WORM, 500, 320, true);
	App->enemies->AddEnemy(ENEMY_TYPES::WORM, 500, 450, false);

	App->enemies->AddEnemy(ENEMY_TYPES::JUMPER, 572, 472, false, -1, false);
	App->enemies->AddEnemy(ENEMY_TYPES::JUMPER, 3074, 484, false, -1, false); 
	App->enemies->AddEnemy(ENEMY_TYPES::JUMPER, 3464, 450, true, -1, true); 
	App->enemies->AddEnemy(ENEMY_TYPES::JUMPER, 3486, 438, true, -1, true);
	App->enemies->AddEnemy(ENEMY_TYPES::JUMPER, 3559, 435, true, -1, true);
	App->enemies->AddEnemy(ENEMY_TYPES::JUMPER, 3654, 421, true, -1, true);
	App->enemies->AddEnemy(ENEMY_TYPES::JUMPER, 3809, 208, true, -1, true);
	App->enemies->AddEnemy(ENEMY_TYPES::JUMPER, 3957, 193, true, -1, true);
	App->enemies->AddEnemy(ENEMY_TYPES::JUMPER, 4373, 268);
	App->enemies->AddEnemy(ENEMY_TYPES::JUMPER, 4411, 222);
	App->enemies->AddEnemy(ENEMY_TYPES::JUMPER, 4429, 224);
	App->enemies->AddEnemy(ENEMY_TYPES::JUMPER, 4429, 200);
	App->enemies->AddEnemy(ENEMY_TYPES::JUMPER, 4547, 224);
	App->enemies->AddEnemy(ENEMY_TYPES::JUMPER, 4589, 200);
	App->enemies->AddEnemy(ENEMY_TYPES::JUMPER, 4600, 180);
	App->enemies->AddEnemy(ENEMY_TYPES::JUMPER, 4638, 185);
	App->enemies->AddEnemy(ENEMY_TYPES::JUMPER, 4680, 180);
	App->enemies->AddEnemy(ENEMY_TYPES::WORMHOLE, 286, 515);
	App->enemies->AddEnemy(ENEMY_TYPES::WORMHOLE, 671, 505);
	App->enemies->AddEnemy(ENEMY_TYPES::WORMHOLE, 1054, 503);
	App->enemies->AddEnemy(ENEMY_TYPES::WORMHOLE, 1307, 503);
	App->enemies->AddEnemy(ENEMY_TYPES::WORMHOLE, 1565, 503);
	App->enemies->AddEnemy(ENEMY_TYPES::WORMHOLE, 1819, 503);
	App->enemies->AddEnemy(ENEMY_TYPES::WORMHOLE, 2074, 503);
}

void ModuleSceneStage3::loadAudio()
{
	music = App->audio->LoadMusic("Assets/Audio/Music/06_The_Rolling_Worms_Stage_3-1_.ogg");
	secondTrack = App->audio->LoadMusic("Assets/Audio/Music/07_The_Rolling_Worms_Stage_3-2_.ogg");
	bossTrack = App->audio->LoadMusic("Assets/Audio/Music/03_Boss_Theme.ogg");
}