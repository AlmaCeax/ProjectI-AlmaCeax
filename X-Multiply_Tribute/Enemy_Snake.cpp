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

	for (int i = 0; i < 20; i++)
	{
		childs[i] = nullptr;
	}
	original_position = { x, y };

	circleCenter = { x, y };
	center = { 22, 12 };
	
	anim.PushBack({ 40, 1652, 44, 32 });

	hitanim.PushBack({ 343, 1652, 44, 32 });

	animation = &anim;
	hitAnimation = &hitanim;

	collider = App->collision->AddCollider({ 0, 0, 42, 32 }, COLLIDER_TYPE::COLLIDER_ENEMY, (Module*)App->enemies);
	state = START;
	points = 100;

}


void Enemy_Snake::Move()
{
	if (indexchild < 10)
	{
		/*if (spawntime == 16)
		{
			EnemyInfo info;
			info.type = ENEMY_TYPES::SNAKEBODY;
			info.x = original_position.x;
			info.y = original_position.y;
			info.powerUpid = indexchild;
			Enemy* e = App->enemies->SpawnEnemyRet(info);
			childs[indexchild] = (Enemy_SnakeBody*)e;
			spawntime = 0;
			indexchild++;
		}
		else {
			spawntime++;
		}*/
	}
	angle += 0.01f;
	switch (state)
	{
	case Enemy_Snake::START:
		position.y = original_position.y + sinf(angle) * radius;
		position.x-=1;
		if (position.x == original_position.x - 100)
		{
			state = CIRCLE;
			circleCenter = { position.x + radius, position.y };
		}
		break;
	case Enemy_Snake::CIRCLE:
		position.y = circleCenter.y + sinf(angle) * radius;
		position.x = circleCenter.x - cosf(angle) * radius;
		circleCenter.x += 1;
		//circleCenter.y += 1;
		//rangle -= 1.2f;
		break;
	case Enemy_Snake::END:
		break;
	case Enemy_Snake::NONE:
		break;
	default:
		break;
	}
}

void Enemy_Snake::OnCollision(Collider * collider)
{
	/*OnDeath();*/
}

void Enemy_Snake::Shine()
{
	/*for each (Enemy_SnakeBody* sb in childs)
	{
		sb->Shine();
	}*/
}

void Enemy_Snake::OnDeath()
{
	//next part explodes after 4 frames;
}