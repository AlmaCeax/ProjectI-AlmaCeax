#include "Application.h"
#include "ModulePlayer.h"
#include "ModuleCollision.h"
#include "ModuleParticles.h"
#include "SDL/include/SDL_timer.h"
#include "Enemy_Jumper.h"



Enemy_Jumper::Enemy_Jumper(int x, int y, bool back, bool normal_spawn): Enemy(x,y)
{
	type = JUMPER;

	points = 400;

	air.PushBack({276,179,23,24});

	down.PushBack({ 309, 181, 40, 24 });

	idle.PushBack({ 270, 213, 40, 24 });
	idle.PushBack({ 320, 215, 40, 24 });
	idle.speed = 0.05f;

	animation = &air;
	canBackJump = back;

	if (!normal_spawn) {
		speed = { 1,1 };
	}

	collider = App->collision->AddCollider({ 0, 0, 30, 18 }, COLLIDER_TYPE::COLLIDER_ENEMY_GROUND, (Module*)App->enemies);
	ground_collider = App->collision->AddCollider({ x, y + 30, 25, 24 }, COLLIDER_TYPE::COLLIDER_GROUND, (Module*)App->enemies);
}


Enemy_Jumper::~Enemy_Jumper()
{

}

void Enemy_Jumper::SetDownAnim()
{
	if(going_down) animation = &down;
}

void Enemy_Jumper::TouchGround()
{
	if (going_down) {
		speed = { 0,0 };
		jumping = false;
		going_down = false;
		animation = &idle;
		start_time = SDL_GetTicks();
		if (jumps == 0) {
			total_time = (Uint32)(2.0f * 1000.0f);
			jumps = 1;
		}
		else {
			total_time = (Uint32)(0.0f * 1000.0f);
			jumps--;
		}
	}
}

void Enemy_Jumper::Move()
{
	if (jumping) Jump();
	else Wait();

	ground_collider->SetPos(position.x, position.y + 30);
}

void Enemy_Jumper::Jump()
{
	position.x += speed.x;
	position.y += speed.y;
	if (speed.y < 3)
	{
		speed.y += 0.1f;
		if (speed.y >= 0) {
			Shoot();
			going_down = true;
		}
	}
}

void Enemy_Jumper::Wait()
{
	Uint32 now = SDL_GetTicks() - start_time;
	if (now >= total_time) {
		if (App->player->position.x > position.x) {
			speed = {2,-3};
		}
		else {
			if (canBackJump) speed = { -1,-3 };
			else speed = {0,-3};
		}
		going_down = false;
		jumping = true;
		canShoot = true;
		animation = &air;
	}
}

void Enemy_Jumper::Shoot() {
	if (canShoot)
	{
		App->particles->AddParticle(App->particles->blueBall, position.x + 11, position.y + 24, COLLIDER_ENEMY_SHOT, { -2, 0 });
		App->particles->AddParticle(App->particles->blueBall, position.x + 11, position.y + 24, COLLIDER_ENEMY_SHOT, { -1.5, -1 });
		App->particles->AddParticle(App->particles->blueBall, position.x + 11, position.y + 24, COLLIDER_ENEMY_SHOT, { -1, -1.5 });
		App->particles->AddParticle(App->particles->blueBall, position.x + 11, position.y + 24, COLLIDER_ENEMY_SHOT, { 0, -2 });
		App->particles->AddParticle(App->particles->blueBall, position.x + 11, position.y + 24, COLLIDER_ENEMY_SHOT, { 2, -1 });
		App->particles->AddParticle(App->particles->blueBall, position.x + 11, position.y + 24, COLLIDER_ENEMY_SHOT, { 1.5, -1.5 });
		App->particles->AddParticle(App->particles->blueBall, position.x + 11, position.y + 24, COLLIDER_ENEMY_SHOT, { 2, 0 });
		canShoot = false;
	}

}

void Enemy_Jumper::OnCollision(Collider* collider) {
	Enemy::OnCollision(collider);
	ground_collider->to_delete = true;
}
