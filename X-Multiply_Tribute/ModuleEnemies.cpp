#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "ModuleEnemies.h"
#include "ModuleParticles.h"
#include "ModuleTextures.h"
#include "ModuleUI.h"
#include "Enemy_HellBall.h"
#include "Enemy_FlyingWorm.h"
#include "Enemy_GreenEye.h"
#include "Enemy_BrownEye.h"
#include "Enemy_Jumper.h"
#include "Enemy_Hostur.h"
#include "Enemy_TentacleShooter.h"
#include "Enemy_BlueMouth.h"
#include "Enemy_BlueFlyer.h"
#include "Enemy_PowerUPShip.h"
#include "Enemy_Cyclop.h"
#include "Enemy_Alien.h"
#include "Enemy_BluePatrol.h"
#include "Enemy_Worm.h"
#include "Enemy_WormBody.h"
#include "Enemy_WormHole.h"
#include "Enemy_Wall.h"
#include "ModuleStage.h"
#include "SDL_mixer\include\SDL_mixer.h"
#include "Enemy.h"

#define SPAWN_MARGIN 20

ModuleEnemies::ModuleEnemies()
{
	for (uint i = 0; i < MAX_ENEMIES; ++i) {
		enemies[i] = nullptr;
		lives[i] = 0;
	}	
}

// Destructor
ModuleEnemies::~ModuleEnemies()
{
	
}

bool ModuleEnemies::Start()
{
	// Create a prototype for each enemy available so we can copy them around
	sprites = App->textures->Load("Assets/Sprites/Enemies/enemies.png");

	hellballDeadsfx = App->audio->LoadFx("Assets/Audio/SFX/xmultipl-066.wav");
	flyerDeadsfx = App->audio->LoadFx("Assets/Audio/SFX/xmultipl-066.wav");
	nemonaDeadsfx = App->audio->LoadFx("Assets/Audio/SFX/xmultipl-063.wav");
	pushipDeadsfx = App->audio->LoadFx("Assets/Audio/SFX/xmultipl-027.wav");
	hitEnemysfx = App->audio->LoadFx("Assets/Audio/SFX/xmultipl-070.wav");
	return true;
}

update_status ModuleEnemies::PreUpdate()
{
	// check camera position to decide what to spawn
	for(uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if(queue[i].type != ENEMY_TYPES::NO_TYPE)
		{
			if (queue[i].normal_spawn) {
				if (queue[i].x * SCREEN_SIZE < App->render->camera.x + (App->render->camera.w * SCREEN_SIZE) + SPAWN_MARGIN && queue[i].x * SCREEN_SIZE> App->render->camera.x + (App->render->camera.w * SCREEN_SIZE))
				{
					SpawnEnemy(queue[i]);
					queue[i].type = ENEMY_TYPES::NO_TYPE;
					LOG("Spawning enemy at %d", queue[i].x * SCREEN_SIZE);
				}
			}
			else {
				if (queue[i].x * SCREEN_SIZE < (App->render->camera.x) - SPAWN_MARGIN)
				{
					SpawnEnemy(queue[i]);
					queue[i].type = ENEMY_TYPES::NO_TYPE;
					LOG("Spawning enemy at %d", queue[i].x * SCREEN_SIZE);
				}
			}
		}
	}

	return UPDATE_CONTINUE;
}

// Called before render is available
update_status ModuleEnemies::Update()
{
	for(uint i = 0; i < MAX_ENEMIES; ++i)
		if(enemies[i] != nullptr) enemies[i]->Move();

	for(uint i = 0; i < MAX_ENEMIES; ++i)
		if(enemies[i] != nullptr) enemies[i]->Draw(sprites);

	return UPDATE_CONTINUE;
}

