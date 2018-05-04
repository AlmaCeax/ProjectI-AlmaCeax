#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "ModuleEnemies.h"
#include "ModuleParticles.h"
#include "ModuleTextures.h"
#include "Enemy_HellBall.h"
#include "Enemy_FlyingWorm.h"
#include "Enemy_TentacleShooter.h"
#include "Enemy_PowerUPShip.h"
#include "SDL_mixer\include\SDL_mixer.h"
#include "Enemy.h"

#define SPAWN_MARGIN 60

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
	return true;
}

update_status ModuleEnemies::PreUpdate()
{
	// check camera position to decide what to spawn
	for(uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if(queue[i].type != ENEMY_TYPES::NO_TYPE)
		{
			if (queue[i].x * SCREEN_SIZE < App->render->camera.x + (App->render->camera.w * SCREEN_SIZE) + SPAWN_MARGIN)
			{
				SpawnEnemy(queue[i]);
				queue[i].type = ENEMY_TYPES::NO_TYPE;
				LOG("Spawning enemy at %d", queue[i].x * SCREEN_SIZE);
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
			if(enemies[i]->position.x * SCREEN_SIZE < (App->render->camera.x) - SPAWN_MARGIN)
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

bool ModuleEnemies::AddEnemy(ENEMY_TYPES type, int x, int y, bool going_up, int powerUPid)
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
			queue[i].powerUpid = powerUPid;
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
		}
	}
}

void ModuleEnemies::OnCollision(Collider* c1, Collider* c2)
{
	for(uint i = 0; i < MAX_ENEMIES; ++i)
	{
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
				//Mix_PlayChannel(-1, hellballDeadsfx, 0);
				enemies[i]->OnCollision(c2);
				delete enemies[i];
				enemies[i] = nullptr;
				break;
			case FLYINGWORM:
				//Mix_PlayChannel(-1, flyerDeadsfx, 0);
				enemies[i]->OnCollision(c2);
				delete enemies[i];
				enemies[i] = nullptr;
				break;
			case TENTACLESHOOTER:
				lives[i]--;
				if (lives[i] == 0) {
					//Mix_PlayChannel(-1, nemonaDeadsfx, 0);
					enemies[i]->OnCollision(c2);
					delete enemies[i];
					enemies[i] = nullptr;
				}
				else {
					enemies[i]->Shine(c2);
				}
				break;
			case POWERUPSHIP:
				Mix_PlayChannel(-1, pushipDeadsfx, 0);
				enemies[i]->OnCollision(c2);
				delete enemies[i];
				enemies[i] = nullptr;
				break;
			default:
				break;
			}
 			
			break;
		}
	}
}