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
		if (spawntime == 10)
		{
			if (up) {
				if (indexchild < 7) {
					App->enemies->AddEnemy(WORMBODY, position.x, originY, true, 0, -1, true, false);
					LOG("HOLA");
				}
				else {
					App->enemies->AddEnemy(WORMBODY, position.x, originY, true, 0, -1, true, true);
					LOG("HOLA");

				}
			}
			else
			{
				if (indexchild < 7) {
					App->enemies->AddEnemy(WORMBODY, position.x, originY, false, 0, -1, true, false);
					LOG("HOLA");

				}
				else {
					App->enemies->AddEnemy(WORMBODY, position.x, originY, false, 0, -1, true, true);
					LOG("HOLA");

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