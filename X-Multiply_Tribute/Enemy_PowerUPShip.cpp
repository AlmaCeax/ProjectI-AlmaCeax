#include "Application.h"
#include "ModuleCollision.h"
#include "ModuleParticles.h"
#include "ModuleParticles.h"
#include "ModulePowerUPS.h"
#include "ModulePlayer.h"
#include "Enemy_PowerUPShip.h"


Enemy_PowerUPShip::Enemy_PowerUPShip(int x, int y) : Enemy(x, y)
{
	original_y = y;
	original_x = x;

	anim.PushBack({ 399, 103, 28, 19 });
	anim.PushBack({ 431, 102, 31, 20 });
	anim.speed = 0.2f;
	anim.loop = true;

	animation = &anim;

	collider = App->collision->AddCollider({ 0, 0, 31, 20 }, COLLIDER_TYPE::COLLIDER_ENEMY, (Module*)App->enemies);

	App->powerups->AddPowerUp(App->powerups->speedBoost, position.x, position.y+20, COLLIDER_POWER_UP);

	points = 1000;
}

void Enemy_PowerUPShip::OnCollision(Collider * collider)
{

}

void Enemy_PowerUPShip::Move()
{
	position.x--;
}
