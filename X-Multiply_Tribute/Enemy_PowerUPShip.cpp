#include "Application.h"
#include "ModuleCollision.h"
#include "ModuleParticles.h"
#include "ModuleParticles.h"
#include "ModulePlayer.h"
#include "Enemy_PowerUPShip.h"
#include "ModulePowerUPS.h"


Enemy_PowerUPShip::Enemy_PowerUPShip(int x, int y, int powerUpid):Enemy(x,y)
{
	original_y = y;
	original_x = x;

	anim.PushBack({ 399, 102, 28, 20 });
	anim.PushBack({ 431, 102, 31, 20 });
	anim.speed = 0.2f;
	anim.loop = true;

	animation = &anim;

	collider = App->collision->AddCollider({ 0, 0, 31, 20 }, COLLIDER_TYPE::COLLIDER_ENEMY, (Module*)App->enemies);

	switch (powerUpid)
	{
	case 1: pu = App->powerups->AddPowerUp(App->powerups->tentacle, position.x+2, position.y + 16, COLLIDER_POWER_UP); break;
	case 2: pu = App->powerups->AddPowerUp(App->powerups->bomb, position.x+2, position.y + 16, COLLIDER_POWER_UP); break;
	case 3: pu = App->powerups->AddPowerUp(App->powerups->speedBoost, position.x+2, position.y + 16, COLLIDER_POWER_UP); break;
	case 4: pu = App->powerups->AddPowerUp(App->powerups->speedDown, position.x+2, position.y + 16, COLLIDER_POWER_UP); break;
	case 5: pu = App->powerups->AddPowerUp(App->powerups->life, position.x+2, position.y + 16, COLLIDER_POWER_UP); break;
	}

	LOG("HI");
}

void Enemy_PowerUPShip::OnCollision(Collider * collider)
{
	if(pu != nullptr)pu->enabled = true;
}

void Enemy_PowerUPShip::Move()
{
	position.x--;
}