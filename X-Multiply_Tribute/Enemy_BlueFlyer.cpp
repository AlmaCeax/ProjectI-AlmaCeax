#include "Application.h"
#include "ModulePlayer.h"
#include "ModuleCollision.h"
#include "ModuleParticles.h"
#include "Enemy_BlueFlyer.h"



Enemy_BlueFlyer::Enemy_BlueFlyer(int x, int y):Enemy(x,y)
{
	type = BLUEFLYER;
	points = 1000;

	idle.PushBack({ 284,106,29,30 });
	hitanim.PushBack({ 884, 182, 29, 30 });

	hitAnimation = &hitanim;
	animation = &idle;

	collider = App->collision->AddCollider({ 0, 0, 30, 18 }, COLLIDER_TYPE::COLLIDER_ENEMY_GROUND, (Module*)App->enemies);
}


Enemy_BlueFlyer::~Enemy_BlueFlyer()
{
}

void Enemy_BlueFlyer::TouchGround()
{
	if (falling) {
		original_position = { position.x, position.y };
		falling = false;
		canShoot = true;
		path.PushBack({ 0,-3 }, 25);
		if (App->player->position.x < position.x) {
			path.PushBack({ -3,0 }, 500);
			flipX = true;
		}
		else {
			path.PushBack({ 3,0 }, 500);
		}
	}
}

void Enemy_BlueFlyer::Move()
{
	if (falling) {
		position.y += 3;
	}
	else {
		position = original_position + path.GetCurrentPosition();
		int distance = position.x - (App->player->position.x+14);
		if (abs(distance) < 50) Shoot();
	}
}

void Enemy_BlueFlyer::Shoot()
{
	if (canShoot) {
		canShoot = false;
		if (flipX) {
			App->particles->AddParticle(App->particles->yellowCircle, position.x, position.y, COLLIDER_ENEMY_SHOT, { -1.5, -1 });
			App->particles->AddParticle(App->particles->yellowCircle, position.x, position.y, COLLIDER_ENEMY_SHOT, { -1, -1.5 });
			App->particles->AddParticle(App->particles->yellowCircle, position.x, position.y, COLLIDER_ENEMY_SHOT, { -1.5, 1 });
		}
		else {
			App->particles->AddParticle(App->particles->yellowCircle, position.x, position.y, COLLIDER_ENEMY_SHOT, { 1.5, -1 });
			App->particles->AddParticle(App->particles->yellowCircle, position.x, position.y, COLLIDER_ENEMY_SHOT, { 1, -1.5 });
			App->particles->AddParticle(App->particles->yellowCircle, position.x, position.y, COLLIDER_ENEMY_SHOT, { 1.5, 1 });
		}
	}
}
