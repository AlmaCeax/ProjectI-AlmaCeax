#include "Application.h"
#include "ModuleCollision.h"
#include "ModuleParticles.h"
#include "Enemy_Cyclop.h"


Enemy_Cyclop::Enemy_Cyclop(int x, int y): Enemy(x,y)
{
	type = TENTACLESHOOTER;
	anim.PushBack({ 28, 488, 42, 57 });
	anim.PushBack({ 86, 488, 42, 57 });

	hitanim.PushBack({ 512, 488, 42, 57 });
	hitanim.PushBack({ 570, 488, 42, 57 });

	anim.speed = 0.01f;
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
		if (readyToShoot) {
			App->particles->AddParticle(App->particles->cyclopshot, position.x, position.y, COLLIDER_ENEMY_SHOT, { -1, -2 });
			App->particles->AddParticle(App->particles->cyclopshot, position.x, position.y, COLLIDER_ENEMY_SHOT, { -2, -1 });
			App->particles->AddParticle(App->particles->cyclopshot, position.x, position.y, COLLIDER_ENEMY_SHOT, { -1, -1 });
			readyToShoot = false;
		}
	}

	if (anim.GetCurrentFrameIndex() == 0)readyToShoot = true;
}
