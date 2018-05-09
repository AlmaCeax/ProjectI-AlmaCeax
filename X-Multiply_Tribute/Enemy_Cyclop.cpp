#include "Application.h"
#include "ModuleCollision.h"
#include "ModuleParticles.h"
#include "Enemy_Cyclop.h"
#include <stdlib.h> 


Enemy_Cyclop::Enemy_Cyclop(int x, int y, bool _up): Enemy(x,y)
{
	numExplosions = 4;
	type = TENTACLESHOOTER;
	up = _up;
	if (_up)
	{
		anim.PushBack({ 86, 562, 42, 57 });
		anim.PushBack({ 28, 562, 42, 57 });

		hitanim.PushBack({ 570, 562, 42, 57 });
		hitanim.PushBack({ 512, 562, 42, 57 });
	}
	else {
		anim.PushBack({ 28, 488, 42, 57 });
		anim.PushBack({ 86, 488, 42, 57 });

		hitanim.PushBack({ 512, 488, 42, 57 });
		hitanim.PushBack({ 570, 488, 42, 57 });
	}

	anim.speed = 0.03f;
	anim.loop = true;

	animation = &anim;
	hitAnimation = &hitanim;

	collider = App->collision->AddCollider({ 0, 0, 48, 41 }, COLLIDER_TYPE::COLLIDER_ENEMY, (Module*)App->enemies);

	points = 600;
	
}

void Enemy_Cyclop::Move()
{
	Particle part;
	if (anim.GetCurrentFrameIndex() == 1)
	{
		int numshots = rand() % 6+ 2;
		if (readyToShoot) {
			if (up)
			{
				for (int i = 0; i < numshots; i++)App->particles->AddParticle(App->particles->cyclopshot, position.x+30, position.y+20, COLLIDER_ENEMY_SHOT, { (float)(+(rand() % 2 + 1)), 0.f });
			}
			else {
				for (int i = 0; i < numshots; i++)App->particles->AddParticle(App->particles->cyclopshot, position.x, position.y, COLLIDER_ENEMY_SHOT, { (float)(-(rand() % 2 + 1)), (float)(-(rand() % 3 + 1)) });
			}
			readyToShoot = false;
		}
	}

	if (anim.GetCurrentFrameIndex() == 0)readyToShoot = true;
}
