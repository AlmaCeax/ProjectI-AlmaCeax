#include "Application.h"
#include "ModuleCollision.h"
#include "ModuleParticles.h"
#include "ModulePlayer.h"
#include "Enemy_WormHole.h"
#include "ModuleEnemies.h"




Enemy_WormHole::Enemy_WormHole(int x, int y, bool up) :Enemy(x, y)
{
	type = WORMHOLE;
	this->up = up;
	if (up)flipY = true;

	anim.PushBack({ 168, 488, 64, 53 });
	anim.PushBack({ 168, 550, 64, 53 });
	anim.PushBack({ 168, 622, 64, 53 });
	anim.PushBack({ 168, 550, 64, 53 });
	anim.PushBack({ 168, 488, 64, 53 });

	hitanim.PushBack({ 655, 488, 64, 53 });
	hitanim.PushBack({ 655, 550, 64, 53 });
	hitanim.PushBack({ 655, 622, 64, 53 });
	hitanim.PushBack({ 655, 550, 64, 53 });
	hitanim.PushBack({ 655, 488, 64, 53 });

	anim.loop = false;

	anim.speed = 0.0f;

	animation = &anim;
	hitAnimation = &hitanim;

	collider = App->collision->AddCollider({ 0, 0, 64, 53 }, COLLIDER_TYPE::COLLIDER_ENEMY, (Module*)App->enemies);

	points = 2000;
}

void Enemy_WormHole::Move()
{
	if (anim.GetCurrentFrameIndex() == 2 && canSpawn)
	{
		if (up){
			EnemyInfo info;
			info.type = ENEMY_TYPES::WORM;
			info.x = position.x + 26;
			info.y = position.y + 50;
			info.going_up = true;
			App->enemies->SpawnEnemyRet(info);
		}
		else {
			EnemyInfo info;
			info.type = ENEMY_TYPES::WORM;
			info.x = position.x + 26;
			info.y = position.y;
			info.going_up = false;
			App->enemies->SpawnEnemyRet(info);
		}
		canSpawn = false;
	}if (anim.GetCurrentFrameIndex() == 1)
	{
		canSpawn = true;
	}

	if (timetoshoot == 70)
	{
		anim.speed = 0.1f;
	}
	else timetoshoot++;
}