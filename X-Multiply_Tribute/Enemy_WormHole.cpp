#include "Application.h"
#include "ModuleCollision.h"
#include "ModuleParticles.h"
#include "ModulePlayer.h"
#include "Enemy_WormHole.h"
#include "ModuleEnemies.h"




Enemy_WormHole::Enemy_WormHole(int x, int y) :Enemy(x, y)
{
	type = WORMHOLE;

	anim.PushBack({ 168, 488, 64, 53 });
	anim.PushBack({ 168, 550, 64, 53 });
	anim.PushBack({ 168, 622, 64, 53 });

	hitanim.PushBack({ 655, 488, 64, 53 });
	hitanim.PushBack({ 655, 549, 64, 53 });
	hitanim.PushBack({ 655, 624, 64, 53 });

	anim.speed = 0.05f;

	animation = &anim;
	hitAnimation = &hitanim;

	collider = App->collision->AddCollider({ 0, 0, 64, 52 }, COLLIDER_TYPE::COLLIDER_ENEMY, (Module*)App->enemies);

	points = 2000;
}

void Enemy_WormHole::Move()
{
	
}