update_status ModuleEnemies::PostUpdate()
{
	// check camera position to decide what to spawn
	for(uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if(enemies[i] != nullptr)
		{
			if(enemies[i]->position.x * SCREEN_SIZE < (App->render->camera.x) - (SPAWN_MARGIN*2))
			{
				LOG("DeSpawning enemy at %d", enemies[i]->position.x * SCREEN_SIZE);
				delete enemies[i];
				enemies[i] = nullptr;
			}
		}
	}

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleEnemies::CleanUp()
{
	LOG("Freeing all enemies");

	App->textures->Unload(sprites);
	sprites = nullptr;

	for(uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if(enemies[i] != nullptr)
		{
			delete enemies[i];
			enemies[i] = nullptr;
		}
	}

	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		queue[i].type = ENEMY_TYPES::NO_TYPE;
	}
	
	App->audio->UnloadSFX(hellballDeadsfx);
	hellballDeadsfx = nullptr;
	App->audio->UnloadSFX(flyerDeadsfx);
	flyerDeadsfx = nullptr;
	App->audio->UnloadSFX(nemonaDeadsfx);
	nemonaDeadsfx = nullptr;
	App->audio->UnloadSFX(pushipDeadsfx);
	pushipDeadsfx = nullptr;

	return true;
}

bool ModuleEnemies::AddEnemy(ENEMY_TYPES type, int x, int y, bool going_up, int speed, int powerUPid, bool normal_spawn, bool tail)
{
	bool ret = false;

	for(uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if(queue[i].type == ENEMY_TYPES::NO_TYPE)
		{
			queue[i].type = type;
			queue[i].x = x;
			queue[i].y = y;
			queue[i].going_up = going_up;
			queue[i].speed = speed;
			queue[i].powerUpid = powerUPid;
			queue[i].normal_spawn = normal_spawn;
			queue[i].tail = tail;
			ret = true;
			break;
		}
	}

	return ret;
}

void ModuleEnemies::SpawnEnemy(const EnemyInfo& info)
{
	// find room for the new enemy
	uint i = 0;
	for(; enemies[i] != nullptr && i < MAX_ENEMIES; ++i);

	if(i != MAX_ENEMIES)
	{
		switch(info.type)
		{
			case ENEMY_TYPES::HELLBALL:
				enemies[i] = new Enemy_HellBall(info.x,info.y,info.going_up);
				break;
			case ENEMY_TYPES::FLYINGWORM:
				enemies[i] = new Enemy_FlyingWorm(info.x, info.y);
				break;
			case ENEMY_TYPES::TENTACLESHOOTER:
				enemies[i] = new Enemy_TentacleShooter(info.x, info.y, info.going_up);
				lives[i] = 3;
				break;
			case ENEMY_TYPES::POWERUPSHIP:
				enemies[i] = new Enemy_PowerUPShip(info.x, info.y, info.powerUpid);
				break;
			case ENEMY_TYPES::CYCLOP:
				enemies[i] = new Enemy_Cyclop(info.x, info.y, info.going_up);
				lives[i] = 14;
				break;
			case ENEMY_TYPES::ALIEN:
				enemies[i] = new Enemy_Alien(info.x, info.y);
				lives[i] = 20;
				break;
			case ENEMY_TYPES::GREENEYE:
				enemies[i] = new Enemy_GreenEye(info.x, info.y, info.going_up, info.speed);
				break;
			case ENEMY_TYPES::BROWNEYE:
				enemies[i] = new Enemy_BrownEye(info.x, info.y);
				lives[i] = 15;
				break;
			case ENEMY_TYPES::JUMPER:
				enemies[i] = new Enemy_Jumper(info.x, info.y, info.going_up, info.normal_spawn);
				break;
			case ENEMY_TYPES::BLUEMOUTH:
				enemies[i] = new Enemy_BlueMouth(info.x, info.y,info.going_up);
				break;
			case ENEMY_TYPES::BLUEFLYER:
				enemies[i] = new Enemy_BlueFlyer(info.x, info.y);
				lives[i] = 3;
				break;
			case ENEMY_TYPES::BLUEPATROL:
				enemies[i] = new Enemy_BluePatrol(info.x, info.y);
				lives[i] = 2;
				break;
			case ENEMY_TYPES::WORM:
				enemies[i] = new Enemy_Worm(info.x, info.y,info.going_up);
				lives[i] = 2;
				break;
			case ENEMY_TYPES::WORMBODY:
				enemies[i] = new Enemy_WormBody(info.x, info.y, info.going_up, info.tail);
				lives[i] = 1;
				break;
			case ENEMY_TYPES::WORMHOLE:
				enemies[i] = new Enemy_WormHole(info.x, info.y, info.going_up);
				lives[i] = 12;
				break;
			case ENEMY_TYPES::WALL:
				enemies[i] = new Enemy_Wall(info.x, info.y);
				lives[i] = 18;
				break;
			case ENEMY_TYPES::HOSTUR:
				enemies[i] = new Enemy_Hostur(info.x, info.y);
				lives[i] = 160;
				break;
		}
	}
}

