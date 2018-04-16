#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModulePlayer.h"
#include "ModuleCollision.h"
#include "ModulePowerUPS.h"


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

	bomb.anim.PushBack({ 229, 6, 21, 16 });
	bomb.anim.PushBack({ 100, 56, 21, 16 });
	bomb.anim.loop = true;
	bomb.anim.speed = 0.1f;
	bomb.type = BOMB;
	bomb.enabled = false;

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

	for (uint i = 0; i < MAX_POWERUPS; ++i)
	{
		if (powerUps[i] != nullptr)
		{
			delete powerUps[i];
			powerUps[i] = nullptr;
		}
	}

	return true;
}

void ModulePowerUPS::OnCollision(Collider * c1, Collider * c2)
{
	for (uint i = 0; i < MAX_POWERUPS; ++i)
	{
		// Always destroy powerUps that collide
		if (powerUps[i] != nullptr && powerUps[i]->collider == c1)
		{
			powerUps[i]->Effect();
			delete powerUps[i];
			powerUps[i] = nullptr;
			break;
		}
	}
}

void ModulePowerUPS::AddPowerUp(const PowerUP & powerup, int x, int y, COLLIDER_TYPE collider_type)
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
}

void PowerUP::Effect()
{
	switch (type)
	{
	case LASER: 
		break;		
	case LIFE: 
		break;		
	case SPEED_BOOST: App->player->speed += 1;
		LOG("SPEEEEEEEEED BOOOOOOOOOOOOOOST");
		break;		
	case SPEED_DOWN: App->player->speed -= 1;
		break;		
	case BOMB: 
		break;
	case SHADOW:
		break;
	}
}

bool PowerUP::Update()
{
	if (collider != nullptr)
		collider->SetPos(position.x, position.y);

	return true;
}
