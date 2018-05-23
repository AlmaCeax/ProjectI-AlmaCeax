#include <math.h>
#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModulePlayer.h"
#include "ModuleUI.h"
#include "ModuleAudio.h"
#include "ModuleCollision.h"
#include "ModuleParticles.h"
#include "ModuleEnemies.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h>  
#include "SDL/include/SDL_timer.h"
#include "SDL_mixer/include/SDL_mixer.h"


ModuleParticles::ModuleParticles()
{
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
		active[i] = nullptr;

	// BaseShot particle

	baseShotExp.anim.PushBack({ 33, 36, 7, 6 });
	baseShotExp.anim.PushBack({ 49, 34, 12, 12 });
	baseShotExp.anim.loop = false;
	baseShotExp.anim.speed = 0.5f;
	baseShotExp.life = 100;
	baseShotExp.speed = { 1,0 };
	baseShotExp.isPlayerAttached = true;
	baseShotExp.offsetx = 30;
	baseShotExp.offsety = 1;

	baseShotColExp.anim.PushBack({ 81, 34, 12, 12 });
	baseShotColExp.anim.PushBack({ 101, 36, 8, 9 });
	baseShotColExp.anim.loop = false;
	baseShotColExp.anim.speed = 0.3f;
	baseShotColExp.life = 100;
	baseShotColExp.speed = { 0,0 };

	baseShot.w = 17;
	baseShot.h = 4;
	baseShot.anim.PushBack({ 63, 38, 17, 4 });
	baseShot.anim.loop = false;
	baseShot.anim.speed = 0.3f;
	baseShot.life = 1000;
	baseShot.speed = { 10,0 };
	baseShot.id = 1;

	tentacleBaseShot.anim.PushBack({ 115, 38, 13, 5 });
	tentacleBaseShot.anim.loop = false;
	tentacleBaseShot.anim.speed = 0.3f;
	tentacleBaseShot.life = 1000;
	tentacleBaseShot.speed = { 10,0 };
	tentacleBaseShot.id = 1;

	playerBoost.h = 4;
	playerBoost.anim.PushBack({ 251, 208, 42, 14 });
	playerBoost.anim.PushBack({ 295, 208, 42, 13 });
	playerBoost.anim.PushBack({ 342, 208, 42, 11 });
	playerBoost.anim.loop = true;
	playerBoost.anim.speed = 0.2f;
	playerBoost.life = 525;
	playerBoost.speed = { 0,0 };
	playerBoost.id = 3;

	bombExplosion.anim.PushBack({ 97, 171, 16, 16 });
	bombExplosion.anim.PushBack({ 122, 166, 26, 26 });
	bombExplosion.anim.PushBack({ 156, 166, 26, 26 });
	bombExplosion.anim.PushBack({ 188, 166, 26, 26 });
	bombExplosion.anim.PushBack({ 220, 166, 26, 26 });
	bombExplosion.anim.loop = false;
	bombExplosion.anim.speed = 0.7f;
	bombExplosion.life = 200;
	bombExplosion.speed = { 0, 0 };


	bombshot.anim.PushBack({ 4, 170, 15, 4 });
	bombshot.anim.PushBack({ 22, 170, 14, 8 });
	bombshot.anim.PushBack({ 38, 166, 12, 12 });
	bombshot.anim.PushBack({ 58, 165, 9, 14 });
	bombshot.anim.PushBack({ 78, 164, 4, 15 });
	bombshot.anim.loop = false;
	bombshot.anim.speed = 0.03f;
	bombshot.life = 2000;
	bombshot.speed = { 4, 1 };
	bombshot.id = 2;

	blueBall.anim.PushBack({ 10,217,8,8 });
	blueBall.anim.PushBack({ 26,217,8,8 });
	blueBall.anim.PushBack({ 42,217,8,8 });
	blueBall.anim.PushBack({ 58,217,8,8 });
	blueBall.anim.speed = 0.1f;
	blueBall.life = 3000;
	blueBall.speed = { 0,0 };

	enemyExplosion.anim.PushBack({ 103, 284, 26, 22 });
	enemyExplosion.anim.PushBack({ 135, 284, 26, 22 });
	enemyExplosion.anim.PushBack({ 166, 279, 44, 32 });
	enemyExplosion.anim.PushBack({ 217, 280, 37, 28 });
	enemyExplosion.anim.PushBack({ 262, 280, 42, 31 });
	enemyExplosion.anim.PushBack({ 106, 325, 39, 36 });
	enemyExplosion.anim.PushBack({ 155, 324, 36, 35 });
	enemyExplosion.anim.PushBack({ 202, 326, 36, 32 });
	enemyExplosion.anim.PushBack({ 250, 324, 37, 40 });
	enemyExplosion.anim.loop = false;
	enemyExplosion.anim.speed = 0.5f;
	enemyExplosion.life = 200;
	enemyExplosion.speed = { 0, 0 };
	enemyExplosion.id = 2;

	enemyBossExplosion.anim.PushBack({ 254, 11, 46, 40 });
	enemyBossExplosion.anim.PushBack({ 302, 11, 46, 40 });
	enemyBossExplosion.anim.PushBack({ 352, 4, 60, 56 });
	enemyBossExplosion.anim.PushBack({ 413, 5, 26, 26 });
	enemyBossExplosion.anim.PushBack({ 254, 63, 63, 63 });
	enemyBossExplosion.anim.PushBack({ 318, 68, 62, 54 });
	enemyBossExplosion.anim.PushBack({ 382, 64, 61, 63 });
	enemyBossExplosion.anim.PushBack({ 449, 70, 60, 50 });
	enemyBossExplosion.anim.PushBack({ 258, 132, 58, 56 });
	enemyBossExplosion.anim.loop = false;
	enemyBossExplosion.anim.speed = 0.5f;
	enemyBossExplosion.life = 200;
	enemyBossExplosion.speed = { 0, 0 };

	cyclopshot.anim.PushBack({ 9,234,12,12 });
	cyclopshot.anim.PushBack({ 26,234,12,12 });
	cyclopshot.anim.PushBack({ 45,234,12,12 });
	cyclopshot.anim.PushBack({ 61,234,12,12 });
	cyclopshot.anim.PushBack({ 9,252,12,12 });
	cyclopshot.anim.PushBack({ 26,252,12,12 });
	cyclopshot.anim.PushBack({ 44,252,12,12 });
	cyclopshot.anim.speed = 0.2f;
	cyclopshot.life = 2000;
	cyclopshot.speed = { 0,0 };
	cyclopshot.anim.loop;
	cyclopshot.id = 7;

	alienshot.anim.PushBack({ 9,275,16,14 });
	alienshot.anim.PushBack({ 31,275,16,14 });
	//alienshot.anim.PushBack({ 54,275,16,14 });
	alienshot.anim.PushBack({ 31,275,16,14 });
	alienshot.anim.speed = 0.2f;
	alienshot.life = 2000;
	alienshot.speed = { 0,0 };
	alienshot.anim.loop = true;

	browneyeshot1.anim.PushBack({ 651,40,11,8 });
	browneyeshot1.anim.PushBack({ 652,64,18,11 });
	browneyeshot1.anim.speed = 0.1f;
	browneyeshot1.life = 3000;
	browneyeshot1.speed = { 0,0 };
	browneyeshot1.anim.loop = true;

	browneyeshot2.anim.PushBack({ 746,224,8,19 });
	browneyeshot2.anim.PushBack({ 707,184,10,24 });
	browneyeshot2.anim.speed = 0.1f;
	browneyeshot2.life = 3000;
	browneyeshot2.speed = { 0,0 };
	browneyeshot2.anim.loop = true;

	browneyeshot3.anim.PushBack({ 537,225,10,8 });
	browneyeshot3.anim.PushBack({ 535,199,16,15 });
	browneyeshot3.anim.speed = 0.1f;
	browneyeshot3.life = 3000;
	browneyeshot3.speed = { 0,0 };
	browneyeshot3.anim.loop = true;

	browneyeshot4.anim.PushBack({ 680,56,19,8 });
	browneyeshot4.anim.PushBack({ 680,70,30,11 });
	browneyeshot4.anim.speed = 0.1f;
	browneyeshot4.life = 3000;
	browneyeshot4.speed = { 0,0 };
	browneyeshot4.anim.loop = true;

	browneyeshot5.anim.PushBack({ 531,150,25,10 });
	browneyeshot5.anim.PushBack({ 560,144,30,11 });
	browneyeshot5.anim.speed = 0.1f;
	browneyeshot5.life = 3000;
	browneyeshot5.speed = { 0,0 };
	browneyeshot5.anim.loop = true;

	browneyeshot6.anim.PushBack({ 537,53,10,9 });
	browneyeshot6.anim.PushBack({ 533,35,18,11 });
	browneyeshot6.anim.speed = 0.1f;
	browneyeshot6.life = 3000;
	browneyeshot6.speed = { 0,0 };
	browneyeshot6.anim.loop = true;

	browneyeshot7.anim.PushBack({ 586,129,7,11 });
	browneyeshot7.anim.PushBack({ 567,95,8,18 });
	browneyeshot7.anim.speed = 0.1f;
	browneyeshot7.life = 3000;
	browneyeshot7.speed = { 0,0 };
	browneyeshot7.anim.loop = true;

	browneyeshot8.anim.PushBack({ 720,75,18,11 });
	browneyeshot8.anim.PushBack({ 598,151,21,15 });
	browneyeshot8.anim.speed = 0.1f;
	browneyeshot8.life = 3000;
	browneyeshot8.speed = { 0,0 };
	browneyeshot8.anim.loop = true;

	greenBall.anim.PushBack({ 644,273,12,10 });
	greenBall.anim.PushBack({ 599,303,14,14 });
	greenBall.anim.speed = 0.02f;
	greenBall.life = 3000;
	greenBall.speed = { 0,0 };

	multipleLittleExplosion.life = 500;
	multipleLittleExplosion.id = 8;

	multipleBigExplosion.life = 1500;
	multipleBigExplosion.id = 9;

	yellowCircle.anim.PushBack({ 14,321,16,15 });
	yellowCircle.anim.PushBack({ 38,318,16,15 });
	yellowCircle.anim.PushBack({ 65,316,16,15 });
	yellowCircle.anim.speed = 0.1f;
	yellowCircle.anim.loop = false;
	yellowCircle.life = 3000;
	yellowCircle.speed = { 0,0 };

	blueCircle.anim.PushBack({ 5,149,16,16 });
	blueCircle.anim.PushBack({ 23,148,16,16 });
	blueCircle.anim.PushBack({ 44,147,16,16 });
	blueCircle.anim.PushBack({ 66,145,16,16 });
	blueCircle.anim.PushBack({ 88,14,16,16 });
	blueCircle.anim.PushBack({ 106,144,16,16 });
	blueCircle.anim.PushBack({ 123,144,16,16 });
	blueCircle.anim.PushBack({ 140,144,16,16 });
	blueCircle.anim.speed = 0.1f;
	blueCircle.anim.loop = false;
	blueCircle.life = 3000;
	blueCircle.speed = { 0,0 };

	redBall.anim.PushBack({ 14,460,18,14 });
	redBall.anim.speed = 0.1f;
	redBall.anim.loop = false;
	redBall.life = 3000;
	redBall.speed = { 0,0 };
	redBall.id = 10;

	hosturballmid.anim.PushBack({ 14,490,18,18 });
	hosturballmid.anim.PushBack({ 37,490,18,18 });
	hosturballmid.anim.PushBack({ 61,490,18,18 });
	hosturballmid.anim.PushBack({ 85,490,18,18 });
	hosturballmid.life = 3000;
	hosturballmid.anim.speed = 0.1f;
	hosturballmid.anim.loop = true;
	hosturballmid.id = 11;

	hosturball.anim.PushBack({ 14,490,18,18 });
	hosturball.anim.PushBack({ 37,490,18,18 });
	hosturball.anim.PushBack({ 61,490,18,18 });
	hosturball.anim.PushBack({ 85,490,18,18 });
	hosturball.life = 3000;
	hosturball.anim.speed = 0.1f;
	hosturball.anim.loop = true;
	hosturball.id = 12;

	hosturballmiddeath.anim.PushBack({14,523,18,18});
	hosturballmiddeath.anim.PushBack({35,523,18,18});
	hosturballmiddeath.anim.PushBack({54,525,18,18});
	hosturballmiddeath.anim.PushBack({77,524,20,20});
	hosturballmiddeath.anim.PushBack({102,523,22,22});
	hosturballmiddeath.anim.PushBack({130,522,24,24});
	hosturballmiddeath.life = 3000;
	hosturballmiddeath.id = 13;

	hosturballdeath.anim.PushBack({ 14,523,18,18 });
	hosturballdeath.anim.PushBack({ 35,523,18,18 });
	hosturballdeath.anim.PushBack({ 54,525,18,18 });
	hosturballdeath.anim.PushBack({ 77,524,20,20 });
	hosturballdeath.anim.PushBack({ 102,523,22,22 });
	hosturballdeath.anim.PushBack({ 130,522,24,24 });
	hosturballdeath.life = 3000;
	hosturballdeath.id = 14;

	hostursmallshot.anim.PushBack({ 97,565,11,7 });
	hostursmallshot.life = 3000;
	hostursmallshot.anim.loop = true;
	hostursmallverticalshot.anim.PushBack({ 87,563,7,11 });
	hostursmallverticalshot.life = 3000;
	hostursmallverticalshot.anim.loop = true;
	hosturlongshot.anim.PushBack({ 166,563,12,12 });
	hosturlongshot.life = 3000;
	hosturlongshot.anim.loop = true;

	hosturbigshot.anim.PushBack({ 50,561,10,11 });
	hosturbigshot.life = 3000;
	hosturbigshot.anim.loop = true;
	hosturbiglargeshot.anim.PushBack({ 49,574,12,11 });
	hosturbiglargeshot.life = 3000;
	hosturbiglargeshot.anim.loop = true;


	missile.anim.PushBack({ 6,73,27,9 });
	missile.anim.PushBack({ 37,73,27,9 });
	missile.anim.PushBack({ 65,72,27,9 });
	missile.anim.PushBack({ 104,72,27,9 });
	missile.anim.speed = 0.08f;
	missile.anim.loop = false;
	missile.life = 3000;
	missile.speed = { 0,0 };
	missile.id = 15;
	missile.preparation = true;
	missile.center = { 27, 4 };

}

