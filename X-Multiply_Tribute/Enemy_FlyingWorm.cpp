#include "Application.h"
#include "ModuleCollision.h"
#include "Enemy_FlyingWorm.h"



Enemy_FlyingWorm::Enemy_FlyingWorm(int x, int y):Enemy(x,y)
{
	original_y = y;

	fly.PushBack({ 40, 75, 34, 30 });
	fly.PushBack({ 84, 73, 37, 33 });
	fly.PushBack({ 35, 127, 42, 31 });
	fly.PushBack({ 90, 129, 43, 26 });
	fly.speed = 0.2f;

	animation = &fly;

	collider = App->collision->AddCollider({ 0, 0, 37, 33 }, COLLIDER_TYPE::COLLIDER_ENEMY, (Module*)App->enemies);
}

void Enemy_FlyingWorm::Move()
{
	/*if (going_up)
	{
		if (wave > 1.0f)
			going_up = false;
		else
			wave += 0.08f;
	}
	else
	{
		if (wave < -1.0f)
			going_up = true;
		else
			wave -= 0.08f;
	}

	position.y = int(float(original_y) + (40.0f * sinf(wave)));*/
	wave += 0.1f;
	position.y = original_y + sinf(wave) * 20;
	position.x -= 1;
}