void ModuleEnemies::OnCollision(Collider* c1, Collider* c2)
{
	for(uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (enemies[i] != nullptr && enemies[i]->type == JUMPER) {
			if (((Enemy_Jumper*)enemies[i])->ground_collider == c1) {
				((Enemy_Jumper*)enemies[i])->SetDownAnim();
				break;
			}
		}

		if (enemies[i] != nullptr && enemies[i]->type == HOSTUR) {
			for (int j = 0; j < 5; ++j) {
				if (((Enemy_Hostur*)enemies[i])->colliders[j] == c1) {
					lives[i]--;
					if (lives[i] == 0) {
						Mix_PlayChannel(-1, nemonaDeadsfx, 0);
						enemies[i]->OnCollision(c2);
						delete enemies[i];
						enemies[i] = nullptr;
						App->ui->StageCleared();
						return;
					}
					else {
						Mix_PlayChannel(-1, hitEnemysfx, 0);
						enemies[i]->Shine();
						return;
					}
				}
			}	
		}

		if(enemies[i] != nullptr && enemies[i]->GetCollider() == c1)
		{	
			switch (enemies[i]->type)
			{
			case NO_TYPE:
				enemies[i]->OnCollision(c2);
				delete enemies[i];
				enemies[i] = nullptr;
				break;
			case HELLBALL:
				Mix_PlayChannel(-1, hellballDeadsfx, 0);
				enemies[i]->OnCollision(c2);
				delete enemies[i];
				enemies[i] = nullptr;
				break;
			case FLYINGWORM:
				Mix_PlayChannel(-1, flyerDeadsfx, 0);
				enemies[i]->OnCollision(c2);
				delete enemies[i];
				enemies[i] = nullptr;
				break;
			case TENTACLESHOOTER:
				lives[i]--;
				if (lives[i] == 0) {
					Mix_PlayChannel(-1, nemonaDeadsfx, 0);
					enemies[i]->OnCollision(c2);
					delete enemies[i];
					enemies[i] = nullptr;
				}
				else {
					Mix_PlayChannel(-1, hitEnemysfx, 0);
					enemies[i]->Shine();
				}
				break;
			case POWERUPSHIP:
				Mix_PlayChannel(-1, pushipDeadsfx, 0);
				enemies[i]->OnCollision(c2);
				delete enemies[i];
				enemies[i] = nullptr;
				break;
			case CYCLOP:
				lives[i]--;
				if (lives[i] == 0) {
					Mix_PlayChannel(-1, nemonaDeadsfx, 0);
					enemies[i]->OnCollision(c2);
					delete enemies[i];
					enemies[i] = nullptr;
				}
				else {
					Mix_PlayChannel(-1, hitEnemysfx, 0);
					enemies[i]->Shine();
				}
				break;
			case ALIEN:
				lives[i]--;
				if (lives[i] == 0) {
					Mix_PlayChannel(-1, nemonaDeadsfx, 0);
					enemies[i]->OnCollision(c2);
					delete enemies[i];
					enemies[i] = nullptr;
				}
				else {
					Mix_PlayChannel(-1, hitEnemysfx, 0);
					enemies[i]->Shine();
				}
				break;
			case BROWNEYE:
				lives[i]--;
				if (lives[i] == 0) {
					Mix_PlayChannel(-1, nemonaDeadsfx, 0);
					enemies[i]->OnCollision(c2);
					delete enemies[i];
					enemies[i] = nullptr;
				}
				else {
					Mix_PlayChannel(-1, hitEnemysfx, 0);
					enemies[i]->Shine();
				}
				break;
			case GREENEYE:
				Mix_PlayChannel(-1, flyerDeadsfx, 0);
				enemies[i]->OnCollision(c2);
				delete enemies[i];
				enemies[i] = nullptr;
				break;
			case JUMPER:
				if (c2->type == COLLIDER_WALL) {
					((Enemy_Jumper*)enemies[i])->TouchGround();
				}
				else {
					Mix_PlayChannel(-1, flyerDeadsfx, 0);
					enemies[i]->OnCollision(c2);
					delete enemies[i];
					enemies[i] = nullptr;
				}
				break;
			case BLUEFLYER:
				if (c2->type == COLLIDER_WALL) {
					((Enemy_BlueFlyer*)enemies[i])->TouchGround();
				}
				else {
					lives[i]--;
					if (lives[i] == 0) {
						Mix_PlayChannel(-1, nemonaDeadsfx, 0);
						enemies[i]->OnCollision(c2);
						delete enemies[i];
						enemies[i] = nullptr;
					}
					else {
						Mix_PlayChannel(-1, hitEnemysfx, 0);
						enemies[i]->Shine();
					}
				}
				break;
			case BLUEMOUTH:
				lives[i]--;
				if (lives[i] == 0) {
					Mix_PlayChannel(-1, nemonaDeadsfx, 0);
					enemies[i]->OnCollision(c2);
					delete enemies[i];
					enemies[i] = nullptr;
				}
				break;
			case BLUEPATROL:
				lives[i]--;
				if (lives[i] == 0) {
					Mix_PlayChannel(-1, nemonaDeadsfx, 0);
					enemies[i]->OnCollision(c2);
					delete enemies[i];
					enemies[i] = nullptr;
				}
				else {
					Mix_PlayChannel(-1, hitEnemysfx, 0);
					enemies[i]->Shine();
				}
				break;
			case WORM:
				lives[i]--;
				if (lives[i] == 0) {
					Mix_PlayChannel(-1, nemonaDeadsfx, 0);
					enemies[i]->OnCollision(c2);
					delete enemies[i];
					enemies[i] = nullptr;
				}
				else {
					Mix_PlayChannel(-1, hitEnemysfx, 0);
					enemies[i]->Shine();
				}
				break;
			case WORMBODY:
				lives[i]--;
				if (lives[i] == 0) {
					if (((Enemy_WormBody*)enemies[i])->splited == true) {
						Mix_PlayChannel(-1, nemonaDeadsfx, 0);
						enemies[i]->OnCollision(c2);
						delete enemies[i];
						enemies[i] = nullptr;
					}
				}
				break;
			case WORMHOLE:
				lives[i]--;
				if (lives[i] == 0) {
					Mix_PlayChannel(-1, nemonaDeadsfx, 0);
					enemies[i]->OnCollision(c2);
					delete enemies[i];
					enemies[i] = nullptr;
				}
				else {
					Mix_PlayChannel(-1, hitEnemysfx, 0);
					enemies[i]->Shine();
				}
				break;
			case WALL:
				lives[i]--;
				if (lives[i] == 0) {
					Mix_PlayChannel(-1, nemonaDeadsfx, 0);
					enemies[i]->OnCollision(c2);
					delete enemies[i];
					enemies[i] = nullptr;
					App->current_stage->right = true;
					App->current_stage->wallboss = true;
				}
				else {
					Mix_PlayChannel(-1, hitEnemysfx, 0);
					enemies[i]->Shine();
				}
				break;
			default:
				break;
			}
		}
	}
}