ModuleParticles::~ModuleParticles()
{}

// Load assets
bool ModuleParticles::Start()
{
	LOG("Loading particles");
	graphics = App->textures->Load("Assets/Sprites/MainCharacter/spr_maincharacter.png");

	baseShotExp.sfx = App->audio->LoadFx("Assets/Audio/SFX/xmultipl-083.wav");
	playerBoost.sfx = App->audio->LoadFx("Assets/Audio/SFX/xmultipl-023.wav");
	hosturball.sfx = App->audio->LoadFx("Assets/Audio/SFX/xmultipl-029.wav");
	hosturballmid.sfx = hosturball.sfx;
	hosturballdeath.sfx = App->audio->LoadFx("Assets/Audio/SFX/xmultipl-028.wav");
	hosturballmiddeath.sfx = hosturballdeath.sfx;

	return true;
}

// Unload assets
bool ModuleParticles::CleanUp()
{
	LOG("Unloading particles");
	App->textures->Unload(graphics);
	graphics = nullptr;

	App->audio->UnloadSFX(baseShotExp.sfx);
	baseShot.sfx = nullptr;

	App->audio->UnloadSFX(playerBoost.sfx);
	playerBoost.sfx = nullptr;

	App->audio->UnloadSFX(hosturball.sfx);
	hosturball.sfx = nullptr;
	hosturballmid.sfx = nullptr;
	App->audio->UnloadSFX(hosturballdeath.sfx);
	hosturballdeath.sfx = nullptr;
	hosturballmiddeath.sfx = nullptr;

	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		if (active[i] != nullptr)
		{
			App->audio->UnloadSFX(active[i]->sfx);
			active[i]->sfx = nullptr;
			delete active[i];
			active[i] = nullptr;
		}
	}

	return true;
}

