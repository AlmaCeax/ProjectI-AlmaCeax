#include <math.h>
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModulePlayer.h"
#include "ModuleCollision.h"
#include "ModulePowerUPS.h"

#include "SDL/include/SDL_timer.h"


ModulePowerUPS::ModulePowerUPS()
{
	for (uint i = 0; i < MAX_POWERUPS; ++i)
		poweUps[i] = nullptr;
}

ModulePowerUPS::~ModulePowerUPS()
{
}

bool ModulePowerUPS::Init()
{
	return false;
}

bool ModulePowerUPS::Start()
{
	return false;
}

update_status ModulePowerUPS::Update()
{
	return update_status();
}

bool ModulePowerUPS::CleanUp()
{
	return false;
}

void ModulePowerUPS::OnCollision(Collider * c1, Collider * c2)
{
}
