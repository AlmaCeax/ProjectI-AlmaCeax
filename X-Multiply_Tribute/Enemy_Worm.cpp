#include "Application.h"
#include "ModuleCollision.h"
#include "ModuleParticles.h"
#include "Enemy_Worm.h"
#include "ModuleEnemies.h"


Enemy_Worm::Enemy_Worm(int x, int y, bool _up) :Enemy(x, y)
{
	type = WORM;

	up = _up;

	originY = y;

	for (int i = 0; i < 8; i++)
	{
		bodies[i] = nullptr;
	}
	anim.PushBack({ 23, 636, 16, 16 });
	anim.PushBack({ 42, 635, 16, 16 });
	anim.PushBack({ 26, 653, 16, 16 });

	hitanim.PushBack({ 512, 636, 16, 16 });
	hitanim.PushBack({ 531, 635, 16, 16 });
	hitanim.PushBack({ 515, 653, 16, 16 });

	anim.speed = 0.f;
	spawntime = 0;
	indexchild = 0;
	animation = &anim;
	hitAnimation = &hitanim;

	collider = App->collision->AddCollider({ 0, 0, 16, 16 }, COLLIDER_TYPE::COLLIDER_ENEMY, (Module*)App->enemies);

	points = 100;

	if (up) {
		flipY = true;
	}
}


void Enemy_Worm::Move()
{
	if (indexchild < 8)
	{
		if (spawntime == 8)
		{
			if (up) {
				if (indexchild < 7) {
					EnemyInfo info;
					info.type = ENEMY_TYPES::WORMBODY;
					info.x = position.x;
					info.y = originY;
					info.going_up = true;
					info.tail = false;
					bodies[indexchild] = App->enemies->SpawnEnemyRet(info);
				}
				else {
					EnemyInfo info;
					info.type = ENEMY_TYPES::WORMBODY;
					info.x = position.x;
					info.y = originY;
					info.going_up = true;
					info.tail = true;
					bodies[indexchild] = App->enemies->SpawnEnemyRet(info);
				}
			}
			else
			{
				if (indexchild < 7) {
					EnemyInfo info;
					info.type = ENEMY_TYPES::WORMBODY;
					info.x = position.x;
					info.y = originY;
					info.going_up = false;
					info.tail = false;
					bodies[indexchild] = App->enemies->SpawnEnemyRet(info);
				}
				else {
					EnemyInfo info;
					info.type = ENEMY_TYPES::WORMBODY;
					info.x = position.x;
					info.y = originY;
					info.going_up = false;
					info.tail = true;
					bodies[indexchild] = App->enemies->SpawnEnemyRet(info);
				}
			}
			indexchild++;
			spawntime = 0;
		}else spawntime++;
		
	}

	if (up) {
		position.y++;
	}
	else {
		position.y--;
	}
}

void Enemy_Worm::OnCollision(Collider * collider)
{
	OnDeath();
}

void Enemy_Worm::Shine()
{
	hit = true;
	for each (Enemy* e in bodies)
	{
		if(e)e->Shine();
	}
}

void Enemy_Worm::OnDeath()
{
	for (int i = 0; i < sizeof(bodies); i++)
	{
		if (bodies[i] != nullptr)
		{
			switch (i)
			{
			case 1:
				bodies[i]->speed = { 0, -2 };
				break;
			case 2:
				bodies[i]->speed = { 1, -1 };
				break;
			case 3:
				bodies[i]->speed = { 2, 0 };
				break;
			case 4:
				bodies[i]->speed = { 1, 1 };
				break;
			case 5:
				bodies[i]->speed = { 0, 2 };
				break;
			case 6:
				bodies[i]->speed = { -1, 1 };
				break;
			case 7:
				bodies[i]->speed = { -2, 0 };
				break;
			default:
				break;
			}
		}
	}
}