// Update: draw background
update_status ModuleParticles::Update()
{
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		Particle* p = active[i];

		if (p == nullptr)
			continue;

		if (p->Update() == false)
		{
			delete p;
			active[i] = nullptr;			
		}
		else if (SDL_GetTicks() >= p->born)
		{
			App->render->BlitFlipped(graphics, p->position.x, p->position.y, &(p->anim.GetCurrentFrame()),p->flipX, p->flipY, p->rangle, p->center);
			if (p->fx_played == false)
			{
				p->fx_played = true;
				if(p->id != 3)Mix_PlayChannel(p->id, p->sfx, 0);
			}
		}
	}
	return UPDATE_CONTINUE;
}


void ModuleParticles::AddParticle(const Particle& particle, int x, int y, COLLIDER_TYPE collider_type, fPoint speed, Uint32 delay, int nTimes, bool isMultiple, bool flipX, bool flipY, bool _up, iPoint offset)
{
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		if (active[i] == nullptr)
		{
			Particle* p = new Particle(particle);
			p->born = SDL_GetTicks() + delay;
			p->position.x = x;
			p->position.y = y;
			p->flipX = flipX;
			p->flipY = flipY;
			p->missileUp = _up;
			p->offsetx = offset.x;
			p->offsety = offset.y;
			if (!speed.IsZero()) p->speed = speed;
			if (collider_type != COLLIDER_NONE)
				p->collider = App->collision->AddCollider(p->anim.GetCurrentFrame(), collider_type, this);
			active[i] = p;
			break;
		}
	}
}

