#include "Application.h"
#include "ModuleCollision.h"
#include "ModuleParticles.h"
#include "Enemy_BlueMouth.h"
#include "ModuleEnemies.h"


Enemy_BlueMouth::Enemy_BlueMouth(int x, int y, bool _up) :Enemy(x, y)
{
	type = BLUEMOUTH;
	if (_up) {
		up = _up;
		anim.PushBack({ 266, 556, 48, 35 });
		anim.PushBack({ 266, 604, 48, 34 });
		anim.PushBack({ 327, 555, 48, 36 });

		hitanim.PushBack({ 763, 440, 48, 35 });
		hitanim.PushBack({ 763, 488, 48, 34 });
		hitanim.PushBack({ 824, 436, 48, 36 });
	}
	else {
		anim.PushBack({ 266, 556, 48, 35 });
		anim.PushBack({ 266, 604, 48, 34 });
		anim.PushBack({ 327, 555, 48, 36 });

		hitanim.PushBack({ 763, 440, 48, 35 });
		hitanim.PushBack({ 763, 488, 48, 34 });
		hitanim.PushBack({ 824, 436, 48, 36 });
	}
	w = 48;
	h = 35;
	anim.speed = 0.03f;

	animation = &anim;
	hitAnimation = &hitanim;

	collider = App->collision->AddCollider({ 0, 0, 48, 34 }, COLLIDER_TYPE::COLLIDER_ENEMY, (Module*)App->enemies);

	points = 500;
}


void Enemy_BlueMouth::Move()
{
	shootTimer++;
	Particle part;
	if (shootTimer == 70) {
		if (up) {
			App->particles->AddParticle(App->particles->greenBall, position.x + 8, position.y + 2, COLLIDER_ENEMY_DESTRUCTIBLE_SHOT, { -3, 0 });
			App->particles->AddParticle(App->particles->greenBall, position.x + 16, position.y + 1, COLLIDER_ENEMY_DESTRUCTIBLE_SHOT, { -2, -1 });
			App->particles->AddParticle(App->particles->greenBall, position.x + 20, position.y, COLLIDER_ENEMY_DESTRUCTIBLE_SHOT, { -1, -2 });
			App->particles->AddParticle(App->particles->greenBall, position.x + 24, position.y, COLLIDER_ENEMY_DESTRUCTIBLE_SHOT, { 1, -2 });
			App->particles->AddParticle(App->particles->greenBall, position.x + 28, position.y + 1, COLLIDER_ENEMY_DESTRUCTIBLE_SHOT, { 2, -1 });
			App->particles->AddParticle(App->particles->greenBall, position.x + 32, position.y + 2, COLLIDER_ENEMY_DESTRUCTIBLE_SHOT, { 3, 0 });
			shootTimer = 0;
		}
		else {
			App->particles->AddParticle(App->particles->greenBall, position.x + 8, position.y + 2, COLLIDER_ENEMY_DESTRUCTIBLE_SHOT, { -3, 0 });
			App->particles->AddParticle(App->particles->greenBall, position.x + 16, position.y + 1, COLLIDER_ENEMY_DESTRUCTIBLE_SHOT, { -2, 1 });
			App->particles->AddParticle(App->particles->greenBall, position.x + 20, position.y, COLLIDER_ENEMY_DESTRUCTIBLE_SHOT, { -1, 2 });
			App->particles->AddParticle(App->particles->greenBall, position.x + 24, position.y, COLLIDER_ENEMY_DESTRUCTIBLE_SHOT, { 1, 2 });
			App->particles->AddParticle(App->particles->greenBall, position.x + 28, position.y + 1, COLLIDER_ENEMY_DESTRUCTIBLE_SHOT, { 2, 1 });
			App->particles->AddParticle(App->particles->greenBall, position.x + 32, position.y + 2, COLLIDER_ENEMY_DESTRUCTIBLE_SHOT, { 3, 0 });
			shootTimer = 0;
		}

	}
}