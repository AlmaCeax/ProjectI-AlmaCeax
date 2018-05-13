#include "Application.h"
#include "ModuleCollision.h"
#include "ModuleParticles.h"
#include "Enemy_Worm.h"
#include "ModuleEnemies.h"


Enemy_Worm::Enemy_Worm(int x, int y, bool _up) :Enemy(x, y)
{
	type = WORM;

	up = _up;

	if (up) {
		flipY = true;
	}
	else
	{

	}

	anim.PushBack({ 23, 636, 16, 16 });
	anim.PushBack({ 42, 635, 16, 16 });
	anim.PushBack({ 26, 653, 16, 16 });

	hitanim.PushBack({ 512, 636, 16, 16 });
	hitanim.PushBack({ 531, 635, 16, 16 });
	hitanim.PushBack({ 515, 653, 16, 16 });

	anim.speed = 0.f;

	animation = &anim;
	hitAnimation = &hitanim;

	collider = App->collision->AddCollider({ 0, 0, 16, 16 }, COLLIDER_TYPE::COLLIDER_ENEMY, (Module*)App->enemies);

	points = 100;
}


void Enemy_Worm::Move()
{
	if(up)position.y++;
	else position.y--;
}