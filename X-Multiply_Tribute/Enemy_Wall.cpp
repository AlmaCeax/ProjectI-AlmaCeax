#include "Application.h"
#include "ModuleCollision.h"
#include "ModuleParticles.h"
#include "ModuleEnemies.h"
#include "ModulePlayer.h"
#include "ModuleRender.h"
#include "Enemy_Wall.h"
#include "ModuleEnemies.h"




Enemy_Wall::Enemy_Wall(int x, int y) :Enemy(x, y)
{
	type = WALL;
	original_y = y;
	original_x = x;

	points = 0;


	anim1.PushBack({ 875, 1229, 128, 145 });
	anim1.speed = 0.1f;
	anim1.loop = true;

	anim2.PushBack({ 875, 1418, 128, 145 });
	anim2.speed = 0.1f;
	anim2.loop = true;

	anim3.PushBack({ 337, 1074, 160, 49 });
	anim3.PushBack({ 153, 1151, 160, 49 });
	anim3.PushBack({ 339, 1364, 160, 54 });
	anim3.PushBack({ 333, 1288, 160, 62 });
	anim3.PushBack({ 344, 1207, 149, 70 });
	anim3.speed = 0.05f;
	anim3.loop = true;

	anim4.PushBack({ 337, 1074, 160, 49 });
	anim4.PushBack({ 153, 1151, 160, 49 });
	anim4.PushBack({ 339, 1364, 160, 54 });
	anim4.PushBack({ 333, 1288, 160, 62 });
	anim4.PushBack({ 344, 1207, 149, 70 });
	anim4.speed = 0.05f;
	anim4.loop = true;
	

	hitanim1.PushBack({ 1903, 1252, 128, 145 });

	hitanim3.PushBack({ 1903, 1252, 128, 145 });

	center3 = { 160, 0 };
	center4 = { 160, 49 };


	animation = &anim1;
	hitAnimation = &hitanim1;


	collider = App->collision->AddCollider({ 138, 13, 128, 145 }, COLLIDER_TYPE::COLLIDER_ENEMY, App->enemies);
	collider3 = App->collision->AddCollider({ 0, 0, 160, 30 }, COLLIDER_TYPE::COLLIDER_WALL, App->enemies);
	collider4 = App->collision->AddCollider({ 0, 0, 160, 30 }, COLLIDER_TYPE::COLLIDER_WALL, App->enemies);
}

void Enemy_Wall::Move()
{
	
	shootTimer++;
	if (shootTimer < 75 && shootTimer % 2 == 0) {
		rangle3--;
		rangle4++;
	}
	else if (shootTimer % 2 == 0) {
		rangle3++;
		rangle4--;
	}
	if (shootTimer == 150) {

		animation = &anim2;
		App->particles->AddParticle(App->particles->wall1, position.x + 40, position.y + 20, COLLIDER_ENEMY_SHOT, { -1, 0 });
		
		shootTimer = 0;
	}
	if (shootTimer == 5) {
		animation = &anim1;
	}

	
}
void Enemy_Wall::Draw(SDL_Texture* sprites) {
	if (collider != nullptr)
		collider->SetPos(position.x+ 138, position.y + 13);
	if (collider3 != nullptr)
		collider3->SetPos(position.x, position.y);
	if (collider4 != nullptr)
		collider4->SetPos(position.x, position.y + 120);

	if (animation != nullptr) {
		if (!hit)App->render->BlitFlipped(sprites, position.x + 138, position.y + 13, &(animation->GetCurrentFrame()), flipX, flipY, rangle, center);
		else {
			hitAnimation->setCurrentFrameIndex(animation->GetCurrentFrameIndex() - 1);
			App->render->BlitFlipped(sprites, position.x + 138, position.y + 13, &(hitAnimation->GetCurrentFrame()), flipX, flipY);
			hit = false;
		}
	}
	if (&anim3 != nullptr) {
		if (!hit)App->render->BlitFlipped(sprites, position.x, position.y, &((&anim3)->GetCurrentFrame()), flipX, flipY, rangle3, center3);
		else {
			(&hitanim3)->setCurrentFrameIndex((&anim3)->GetCurrentFrameIndex() - 1);
			App->render->BlitFlipped(sprites, position.x, position.y, &((&hitanim3)->GetCurrentFrame()), flipX, flipY);
			hit = false;
		}
	}
	if (&anim4 != nullptr) {
		if (!hit)App->render->BlitFlipped(sprites, position.x, position.y + 120, &((&anim4)->GetCurrentFrame()), flipX, true, rangle4, center4);
		else {
			(&hitanim4)->setCurrentFrameIndex((&anim4)->GetCurrentFrameIndex() - 1);
			App->render->BlitFlipped(sprites, position.x, position.y + 120, &((&hitanim4)->GetCurrentFrame()), true, flipY);
			hit = false;
		}
	}
}
