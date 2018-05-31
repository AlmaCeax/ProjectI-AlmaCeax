#include "Application.h"
#include "ModuleCollision.h"
#include "ModuleParticles.h"
#include "Enemy_SnakeBody.h"
#include "Enemy_WormBody.h"
#include "ModuleEnemies.h"
#include <stdlib.h> 


Enemy_SnakeBody::Enemy_SnakeBody(int x, int y, int id): Enemy(x, y)
{
	type = SNAKEBODY;

	w = 32;
	h = 32;

	if (id < 20) {
		center = { 16, 16 };

		anim.PushBack({ 179, 1835, 32, 32 });
		hitanim.PushBack({ 482, 1835, 32, 32 });
	}
	else {
		center = { 24, 15 };
		anim.PushBack({ 176, 1652, 48, 29 });
		hitanim.PushBack({ 479, 1652, 48, 29 });
	}

	animation = &anim;
	hitAnimation = &hitanim;

	collider = App->collision->AddCollider({ 0, 0, 32, 32 }, COLLIDER_TYPE::COLLIDER_ENEMY, (Module*)App->enemies);
	points = 100;
}


void Enemy_SnakeBody::Move()
{

	if (startTime < 80 && startTime % 4 == 0) {
		EnemyInfo info;
		info.type = ENEMY_TYPES::SNAKEBODY;
		info.x = position.x;
		info.y = position.y;
		info.powerUpid = startTime;
		Enemy* e = App->enemies->SpawnEnemyRet(info);
		nextPart = (Enemy_SnakeBody*)e;
	}
	
	if (startTime > 16)
	{
		position.x--;
	}
	startTime++;
}

void Enemy_SnakeBody::Shine()
{
	if(nextPart != nullptr)nextPart->Shine();
}

void Enemy_SnakeBody::OnDeath()
{
	if (death)
	{

	}
}