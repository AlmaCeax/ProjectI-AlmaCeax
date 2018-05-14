#include "Application.h"
#include "ModuleCollision.h"
#include "ModuleParticles.h"
#include "Enemy_WormBody.h"
#include "ModuleEnemies.h"


Enemy_WormBody::Enemy_WormBody(int x, int y, bool _up) :Enemy(x, y)
{
	type = WORM;

	up = _up;

	if (up) {
		flipY = true;
	}
	else
	{

	}

	if (!tail)
	{
		anim.PushBack({ 59, 636, 16, 14 });
		anim.PushBack({ 76, 635, 16, 16 });
		anim.PushBack({ 94, 635, 14, 16 });

		hitanim.PushBack({ 548, 636, 16, 16 });
		hitanim.PushBack({ 565, 635, 16, 16 });
		hitanim.PushBack({ 583, 635, 16, 16 });
	}
	else {
		anim.PushBack({ 59, 636, 16, 14 });
		anim.PushBack({ 76, 635, 16, 16 });
		anim.PushBack({ 94, 635, 14, 16 });

		hitanim.PushBack({ 548, 636, 16, 16 });
		hitanim.PushBack({ 565, 635, 16, 16 });
		hitanim.PushBack({ 583, 635, 16, 16 });
	}

	anim.speed = 0.f;

	animation = &anim;
	hitAnimation = &hitanim;

	collider = App->collision->AddCollider({ 0, 0, 16, 16 }, COLLIDER_TYPE::COLLIDER_ENEMY, (Module*)App->enemies);

	points = 100;
}


void Enemy_WormBody::Move()
{
	if (up)position.y++;
	else position.y--;
}