#include "Application.h"
#include "ModuleCollision.h"
#include "Enemy_HellBall.h"
#include "ModuleEnemies.h"


Enemy_HellBall::Enemy_HellBall(int x, int y, bool _going_up):Enemy(x,y)
{
	w = 24;
	h = 27;
	type = HELLBALL;
	open_mouth.PushBack({ 241, 8, 24, 27 });
	open_mouth.PushBack({ 271, 8, 24, 27 });
	open_mouth.PushBack({ 302, 8, 24, 27 });
	open_mouth.speed = 0.2f;

	origin = { x-radius,y+radius };

	going_up = _going_up;
	
	animation = &open_mouth;

	collider = App->collision->AddCollider({ 0, 0, 24, 27 }, COLLIDER_TYPE::COLLIDER_ENEMY, (Module*)App->enemies);

	points = 100;
}

void Enemy_HellBall::Move()
{
	angle += 0.06f;
	
	if (origin.x - 140 == position.x) {
		change = false;
	}

	if ( change) {
		if (going_up) position.y = origin.y + 2 * sinf(angle) * radius;
		else position.y = origin.y + 2 * sinf(-angle) * radius;

		position.x -= 1;
	}else {
		if (going_up) position.y = origin.y + 2 * sinf(angle) * radius;
		else position.y = origin.y + 2 * sinf(-angle) * radius;

		position.x += 3;
	}
	
}
