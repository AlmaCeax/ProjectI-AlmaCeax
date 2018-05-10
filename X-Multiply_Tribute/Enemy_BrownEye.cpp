#include "Application.h"
#include "ModuleCollision.h"
#include "ModuleParticles.h"
#include "ModulePlayer.h"
#include "Enemy_BrownEye.h"
#include "ModuleEnemies.h"




Enemy_BrownEye::Enemy_BrownEye(int x, int y) :Enemy(x, y)
{
	type = BROWNEYE;
	original_y = y;
	original_x = x;

	points = 5000;

	shoot.PushBack({ 24, 183, 64, 60 });
	shoot.PushBack({ 24, 183, 64, 60 });
	shoot.PushBack({ 24, 183, 64, 60 });
	shoot.PushBack({ 24, 183, 64, 60 });
	shoot.PushBack({ 24, 183, 64, 60 });
	shoot.PushBack({ 24, 183, 64, 60 });
	shoot.PushBack({ 24, 183, 64, 60 });
	shoot.speed = 0.1f;

	hitanim.PushBack({ 751, 269, 56, 58 });


	animation = &shoot;

	hitAnimation = &hitanim;

	collider = App->collision->AddCollider({ 0, 0, 57, 58 }, COLLIDER_TYPE::COLLIDER_ENEMY, (Module*)App->enemies);
}

void Enemy_BrownEye::Move()
{

	
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
