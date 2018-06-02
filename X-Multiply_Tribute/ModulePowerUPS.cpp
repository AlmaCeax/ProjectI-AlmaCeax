#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModulePlayer.h"
#include "ModuleCollision.h"
#include "ModulePowerUPS.h"
#include "Enemy_PowerUPShip.h"
#include "ModuleParticles.h"
#include "ModuleUI.h"
#include "ModuleAudio.h"
#include "SDL_mixer\include\SDL_mixer.h"


ModulePowerUPS::ModulePowerUPS()
{
	for (uint i = 0; i < MAX_POWERUPS; ++i)
		powerUps[i] = nullptr;

	speedBoost.anim.PushBack({68, 74, 24, 13});
	speedBoost.anim.PushBack({164, 74, 24, 13});
	speedBoost.anim.loop = true;
	speedBoost.anim.speed = 0.1f;
	speedBoost.type = SPEED_BOOST;
	speedBoost.enabled = false;

	speedDown.anim.PushBack({ 36, 74, 24, 13 });
	speedDown.anim.PushBack({ 132, 74, 24, 13 });
	speedDown.anim.loop = true;
	speedDown.anim.speed = 0.1f;
	speedDown.type = SPEED_DOWN;
	speedDown.enabled = false;

	life.anim.PushBack({ 4, 79, 24, 18 });
	life.anim.PushBack({ 100, 79, 24, 18 });
	life.anim.loop = true;
	life.anim.speed = 0.1f;
	life.type = LIFE;
	life.enabled = false;

	bomb.anim.PushBack({ 229, 56, 21, 16 });
	bomb.anim.PushBack({ 101, 56, 21, 16 });
	bomb.anim.loop = true;
	bomb.anim.speed = 0.1f;
	bomb.type = BOMB;
	bomb.enabled = false;

	laser.anim.PushBack({ 5, 57, 18, 15 });
	laser.anim.loop = true;
	laser.anim.speed = 0.1f;
	laser.type = LASER;
	laser.enabled = false;

	shadow.anim.PushBack({37,57,18,15});
	shadow.anim.PushBack({ 165,57,18,15 });
	shadow.anim.loop = true;
	shadow.anim.speed = 0.1f;
	shadow.type = SHADOW;
	shadow.enabled = false;

	missile.anim.PushBack({ 69,57,18,15 });
	missile.anim.PushBack({ 197,57,18,15 });
	missile.anim.loop = true;
	missile.anim.speed = 0.1f;
	missile.type = MISSILE;
	missile.enabled = false;

}

ModulePowerUPS::~ModulePowerUPS()
{
}

bool ModulePowerUPS::Init()
{
	return true;
}

bool ModulePowerUPS::Start()
{
	LOG("Loading particles");
	graphics = App->textures->Load("Assets/Sprites/PowerUP/spr_powerup.png");

	bomb.sfx = App->audio->LoadFx("Assets/Audio/SFX/xmultipl-021.wav");
	laser.sfx = App->audio->LoadFx("Assets/Audio/SFX/xmultipl-021.wav");
	life.sfx = App->audio->LoadFx("Assets/Audio/SFX/xmultipl-023.wav");
	shadow.sfx = bomb.sfx;
	missile.sfx = bomb.sfx;

	if (!graphics)return false;
	
	return true;
}

update_status ModulePowerUPS::Update()
{
	for (uint i = 0; i < MAX_POWERUPS; ++i)
	{
		PowerUP* pu = powerUps[i];

		if (pu == nullptr)
			continue;

		pu->Update();
		
		App->render->Blit(graphics, pu->position.x, pu->position.y, &(pu->anim.GetCurrentFrame()));
	}
	return UPDATE_CONTINUE;
}

bool ModulePowerUPS::CleanUp()
{
	LOG("Unloading particles");
	App->textures->Unload(graphics);
	graphics = nullptr;

	for (uint i = 0; i < MAX_POWERUPS; ++i)
	{
		if (powerUps[i] != nullptr)
		{
			delete powerUps[i];
			powerUps[i] = nullptr;
		}
	}

	App->audio->UnloadSFX(bomb.sfx);
	bomb.sfx = nullptr;
	App->audio->UnloadSFX(laser.sfx);
	bomb.sfx = nullptr;
	App->audio->UnloadSFX(life.sfx);
	bomb.sfx = nullptr;

	return true;
}

