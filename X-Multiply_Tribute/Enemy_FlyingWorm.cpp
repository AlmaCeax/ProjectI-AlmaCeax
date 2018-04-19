#include "Application.h"
#include "ModuleCollision.h"
#include "ModuleParticles.h"
#include "ModulePlayer.h"
#include "Enemy_FlyingWorm.h"
#include "ModuleEnemies.h"




Enemy_FlyingWorm::Enemy_FlyingWorm(int x, int y):Enemy(x,y)
{
	type = FLYINGWORM;
	original_y = y;
	original_x = x;

	points = 400;

	fly.PushBack({ 40, 75, 34, 30 });
	fly.PushBack({ 84, 73, 37, 33 });
	fly.PushBack({ 35, 127, 42, 31 });
	fly.PushBack({ 90, 129, 43, 26 });
	fly.speed = 0.2f;

	animation = &fly;

	collider = App->collision->AddCollider({ 0, 0, 37, 33 }, COLLIDER_TYPE::COLLIDER_ENEMY, (Module*)App->enemies);
}

void Enemy_FlyingWorm::Move()
{
	wave += 0.1f;
	position.y = original_y + sinf(wave) * 20;
	position.x -= 1;

	shootTimer++;

	if (shootTimer == 80) {
		float vx = App->player->position.x + 5 - position.x;
		float vy = App->player->position.y - position.y;
		App->particles->AddParticle(App->particles->blueBall, position.x, position.y, COLLIDER_ENEMY_SHOT, {vx/40,vy/40});
		shootTimer = 0;
	}
}
