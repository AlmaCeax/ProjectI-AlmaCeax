#include "Application.h"
#include "ModuleCollision.h"
#include "ModuleParticles.h"
#include "ModulePlayer.h"
#include "Enemy_GreenEye.h"
#include "ModuleEnemies.h"




Enemy_GreenEye::Enemy_GreenEye(int x, int y) :Enemy(x, y)
{
	type = GREENEYE;
	original_y = y;
	original_x = x;

	points = 200;

	greeneye.PushBack({ 155, 69, 46, 35 });
	greeneye.PushBack({ 221, 70, 42, 34 });
	greeneye.PushBack({ 188, 117, 47, 36 });
	greeneye.speed = 0.2f;

	animation = &greeneye;

	collider = App->collision->AddCollider({ 0, 0, 46, 35 }, COLLIDER_TYPE::COLLIDER_ENEMY, (Module*)App->enemies);
}

void Enemy_GreenEye::Move()
{
	position.x -= 2;
}
