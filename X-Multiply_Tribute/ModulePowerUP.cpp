#include "ModulePowerUP.h"
#include <math.h>
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModulePlayer.h"
#include "ModuleCollision.h"
#include "ModuleParticles.h"

#include "SDL/include/SDL_timer.h"



ModulePowerUP::ModulePowerUP()
{
	for (uint i = 0; i < MAX_POWERUP; ++i)
		poweUps[i] = nullptr;
}


ModulePowerUP::~ModulePowerUP()
{
}

bool ModulePowerUP::Init()
{
	return false;
}

bool ModulePowerUP::Start()
{
	return false;
}

update_status ModulePowerUP::Update()
{
	return update_status();
}

bool ModulePowerUP::CleanUp()
{
	return false;
}

PowerUp::PowerUp(iPoint _position)
{
}
