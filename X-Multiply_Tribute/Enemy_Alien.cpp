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

	walk.PushBack({ 263, 269, 57, 58 });
	walk.PushBack({ 332, 269, 57, 58 });
	walk.PushBack({ 265, 335, 57, 58 });
	walk.PushBack({ 332, 269, 57, 58 });
	walk.speed = 0.2f;
	walk.loop = true;

	shoot.PushBack({ 332, 333, 53, 66 });
	shoot.PushBack({ 404, 267, 53, 64 });
	shoot.speed = 0.05f;



	hitanim.PushBack({ 751, 269, 57, 58 });
	hitanim.PushBack({ 820, 269, 57, 58 });
	hitanim.PushBack({ 753, 335, 57, 58 });
	hitanim.PushBack({ 820, 269, 57, 58 });
	hitanim.speed = 0.2f;

	hitanimshoot.PushBack({ 820, 333, 53, 66 });
	hitanimshoot.PushBack({ 892, 267, 53, 64 });
	hitanimshoot.speed = 0.05f;

	animation = &walk;
	
	hitAnimation = &hitanim;
	flipX = true;

	collider = App->collision->AddCollider({ 0, 0, 57, 58 }, COLLIDER_TYPE::COLLIDER_ENEMY, (Module*)App->enemies);
}

void Enemy_Alien::Move()
{

	if (shootTimer == 80) {
		animation = &shoot;
		hitAnimation = &hitanimshoot;
	}
	else if (shootTimer < 80 && !flipX) {
		animation = &walk;
		hitAnimation = &hitanim;
		position.x += 1;
	}
	else if (shootTimer < 80 && flipX) {
		animation = &walk;
		hitAnimation = &hitanim;
		position.x -= 1;
	}

	if (original_x - position.x < -110 ) {
		flipX = true;
	}
	if (original_x - position.x > 20 ) {
		flipX = false;
	}
	
	

	shootTimer++;
	
	if (shootTimer == 100 || shootTimer  == 120) {
		if (shootTimer == 120) {
			shootTimer = 0;
		}
		else {
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
		}
		
		
	}
}
