#include "Application.h"
#include "ModuleCollision.h"
#include "ModuleParticles.h"
#include "ModuleRender.h"
#include "ModuleUI.h"
#include "SDL_mixer\include\SDL_mixer.h"
#include "Enemy.h"
#include <stdlib.h> 

Enemy::Enemy(int x, int y) : position(x, y)
{}

Enemy::~Enemy()
{
	if (collider != nullptr)
		collider->to_delete = true;
}

const Collider* Enemy::GetCollider() const
{
	return collider;
}

void Enemy::Draw(SDL_Texture* sprites)
{
	if(collider != nullptr)
		collider->SetPos(position.x, position.y);

	if (animation != nullptr) {
		if(!hit)App->render->BlitFlipped(sprites, position.x, position.y, &(animation->GetCurrentFrame()), flipX, flipY, rangle, center);
		else {
			hitAnimation->setCurrentFrameIndex(animation->GetCurrentFrameIndex()-1);
			App->render->BlitFlipped(sprites, position.x, position.y, &(hitAnimation->GetCurrentFrame()), flipX, flipY);
			hit = false;
		}
	}
	
}

void Enemy::OnCollision(Collider* collider)
{
	for (int i = 0; i < numExplosions; i++)
	{
		if(i == 0)App->particles->AddParticle(App->particles->enemyExplosion, position.x, position.y);
		else {
			App->particles->AddParticle(App->particles->multipleLittleExplosion, position.x, position.y, COLLIDER_TYPE::COLLIDER_NONE, {0, 0},0, 4, true);
		}
	}
	App->ui->AddScore(points);
	isdead = true;
}
void Enemy::Shine()
{
	hit = true;
}