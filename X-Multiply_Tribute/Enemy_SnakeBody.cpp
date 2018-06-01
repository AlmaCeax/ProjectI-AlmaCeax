#include "Application.h"
#include "ModuleCollision.h"
#include "ModuleParticles.h"
#include "Enemy_SnakeBody.h"
#include "Enemy_WormBody.h"
#include "ModuleEnemies.h"
#include <stdlib.h> 


Enemy_SnakeBody::Enemy_SnakeBody(int x, int y, int id): Enemy(x, y)
{
	type = SNAKEBODY;

	w = 32;
	h = 32;

	circleCenter = { x, y };
	original_position = { x, y };

	if (id < 9) {
		center = { 16, 16 };

		anim.PushBack({ 179, 1835, 32, 32 });
		hitanim.PushBack({ 482, 1835, 32, 32 });
	}
	else {
		center = { 24, 15 };
		anim.PushBack({ 176, 1652, 48, 29 });
		hitanim.PushBack({ 479, 1652, 48, 29 });
	}

	animation = &anim;
	hitAnimation = &hitanim;

	collider = App->collision->AddCollider({ 0, 0, 32, 32 }, COLLIDER_TYPE::COLLIDER_ENEMY, (Module*)App->enemies);
	points = 100;
}


void Enemy_SnakeBody::Move()
{
	angle += 0.015f;
	switch (state)
	{
	case Enemy_SnakeBody::START:
		position.y = original_position.y + sinf(angle) * radius;
		position.x--;
		if (position.x == original_position.x - 100)
		{
			state = CIRCLE;
			circleCenter = { position.x + radius, position.y };
		}
		break;
	case Enemy_SnakeBody::CIRCLE:
		position.y = original_position.y + 2*sinf(angle) * radius;
		position.x = original_position.x + 2*cosf(angle) * radius;
		//rangle -= 1.2f;
		break;
	case Enemy_SnakeBody::END:
		break;
	case Enemy_SnakeBody::NONE:
		break;
	default:
		break;
	}
}

void Enemy_SnakeBody::Shine()
{

}

void Enemy_SnakeBody::OnDeath()
{
	if (death)
	{

	}
}