Enemy* ModuleEnemies::SpawnEnemyRet(const EnemyInfo& info)
{
	// find room for the new enemy
	uint i = 0;
	for (; enemies[i] != nullptr && i < MAX_ENEMIES; ++i);

	if (i != MAX_ENEMIES)
	{
		switch (info.type)
		{
		case ENEMY_TYPES::WORM:
			enemies[i] = new Enemy_Worm(info.x, info.y, info.going_up);
			lives[i] = 2;
			break;
		case ENEMY_TYPES::WORMBODY:
			enemies[i] = new Enemy_WormBody(info.x, info.y, info.going_up, info.tail);
			lives[i] = 1;
			break;
		case ENEMY_TYPES::HOSTUR:
			enemies[i] = new Enemy_Hostur(info.x, info.y);
			lives[i] = 160;
			break;
		}
	}

	return enemies[i];
}

void ModuleEnemies::Kill(Enemy* e) {
	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (enemies[i] == e) {
			switch (enemies[i]->type) {
			case HOSTUR:
				Mix_PlayChannel(-1, nemonaDeadsfx, 0);
				enemies[i]->OnCollision(nullptr);
				delete enemies[i];
				enemies[i] = nullptr;
				App->ui->StageCleared();
				return;
				break;
			}
		}
	}
}