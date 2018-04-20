#include "Application.h"
#include "ModuleCollision.h"
#include "ModuleParticles.h"
#include "Enemy_TentacleShooter.h"
#include "ModuleEnemies.h"


Enemy_TentacleShooter::Enemy_TentacleShooter(int x, int y, bool _up):Enemy(x,y)
{
	type = TENTACLESHOOTER;
	if (_up) {
		up = _up;
		anim.PushBack({ 37, 419, 54, 42 });
		anim.PushBack({ 95, 419, 54, 42 });
		anim.PushBack({ 157, 419, 54, 42 });
	}
	else {
		anim.PushBack({ 37, 10, 54, 42 });
		anim.PushBack({ 94, 10, 54, 42 });
		anim.PushBack({ 155, 10, 54, 42 });
	}
	
	anim.speed = 0.05f;

	animation = &anim;

	collider = App->collision->AddCollider({ 0, 0, 48, 41 }, COLLIDER_TYPE::COLLIDER_ENEMY, (Module*)App->enemies);

	points = 600;
}


void Enemy_TentacleShooter::Move()
{
	shootTimer++;
	Particle part;
	if (shootTimer == 80) {
		if (up) {
			App->particles->AddParticle(App->particles->blueBall, position.x, position.y + 20, COLLIDER_ENEMY_SHOT, { -2, 1 });
			App->particles->AddParticle(App->particles->blueBall, position.x + 12, position.y + 10, COLLIDER_ENEMY_SHOT, { -1, 2 });
			App->particles->AddParticle(App->particles->blueBall, position.x + 24, position.y, COLLIDER_ENEMY_SHOT, { 0, 3 });
			App->particles->AddParticle(App->particles->blueBall, position.x + 36, position.y + 10, COLLIDER_ENEMY_SHOT, { 1, 2 });
			App->particles->AddParticle(App->particles->blueBall, position.x + 48, position.y + 20, COLLIDER_ENEMY_SHOT, { 2, 1 });
			shootTimer = 0;
		}
		else {
			App->particles->AddParticle(App->particles->blueBall, position.x, position.y + 20, COLLIDER_ENEMY_SHOT, { -2, -1 });
			App->particles->AddParticle(App->particles->blueBall, position.x + 12, position.y + 10, COLLIDER_ENEMY_SHOT, { -1, -2 });
			App->particles->AddParticle(App->particles->blueBall, position.x + 24, position.y, COLLIDER_ENEMY_SHOT, { 0, -2 });
			App->particles->AddParticle(App->particles->blueBall, position.x + 36, position.y + 10, COLLIDER_ENEMY_SHOT, { 1, -2 });
			App->particles->AddParticle(App->particles->blueBall, position.x + 48, position.y + 20, COLLIDER_ENEMY_SHOT, { 2, -1 });
			shootTimer = 0;
		}
		
	}
}