void ModulePowerUPS::OnCollision(Collider * c1, Collider * c2)
{
	for (uint i = 0; i < MAX_POWERUPS; ++i)
	{
		// Always destroy powerUps that collide
		if (powerUps[i] != nullptr && powerUps[i]->collider == c1 && powerUps[i]->enabled)
		{
			powerUps[i]->Effect();
			delete powerUps[i];
			powerUps[i] = nullptr;
			break;
		}
	}
}

PowerUP* ModulePowerUPS::AddPowerUp(const PowerUP & powerup, int x, int y, COLLIDER_TYPE collider_type)
{
	for (uint i = 0; i < MAX_POWERUPS; ++i)
	{
		if (powerUps[i] == nullptr)
		{
			PowerUP* pu = new PowerUP(powerup);
			pu->position.x = x;
			pu->position.y = y;
			if (collider_type != COLLIDER_NONE)
				pu->collider = App->collision->AddCollider(pu->anim.GetCurrentFrame(), collider_type, this);
			powerUps[i] = pu;
			return pu;
			break;
		}
	}
}


PowerUP::PowerUP()
{
}

PowerUP::PowerUP(const PowerUP& pu) : 
	anim(pu.anim), position(pu.position), type(pu.type), enabled(pu.enabled)
{}

PowerUP::~PowerUP()
{
	if (collider != nullptr)
		collider->to_delete = true;
}

void PowerUP::Effect()
{
	switch (type)
	{
	case LASER:
		if (!App->player->activePU[TENTACLES])Tentacles();
		else {
			App->player->activePU[LASER] = true;
			App->player->activePU[SHADOW] = false;
			App->player->activePU[MISSILE] = false;
		}
		break;		
	case LIFE: 
		Mix_PlayChannel(-1, App->powerups->life.sfx, 0);
		App->ui->IncreaseLife();
		break;		
	case SPEED_BOOST: App->player->speed.x += 0.3f;
		App->player->speed.y += 0.3f;
		App->particles->AddParticle(App->particles->playerBoost, App->player->position.x -42, App->player->position.y, COLLIDER_NONE);
		Mix_PlayChannel(-1, App->particles->playerBoost.sfx, 0);
		break;		
	case SPEED_DOWN: App->player->speed.x -= 0.3f;
		App->player->speed.y -= 0.3f;
		App->particles->AddParticle(App->particles->playerBoost, App->player->position.x -42, App->player->position.y, COLLIDER_NONE);
		break;		
	case BOMB: 
		Mix_PlayChannel(-1, App->powerups->bomb.sfx, 0);
		App->player->activePU[BOMB] = true;
		break;
	case SHADOW:
		if (!App->player->activePU[TENTACLES])Tentacles();
		else {
			App->player->activePU[SHADOW] = true;
			App->player->activePU[MISSILE] = false;
			App->player->activePU[LASER] = false;
		}
		Mix_PlayChannel(-1,App->powerups->shadow.sfx, 0);
		break;
	case MISSILE:
		if (!App->player->activePU[TENTACLES])Tentacles();
			App->player->activePU[MISSILE] = true;
			App->player->activePU[LASER] = false;
			App->player->activePU[SHADOW] = false;
		Mix_PlayChannel(-1, App->powerups->missile.sfx, 0);
		break;
	}
}

void PowerUP::Tentacles() 
{
	App->player->activePU[TENTACLES] = true;
	App->player->tentacle.collider->enable = true;
	App->player->tentacle2.collider->enable = true;
	Mix_PlayChannel(-1, App->powerups->tentacle.sfx, 0);
}

bool PowerUP::Update()
{
	if (collider != nullptr)
		collider->SetPos(position.x, position.y);

	if (!enabled)
	{
		position.x--;
	}

	return true;
}
