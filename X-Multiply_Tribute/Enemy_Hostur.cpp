#include "Application.h"
#include "ModuleCollision.h"
#include "ModuleParticles.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleEnemies.h"
#include "ModulePlayer.h"
#include "Enemy_Hostur.h"


Enemy_Hostur::Enemy_Hostur(int x, int y):Enemy(x,y)
{
	spawning = true;
	fire_rate = 90;
	type = HOSTUR;
	w = 167;
	h = 184;

	idle.PushBack({ 23, 791, 167, 184 });

	shoot.PushBack({ 209,791,167,184 });
	shoot.PushBack({ 209,791,167,184 });
	shoot.PushBack({ 209,791,167,184 });
	shoot.PushBack({ 209,791,167,184 });
	shoot.PushBack({ 209,791,167,184 });
	shoot.PushBack({ 209,791,167,184 });
	shoot.PushBack({ 209,791,167,184 });
	shoot.PushBack({ 209,791,167,184 });
	shoot.PushBack({ 209,791,167,184 });
	shoot.PushBack({ 209,791,167,184 });
	shoot.PushBack({ 209,791,167,184 });
	shoot.PushBack({ 209,791,167,184 });
	shoot.PushBack({ 209,791,167,184 });

	hitanim.PushBack({ 477, 802, 167, 184 });

	points = 100000;

	hitAnimation = &hitanim;

	background_position = { 4548, 256 };

	bckg_texture = App->textures->Load("Assets/Sprites/Stages/Stage1/Background/BossBackground.png");

	animation = &idle;
}


Enemy_Hostur::~Enemy_Hostur()
{
	App->textures->Unload(bckg_texture);
	for (int i = 0; i < 5; i++) {
		colliders[i]->to_delete = true;
	}
}

void Enemy_Hostur::Move() {
	if (spawning) {
		position.y-=2;
		if (position.y == 278) {
			spawning = false;

			rect_1 = { position.x + 49,position.y + 96,118,88 };
			rect_2 = { position.x + 3,position.y + 44,164,41 };
			rect_3 = { position.x + 24,position.y + 29,143,15 };
			rect_4 = { position.x + 55,position.y + 13,32,14 };
			rect_5 = { position.x + 78,position.y,7,13 };


			colliders[0] = App->collision->AddCollider(rect_1, COLLIDER_ENEMY, App->enemies);
			colliders[1] = App->collision->AddCollider(rect_2, COLLIDER_ENEMY, App->enemies);
			colliders[2] = App->collision->AddCollider(rect_3, COLLIDER_ENEMY, App->enemies);
			colliders[3] = App->collision->AddCollider(rect_4, COLLIDER_ENEMY, App->enemies);
			colliders[4] = App->collision->AddCollider(rect_5, COLLIDER_ENEMY, App->enemies);
		}
	}
	else Shoot();
}

void Enemy_Hostur::Shoot()
{
	if (shot_cd == 0) {
		animation = &shoot;
		fPoint temp, shot_vector;
		float magnitude;

		switch (current_shot) {
		case shot_types::mid_shot:
			temp = { App->player->position.x - (position.x + 22), App->player->position.y - (position.y + 78) };
			magnitude = sqrt((pow(temp.x, 2)) + (pow(temp.y, 2)));
			shot_vector = { (temp.x / magnitude ) * 2, (temp.y / magnitude) * 2 };
			if (shot_vector.y < 0.1f && shot_vector.y > -0.1f) shot_vector.y = 0;

			App->particles->AddParticle(App->particles->hosturballmid, position.x + 22, position.y + 78, COLLIDER_ENEMY_SHOT_WALL, shot_vector);
			if (last_shot == bot_shot) current_shot = top_shot;
			else current_shot = bot_shot;

			last_shot = mid_shot;
			break;
		case shot_types::bot_shot:
			temp = { App->player->position.x - (position.x + 22), (App->player->position.y + 64) - (position.y + 78) };
			magnitude = sqrt((pow(temp.x, 2)) + (pow(temp.y, 2)));
			shot_vector = { (temp.x / magnitude) * 2, (temp.y / magnitude) * 3 };
			if (shot_vector.y > 0.5 && shot_vector.y < 1) shot_vector.y = 1;

			App->particles->AddParticle(App->particles->hosturball, position.x + 22, position.y + 78, COLLIDER_ENEMY_SHOT_WALL, shot_vector);
			last_shot = bot_shot;
			current_shot = mid_shot;
			break;
		case shot_types::top_shot:
			temp = { App->player->position.x - (position.x + 22), (App->player->position.y - 50) - (position.y + 78) };
			magnitude = sqrt((pow(temp.x, 2)) + (pow(temp.y, 2)));
			shot_vector = { (temp.x / magnitude) * 2, (temp.y / magnitude) * 2 };

			App->particles->AddParticle(App->particles->hosturball, position.x + 22, position.y + 78, COLLIDER_ENEMY_SHOT_WALL, shot_vector);
			last_shot = top_shot;
			current_shot = mid_shot;
			break;
		}
		shot_cd = fire_rate;
	}
	else shot_cd--;

	if (animation == &shoot && shoot.isDone()) {
		animation = &idle;
		shoot.Reset();
	}
}

void Enemy_Hostur::Draw(SDL_Texture* sprites) {
	Enemy::Draw(sprites);
	App->render->Blit(bckg_texture, background_position.x, background_position.y, nullptr);
}
