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

	left.PushBack({ 24, 183, 64, 60 });
	left.PushBack({ 99, 183, 64, 60 });
	left.PushBack({ 177, 183, 64, 61 });
	left.PushBack({ 99, 183, 64, 60 });
	left.PushBack({ 24, 183, 64, 60 });

	left.PushBack({ 99, 183, 64, 60 });
	left.PushBack({ 177, 183, 64, 61 });
	left.PushBack({ 99, 183, 64, 60 });
	left.speed = 0.1f;
	left.loop = true;

	leftright.PushBack({ 24, 183, 64, 60 });
	leftright.PushBack({ 99, 183, 64, 60 });
	leftright.PushBack({ 177, 183, 64, 61 });
	leftright.PushBack({ 99, 183, 64, 60 });
	leftright.PushBack({ 24, 183, 64, 60 });

	leftright.PushBack({ 99, 253, 64, 60 });
	leftright.PushBack({ 24, 252, 64, 61 });
	leftright.PushBack({ 99, 253, 64, 60 });
	leftright.speed = 0.1f;
	left.loop = true;

	leftup.PushBack({ 24, 183, 64, 60 });
	leftup.PushBack({ 99, 183, 64, 60 });
	leftup.PushBack({ 177, 183, 64, 61 });
	leftup.PushBack({ 99, 183, 64, 60 });
	leftup.PushBack({ 24, 183, 64, 60 });

	leftup.PushBack({ 177, 253, 64, 60 });
	leftup.PushBack({ 24, 333, 64, 61 });
	leftup.PushBack({ 177, 253, 64, 60 });
	leftup.speed = 0.1f;
	left.loop = true;

	hitanim.PushBack({ 511, 183, 64, 60 });
	hitanim.PushBack({ 586, 183, 64, 60 });
	hitanim.PushBack({ 664, 183, 64, 61 });
	hitanim.PushBack({ 586, 183, 64, 60 });
	hitanim.PushBack({ 511, 183, 64, 60 });
	hitanim.PushBack({ 586, 183, 64, 60 });
	hitanim.PushBack({ 664, 183, 64, 61 });
	hitanim.PushBack({ 586, 183, 64, 60 });


	animation = &left;

	hitAnimation = &hitanim;

	collider = App->collision->AddCollider({ 0, 0, 57, 58 }, COLLIDER_TYPE::COLLIDER_ENEMY, (Module*)App->enemies);
}

void Enemy_BrownEye::Move()
{
	if (position.x == 4413 && position.y == 275) {
		animation = &leftright;
	}
	else if (position.x == 4413 && position.y == 340) {
		animation = &left;
	}
	else if (position.x == 4477 && position.y == 340) {
		animation = &left;
	}
	else if (position.x == 4413 && position.y == 410) {
		animation = &leftup;
	}

	shootTimer++;

	if (shootTimer == 60 || shootTimer == 70 || shootTimer == 150) {
		if (position.x == 4413 && position.y == 275) {
			if (App->player->position.x > position.x - 50 && App->player->position.x < position.x + 50)
			{
				App->particles->AddParticle(App->particles->browneyeshot2, position.x + 20, position.y + 20 , COLLIDER_ENEMY_SHOT, { 0, 1 });
			}
			else if (App->player->position.x < position.x) {
				App->particles->AddParticle(App->particles->browneyeshot1, position.x + 20, position.y + 20, COLLIDER_ENEMY_SHOT, { -1, 1 });
			}
			else if (App->player->position.x > position.x) {
				App->particles->AddParticle(App->particles->browneyeshot3, position.x + 20, position.y + 20, COLLIDER_ENEMY_SHOT, { 1, 1 });
			}
		}
		else if (position.x == 4413 && position.y == 340) {
			if (App->player->position.x < position.x) {
				App->particles->AddParticle(App->particles->browneyeshot4, position.x + 20, position.y + 20, COLLIDER_ENEMY_SHOT, { -1, 0 });
			}
			else if (App->player->position.x > position.x) {
				App->particles->AddParticle(App->particles->browneyeshot5, position.x + 20, position.y + 20, COLLIDER_ENEMY_SHOT, { 1, 0 });
			}
		}
		else if (position.x == 4477 && position.y == 340) {
			if (App->player->position.x < position.x) {
				App->particles->AddParticle(App->particles->browneyeshot4, position.x + 20, position.y + 20, COLLIDER_ENEMY_SHOT, { -1, 0 });
			}
			else if (App->player->position.x > position.x) {
				App->particles->AddParticle(App->particles->browneyeshot5, position.x + 20, position.y + 20, COLLIDER_ENEMY_SHOT, { 1, 0 });
			}
		}
		else if (position.x == 4413 && position.y == 410) {
			if (App->player->position.x > position.x - 50 && App->player->position.x < position.x + 50)
			{
				App->particles->AddParticle(App->particles->browneyeshot7, position.x + 20, position.y + 20, COLLIDER_ENEMY_SHOT, { 0, -1 });
			}
			else if (App->player->position.x < position.x) {
				App->particles->AddParticle(App->particles->browneyeshot6, position.x + 20, position.y + 20, COLLIDER_ENEMY_SHOT, { -1, -1 });
			}
			else if (App->player->position.x > position.x) {
				App->particles->AddParticle(App->particles->browneyeshot8, position.x + 20, position.y + 20, COLLIDER_ENEMY_SHOT, { 1, -1 });
			}
		}
	}
	if (shootTimer == 150) {
		shootTimer = 0;
	}
}
