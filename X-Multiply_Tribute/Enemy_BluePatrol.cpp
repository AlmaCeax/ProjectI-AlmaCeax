#include "Application.h"
#include "ModuleCollision.h"
#include "ModuleParticles.h"
#include "ModulePlayer.h"
#include "Enemy_BluePatrol.h"
#include "ModuleEnemies.h"




Enemy_BluePatrol::Enemy_BluePatrol(int x, int y) :Enemy(x, y)
{
	type = BLUEPATROL;
	original_y = y;
	original_x = x;

	w = 27;
	h = 29;
	points = 900;

	walk.PushBack({ 23, 692, 27, 29 });
	walk.PushBack({ 80, 691, 27, 29 });
	walk.PushBack({ 27, 749, 28, 29 });
	walk.PushBack({ 84, 750, 28, 27 });
	walk.speed = 0.2f;
	walk.loop = true;


	hitanim.PushBack({ 512, 692, 27, 29 });
	hitanim.PushBack({ 569, 691, 27, 29 });
	hitanim.PushBack({ 516, 749, 28, 29 });
	hitanim.PushBack({ 573, 750, 28, 27 });
	hitanim.speed = 0.2f;
	hitanim.loop = true;


	animation = &walk;

	hitAnimation = &hitanim;

	collider = App->collision->AddCollider({ 0, 0, 27, 29 }, COLLIDER_TYPE::COLLIDER_ENEMY, (Module*)App->enemies);
}

void Enemy_BluePatrol::Move()
{

	if (shootTimer < 100 && !flipX) {
		if (shootTimer % 3 == 0) {
			position.x -= 1;
		}
		
	}else if (shootTimer < 100 && flipX) {
		if (cont > 0) {
			position.x += 2;
		}
		
	}

	if (shootTimer == 160 || shootTimer == 80 || shootTimer == 0) {

		if (shootTimer == 80 || shootTimer == 0) {
			if (App->player->position.x < position.x) {
				flipX = false;
			}
			else if (App->player->position.x > position.x) {
				flipX = true;
				cont--;
			}
		}
		else {
			if (App->player->position.x > position.x - 50 && App->player->position.x < position.x + 50)
			{
				App->particles->AddParticle(App->particles->blueBall, position.x, position.y, COLLIDER_ENEMY_SHOT, { 0, -1 });
			}
			else if (App->player->position.x < position.x) {
				App->particles->AddParticle(App->particles->blueBall, position.x, position.y, COLLIDER_ENEMY_SHOT, { -1, -1 });
			}
			else if (App->player->position.x > position.x) {
				App->particles->AddParticle(App->particles->blueBall, position.x, position.y, COLLIDER_ENEMY_SHOT, { 1, -1 });
			}
			shootTimer = 0;
		}
		
	}
	shootTimer++;
}
