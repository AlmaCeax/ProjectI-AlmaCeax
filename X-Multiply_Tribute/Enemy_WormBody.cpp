#include "Application.h"
#include "ModuleCollision.h"
#include "ModuleParticles.h"
#include "Enemy_WormBody.h"
#include "ModuleEnemies.h"


Enemy_WormBody::Enemy_WormBody(int x, int y, bool _up, bool tail) :Enemy(x, y)
{
	type = WORMBODY;

	up = _up;
	speed = {0,1};
	if (up) {
		flipY = true;
	}

	if (!tail)
	{
		anim.PushBack({ 59, 636, 16, 16 });
		anim.PushBack({ 76, 635, 16, 16 });
		anim.PushBack({ 94, 635, 14, 16 });

		hitanim.PushBack({ 548, 636, 16, 16 });
		hitanim.PushBack({ 565, 635, 16, 16 });
		hitanim.PushBack({ 583, 635, 16, 16 });
	}
	else {
		anim.PushBack({ 110, 636, 16, 16 });
		anim.PushBack({ 63, 654, 16, 16 });
		anim.PushBack({ 45, 654, 16, 16 });

		hitanim.PushBack({ 600, 636, 16, 16 });
		hitanim.PushBack({ 654, 635, 16, 16 });
		hitanim.PushBack({ 534, 654, 16, 16 });
	}

	anim.speed = 0.f;

	animation = &anim;
	hitAnimation = &hitanim;

	collider = App->collision->AddCollider({ 0, 0, 16, 16 }, COLLIDER_TYPE::COLLIDER_ENEMY, (Module*)App->enemies);

	points = 100;
}


void Enemy_WormBody::Move()
{
	if (up)position += speed;
	else position-=speed;
}