void ModuleParticles::OnCollision(Collider* c1, Collider* c2)
{
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		// Always destroy particles that collide
		if (active[i] != nullptr && active[i]->collider == c1)
		{
			switch (active[i]->id) {
			case 1: AddParticle(baseShotColExp, active[i]->position.x, active[i]->position.y); break;
			case 2: AddParticle(bombExplosion, active[i]->position.x, active[i]->position.y); break;
			case 10: if (c2->type == COLLIDER_PLAYER_SHOT) App->ui->AddScore(100); break;
			case 11: App->particles->AddParticle(App->particles->hosturballmiddeath, active[i]->position.x, active[i]->position.y); break;
			case 12: App->particles->AddParticle(App->particles->hosturballdeath, active[i]->position.x, active[i]->position.y); break;
			}
			delete active[i];
			active[i] = nullptr;
			break;
		}
	}
}

// -------------------------------------------------------------
// -------------------------------------------------------------

Particle::Particle()
{
	position.SetToZero();
	speed.SetToZero();
}

Particle::Particle(const Particle& p) :
	anim(p.anim), position(p.position), speed(p.speed),
	fx(p.fx), born(p.born), life(p.life), isPlayerAttached(p.isPlayerAttached), offsetx(p.offsetx), offsety(p.offsety), sfx(p.sfx), id(p.id), nTimes(p.nTimes), isMultiple(p.isMultiple), flipX(p.flipX), flipY(p.flipY), missileUp(p.missileUp)
{}

