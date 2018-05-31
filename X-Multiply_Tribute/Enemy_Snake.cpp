#include "Application.h"
#include "ModuleCollision.h"
#include "ModuleParticles.h"
#include "Enemy_Snake.h"
#include "Enemy_WormBody.h"
#include "ModuleEnemies.h"
#include <stdlib.h> 


Enemy_Snake::Enemy_Snake(int x, int y, int id) :Enemy(x, y)
{
	type = SNAKE;

	w = 44;
	h = 32;

	center = { 22, 12 };
	
	anim.PushBack({ 40, 1652, 44, 32 });

	hitanim.PushBack({ 343, 1652, 44, 32 });

	animation = &anim;
	hitAnimation = &hitanim;
	original_position = { x, y };

	collider = App->collision->AddCollider({ 0, 0, 42, 32 }, COLLIDER_TYPE::COLLIDER_ENEMY, (Module*)App->enemies);
	state = START;
	points = 100;

	
}


void Enemy_Snake::Move()
{
	if (!createdBody) {
		createdBody = true;
		EnemyInfo info;
		info.type = ENEMY_TYPES::SNAKEBODY;
		info.x = position.x + 12;
		info.y = position.y;
		info.powerUpid = 1;
		Enemy* e = App->enemies->SpawnEnemyRet(info);
		nextPart = (Enemy_SnakeBody*)e;
		
	}
	
	position.x -=1;
}

void Enemy_Snake::OnCollision(Collider * collider)
{
	OnDeath();
}

void Enemy_Snake::Shine()
{
	nextPart->Shine();
}

void Enemy_Snake::OnDeath()
{
	//next part explodes after 4 frames;
}