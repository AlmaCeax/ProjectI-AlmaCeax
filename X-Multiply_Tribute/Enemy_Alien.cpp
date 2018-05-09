#include "Application.h"
#include "ModuleCollision.h"
#include "ModuleParticles.h"
#include "ModulePlayer.h"
#include "Enemy_Alien.h"
#include "ModuleEnemies.h"




Enemy_Alien::Enemy_Alien(int x, int y) :Enemy(x, y)
{
	type = ALIEN;
	original_y = y;
	original_x = x;

	points = 5000;

	walk.PushBack({ 264, 411, 57, 58 });
	walk.PushBack({ 331, 411, 57, 58 });
	walk.PushBack({ 264, 477, 57, 58 });
	walk.PushBack({ 331, 411, 57, 58 });
	walk.speed = 0.2f;
	walk.loop = true;

	walkright.PushBack({ 263, 269, 57, 58 });
	walkright.PushBack({ 332, 269, 57, 58 });
	walkright.PushBack({ 265, 335, 57, 58 });
	walkright.PushBack({ 332, 269, 57, 58 });
	walkright.speed = 0.2f;
	walkright.loop = true;

	shoot.PushBack({ 340, 480, 53, 55 });
	shoot.PushBack({ 340, 480, 53, 55 });
	shoot.PushBack({ 407, 414, 53, 54 });
	shoot.PushBack({ 407, 414, 53, 54 });
	shoot.PushBack({ 407, 414, 53, 54 });
	shoot.PushBack({ 407, 414, 53, 54 });
	shoot.PushBack({ 340, 480, 53, 55 });
	shoot.PushBack({ 340, 480, 53, 55 });
	shoot.speed = 0.1f;

	shootright.PushBack({ 332, 338, 53, 56 });
	shootright.PushBack({ 332, 338, 53, 56 });
	shootright.PushBack({ 404, 272, 53, 54 });
	shootright.PushBack({ 404, 272, 53, 54 });
	shootright.PushBack({ 404, 272, 53, 54 });
	shootright.PushBack({ 404, 272, 53, 54 });
	shootright.PushBack({ 332, 338, 53, 56 });
	shootright.PushBack({ 332, 338, 53, 56 });
	shootright.speed = 0.1f;

	hitanim.PushBack({ 751, 269, 56, 58 });


	animation = &walk;
	
	hitAnimation = &hitanim;

	collider = App->collision->AddCollider({ 0, 0, 57, 58 }, COLLIDER_TYPE::COLLIDER_ENEMY, (Module*)App->enemies);
}

void Enemy_Alien::Move()
{

	if (shootTimer == 80 && right) {
		animation = &shootright;
	}
	else if (shootTimer == 80 && !right) {
		animation = &shoot;
	}else if (shootTimer < 80 && right) {
		animation = &walkright;
		position.x += 1;
	}
	else if (shootTimer < 80 && !right) {
		animation = &walk;
		position.x -= 1;
	}

	if (original_x - position.x < -110 ) {
		right = false;
	}
	if (original_x - position.x > 20 ) {
		right = true;
	}
	
	

	shootTimer++;
	
	if (shootTimer == 120) {
		
		if (App->player->position.x > position.x - 50 && App->player->position.x < position.x + 50)
		{
			App->particles->AddParticle(App->particles->alienshot, position.x, position.y, COLLIDER_ENEMY_SHOT, { -1, -1 });
			App->particles->AddParticle(App->particles->alienshot, position.x, position.y, COLLIDER_ENEMY_SHOT, { 0, -1 });
			App->particles->AddParticle(App->particles->alienshot, position.x, position.y, COLLIDER_ENEMY_SHOT, { 1, -1 });
		}
		else if (App->player->position.x < position.x) {
			App->particles->AddParticle(App->particles->alienshot, position.x, position.y, COLLIDER_ENEMY_SHOT, { 0, -2 });
			App->particles->AddParticle(App->particles->alienshot, position.x, position.y, COLLIDER_ENEMY_SHOT, { -1, -1 });
			App->particles->AddParticle(App->particles->alienshot, position.x, position.y, COLLIDER_ENEMY_SHOT, { -2, 0 });
		}
		else if (App->player->position.x > position.x) {
			App->particles->AddParticle(App->particles->alienshot, position.x, position.y, COLLIDER_ENEMY_SHOT, { 0, -2 });
			App->particles->AddParticle(App->particles->alienshot, position.x, position.y, COLLIDER_ENEMY_SHOT, { 1, -1 });
			App->particles->AddParticle(App->particles->alienshot, position.x, position.y, COLLIDER_ENEMY_SHOT, { 2, 0 });
		}
		shootTimer = 0;
	}
}
