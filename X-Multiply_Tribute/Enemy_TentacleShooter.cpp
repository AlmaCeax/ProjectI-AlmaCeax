#include "Application.h"
#include "ModuleCollision.h"
#include "ModuleParticles.h"
#include "Enemy_TentacleShooter.h"


Enemy_TentacleShooter::Enemy_TentacleShooter(int x, int y):Enemy(x,y)
{
	anim.PushBack({ 35, 2, 48, 41 });
	anim.PushBack({ 96, 3, 46, 39 });
	anim.PushBack({ 156, 0, 47, 42 });
	anim.speed = 0.05f;

	animation = &anim;

	collider = App->collision->AddCollider({ 0, 0, 48, 41 }, COLLIDER_TYPE::COLLIDER_ENEMY, (Module*)App->enemies);
}


void Enemy_TentacleShooter::Move()
{
	shootTimer++;
	Particle part;
	if (shootTimer == 80) {
		App->particles->AddParticle(App->particles->blueBall, position.x, position.y + 20, COLLIDER_ENEMY_SHOT, { -2, -1 });
		App->particles->AddParticle(App->particles->blueBall, position.x + 12, position.y + 10, COLLIDER_ENEMY_SHOT, { -1, -2 });	
		App->particles->AddParticle(App->particles->blueBall, position.x + 24, position.y, COLLIDER_ENEMY_SHOT, { 0, -2 });
		App->particles->AddParticle(App->particles->blueBall, position.x + 36, position.y + 10, COLLIDER_ENEMY_SHOT, { 1, -2 });
		App->particles->AddParticle(App->particles->blueBall, position.x + 48, position.y + 20, COLLIDER_ENEMY_SHOT, { 2, -1 });
		shootTimer = 0;
	}
}