Particle::~Particle()
{
	if (collider != nullptr)
		collider->to_delete = true;
}

bool Particle::Update()
{
	bool ret = true;

	if (life > 0)
	{
		if ((SDL_GetTicks() - born) > life)
			ret = false;
	}
	else
		if (anim.Finished())
			ret = false;


	switch (id)
	{
	case 1:
		position.x += speed.x;
		position.y += speed.y; break;
	case 2:
		position.x += speed.x;
		position.y += speed.y;
		if (speed.x > 1.55f)speed.x -= 0.1f;
		if (speed.y < 2.0f)speed.y += 0.1f;
		break;
	case 3:
		position.x = App->player->position.x - 42;
		position.y = App->player->position.y;
		break;
	case 7:
		position.x += speed.x;
		position.y += speed.y;
		speed.y += 0.1f;
		break;
	case 8:
		timebeforeanotherexplosion++;
		if (timebeforeanotherexplosion == 10)
		{
			App->particles->AddParticle(App->particles->enemyExplosion, position.x + rand() % 40 + (-20), position.y + rand() % 40 + (-20));
			timebeforeanotherexplosion = 0;
		}
		break;
	case 9:
		timebeforeanotherexplosion++;
		if (timebeforeanotherexplosion == 5)
		{
			App->particles->AddParticle(App->particles->enemyBossExplosion, position.x + rand() % 150 + (-75), position.y + rand() % 150 + (-75));
			timebeforeanotherexplosion = 0;
		}
		break;
	case 11:
		position.x += speed.x;
		position.y += speed.y;
		if (position.x < 4640) {
			App->particles->AddParticle(App->particles->hosturballmiddeath, position.x, position.y);
			ret = false;
		}
		break;
	case 10:
		position.x += speed.x;
		position.y += speed.y;
		speed.y += 0.1f;
		break;
		position.x += speed.x;
		position.y += speed.y;
		if (position.x < 4640) {
			App->particles->AddParticle(App->particles->hosturballmiddeath, position.x, position.y);
			ret = false;
		}
		break;
	case 12:
		position.x += speed.x;
		position.y += speed.y;
		if (position.x < 4726) {
			App->particles->AddParticle(App->particles->hosturballdeath, position.x, position.y);
			ret = false;
		}
		break;
	case 13:
		if (anim.Finished()) {
			App->particles->AddParticle(App->particles->hosturbigshot, position.x + 12, position.y + 10, COLLIDER_ENEMY_SHOT_WALL, { 1,-2 }, 0, 1, false, false, false);
			App->particles->AddParticle(App->particles->hosturbigshot, position.x + 12, position.y + 10, COLLIDER_ENEMY_SHOT_WALL, { -1,-2 }, 0, 1, false, true, false);
			App->particles->AddParticle(App->particles->hosturbigshot, position.x + 12, position.y + 10, COLLIDER_ENEMY_SHOT_WALL, { 1,2 }, 0, 1, false, false, true);
			App->particles->AddParticle(App->particles->hosturbigshot, position.x + 12, position.y + 10, COLLIDER_ENEMY_SHOT_WALL, { -1,2 }, 0, 1, false, true, true);
			App->particles->AddParticle(App->particles->hosturbiglargeshot, position.x + 12, position.y + 10, COLLIDER_ENEMY_SHOT_WALL, { 2, -1 }, 0, 1, false, false, false);
			App->particles->AddParticle(App->particles->hosturbiglargeshot, position.x + 12, position.y + 10, COLLIDER_ENEMY_SHOT_WALL, { 2, 1 }, 0, 1, false, false, true);
			App->particles->AddParticle(App->particles->hosturbiglargeshot, position.x + 12, position.y + 10, COLLIDER_ENEMY_SHOT_WALL, { -2, -1 }, 0, 1, false, true, false);
			App->particles->AddParticle(App->particles->hosturbiglargeshot, position.x + 12, position.y + 10, COLLIDER_ENEMY_SHOT_WALL, { -2, 1 }, 0, 1, false, true, true);
			ret = false;
		}
		break;
	case 14:
		if (anim.Finished()) {
			App->particles->AddParticle(App->particles->hostursmallverticalshot, position.x + 12, position.y + 10, COLLIDER_ENEMY_SHOT_WALL, { 0, 2 }, 0, 1, false, false, false);
			App->particles->AddParticle(App->particles->hostursmallshot, position.x + 12, position.y + 10, COLLIDER_ENEMY_SHOT_WALL, { 2, 0 }, 0, 1, false, false, false);
			App->particles->AddParticle(App->particles->hostursmallverticalshot, position.x + 12, position.y + 10, COLLIDER_ENEMY_SHOT_WALL, { 0, -2 }, 0, 1, false, false, true);
			App->particles->AddParticle(App->particles->hostursmallshot, position.x + 12, position.y + 10, COLLIDER_ENEMY_SHOT_WALL, { -2, 0 }, 0, 1, false, true, false);
			App->particles->AddParticle(App->particles->hosturlongshot, position.x + 12, position.y + 10, COLLIDER_ENEMY_SHOT_WALL, { 2, -2 }, 0, 1, false, false, false);
			App->particles->AddParticle(App->particles->hosturlongshot, position.x + 12, position.y + 10, COLLIDER_ENEMY_SHOT_WALL, { 2, 2 }, 0, 1, false, false, true);
			App->particles->AddParticle(App->particles->hosturlongshot, position.x + 12, position.y + 10, COLLIDER_ENEMY_SHOT_WALL, { -2, 2 }, 0, 1, false, true, true);
			App->particles->AddParticle(App->particles->hosturlongshot, position.x + 12, position.y + 10, COLLIDER_ENEMY_SHOT_WALL, { -2,-2 }, 0, 1, false, true, false);
			ret = false;
			break;
	case 15:
		if (preparation)
		{
			speed.x = -1;
			if (missileUp)speed.y = -1;
			else speed.y = +1;
			position.x = App->player->position.x + offsetx;
			position.y = App->player->position.y + offsety;
			offsetx += speed.x;
			offsety += speed.y;
			if (preparationtimer == 15) {
				preparation = false;
				target = closerTarget(position);
			}
			else preparationtimer++;
		}
		else {
			anim.frames[0] = anim.frames[2];
			anim.frames[1] = anim.frames[3];
			anim.speed = 0.1f;
			anim.loop = true;
			speed = { 4, 0 };
			if (target != nullptr) {
				float distance = position.DistanceTo({ target->position.x+((target->w)/2), target->position.y + ((target->h) / 2) });
				fPoint direction = { (target->position.x + ((target->w) / 2)) / distance - position.x / distance, (target->position.y + ((target->h) / 2)) / distance - position.y / distance };
				position.x += direction.x * 4;
				position.y += direction.y * 4;
				float dot = (position.x * target->position.x) + (position.y * target->position.y);
				float det = (position.x * target->position.y) - (position.y * target->position.x);
				rangle += (SDL_atan2(det, dot));
			}
			else position.x += speed.x;
		}
		break;
	default:
		position.x += speed.x;
		position.y += speed.y; break;
		}
	}
	if (isPlayerAttached) {

		if (App->player->state == App->player->idl)	offsety = 1;
		else if (App->player->state == App->player->top) offsety = -1;
		else if (App->player->state == App->player->bot) offsety = 2;
		position.y = App->player->position.y + offsety;
		position.x = (App->player->position.x + 30) + offsetx;
	}

	if (collider != nullptr) {
		collider->SetPos(position.x, position.y);
		collider->SetShape(anim.GetCurrentFrame().w, anim.GetCurrentFrame().h);
	}
	return ret;
}

Enemy* Particle::closerTarget(iPoint _position)
{
	int aux = 0;
	iPoint auxpos = { 0,0 };
	int closer = 1000;
	Enemy* closerEnemy = nullptr;
	for (int i = 0; i < 99; i++)
	{
		if (App->enemies->enemies[i] != nullptr)
		{
			auxpos = App->enemies->enemies[i]->position;
			aux = _position.DistanceTo(auxpos);
			if (aux < closer) {
				closer = aux;
				closerEnemy = App->enemies->enemies[i];
			}
		}
	}

	return closerEnemy;
}