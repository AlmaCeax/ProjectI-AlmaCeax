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
	walk.PushBack({ 263, 269, 57, 58 });
	walk.PushBack({ 263, 269, 57, 58 });
	walk.speed = 0.2f;

	shoot.PushBack({ 263, 269, 57, 58 });
	shoot.PushBack({ 263, 269, 57, 58 });
	shoot.speed = 0.2f;

	animation = &walk;

	collider = App->collision->AddCollider({ 0, 0, 57, 58 }, COLLIDER_TYPE::COLLIDER_ENEMY, (Module*)App->enemies);
}

void Enemy_Alien::Move()
{

	if (original_x - position.x < -100 ) {
		right = false;
	}
	if (original_x - position.x > 20 ) {
		right = true;
	}
	if (right) {
		position.x += 1;
	}
	else {
		position.x -= 1;
	}

	shootTimer++;

	if (shootTimer == 80) {
		if (App->player->position.x > position.x - 50 && App->player->position.x < position.x + 50)
		{
			App->particles->AddParticle(App->particles->alienshot, position.x, position.y, COLLIDER_ENEMY_SHOT, { -1, -1 });
			App->particles->AddParticle(App->particles->alienshot, position.x, position.y, COLLIDER_ENEMY_SHOT, { -1, -1 });
			App->particles->AddParticle(App->particles->alienshot, position.x, position.y, COLLIDER_ENEMY_SHOT, { -1, -1 });
		}
		else if (App->player->position.x < position.x) {
			App->particles->AddParticle(App->particles->alienshot, position.x, position.y, COLLIDER_ENEMY_SHOT, { -1, -1 });
			App->particles->AddParticle(App->particles->alienshot, position.x, position.y, COLLIDER_ENEMY_SHOT, { -1, -1 });
			App->particles->AddParticle(App->particles->alienshot, position.x, position.y, COLLIDER_ENEMY_SHOT, { -1, -1 });
		}
		else if (App->player->position.x > position.x) {
			App->particles->AddParticle(App->particles->alienshot, position.x, position.y, COLLIDER_ENEMY_SHOT, { -1, -1 });
			App->particles->AddParticle(App->particles->alienshot, position.x, position.y, COLLIDER_ENEMY_SHOT, { -1, -1 });
			App->particles->AddParticle(App->particles->alienshot, position.x, position.y, COLLIDER_ENEMY_SHOT, { -1, -1 });
		}
		shootTimer = 0;
	}
}
