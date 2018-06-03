#include "Application.h"
#include "ModuleCollision.h"
#include "ModuleParticles.h"
#include "Enemy_WormBody.h"
#include "ModuleEnemies.h"
#include <stdlib.h> 



Enemy_WormBody::Enemy_WormBody(int x, int y, bool _up, bool tail) :Enemy(x, y)
{
	type = WORMBODY;

	up = _up;
	speed = {0,1};
	if (up) {
		flipY = true;
	}
	center = { 8, 8 };

	if (!tail)
	{
		anim.PushBack({ 59, 636, 16, 16 });
		anim.PushBack({ 76, 635, 16, 16 });
		anim.PushBack({ 94, 635, 14, 16 });

		hitanim.PushBack({ 548, 636, 16, 16 });
		hitanim.PushBack({ 565, 635, 16, 16 });
		hitanim.PushBack({ 583, 635, 16, 16 });
	}
	else {
		anim.PushBack({ 110, 636, 16, 16 });
		anim.PushBack({ 63, 654, 16, 16 });
		anim.PushBack({ 45, 654, 16, 16 });

		hitanim.PushBack({ 600, 636, 16, 16 });
		hitanim.PushBack({ 654, 635, 16, 16 });
		hitanim.PushBack({ 534, 654, 16, 16 });
	}

	anim.speed = 0.f;

	animation = &anim;
	hitAnimation = &hitanim;
	state = VERTICAL;
	collider = App->collision->AddCollider({ 0, 0, 16, 16 }, COLLIDER_TYPE::COLLIDER_ENEMY, (Module*)App->enemies);

	points = 100;
}


void Enemy_WormBody::Move()
{
	if (up) {
		switch (state)
		{
		case Enemy_WormBody::VERTICAL:
			position.y++;
			anim.setCurrentFrameIndex(0);
			break;
		case Enemy_WormBody::HORIZONTAL:
			position.x++;
			anim.setCurrentFrameIndex(1);
			break;
		case Enemy_WormBody::CURVER:
			angle += 0.03f;
			if (!startedcircle) {
				iPoint c = { position.x - radius , position.y };
				circleCenter = c;
				startedcircle = true;
			}
			position.y = circleCenter.y + sinf(angle) * radius;
			position.x = circleCenter.x + cosf(angle) * radius;
			rangle += 1.7f;
			break;
		case::Enemy_WormBody::CURVEL:
			angle += 0.03f;
			if (!startedcircle) {
				iPoint c = { position.x + radius , position.y };
				circleCenter = c;
				startedcircle = true;
			}
			position.y = circleCenter.y + sinf(angle) * radius;
			position.x = circleCenter.x - cosf(angle) * radius;
			rangle -= 1.7f;
			break;
		case Enemy_WormBody::CIRCLE:
			position.y++;
			if (left == 1)state = CURVEL;
			else state = CURVER;
			break;
		default:
			break;
		}

		if (state != HEADDEAD) {
			if (position.y == 420)state = CIRCLE;
		}
		else {
			position += speed;
		}
	}
	else {
		switch (state)
		{
		case Enemy_WormBody::VERTICAL:
			position.y--;
			anim.setCurrentFrameIndex(0);
			break;
		case Enemy_WormBody::HORIZONTAL:
			position.x++;
			anim.setCurrentFrameIndex(1);
			break;
		case Enemy_WormBody::CURVER:
			angle += 0.03f;
			if (!startedcircle) {
				iPoint c = { position.x + radius , position.y };
				circleCenter = c;
				startedcircle = true;
			}
			position.y = circleCenter.y - sinf(angle) * radius;
			position.x = circleCenter.x - cosf(angle) * radius;
			rangle += 1.7f;
			break;
		case::Enemy_WormBody::CURVEL:
			angle += 0.03f;
			if (!startedcircle) {
				iPoint c = { position.x - radius , position.y };
				circleCenter = c;
				startedcircle = true;
			}
			position.y = circleCenter.y - sinf(angle) * radius;
			position.x = circleCenter.x + cosf(angle) * radius;
			rangle -= 1.7f;
			break;
		case Enemy_WormBody::CIRCLE:
			position.y++;
			if (left == 1)state = CURVEL;
			else state = CURVER;
			break;
		default:
			break;
		}
		if (state != HEADDEAD) {
			if (position.y == 460)state = CIRCLE;
		}
		else {
			position += speed;
			rangle -= 3.f;
		}
	}
	
}

