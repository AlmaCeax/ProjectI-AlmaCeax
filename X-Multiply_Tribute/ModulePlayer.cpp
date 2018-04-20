#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleParticles.h"
#include "ModuleAudio.h"
#include "ModuleCollision.h"
#include "ModuleFadeToBlack.h"
#include "ModuleSceneStart.h"
#include "ModuleUI.h"
#include "ModulePowerUPS.h"
#include "SDL_mixer/include/SDL_mixer.h"
#include "ModulePlayer.h"

enum pu_type;

ModulePlayer::ModulePlayer()
{

}

ModulePlayer::~ModulePlayer()
{}

bool ModulePlayer::Init() {
	// idle animation (arcade sprite sheet)
	idle.PushBack({ 100, 1, 36, 14 });
	idle.PushBack({ 100, 1, 36, 14 });
	idle.speed = 0.2f;

	// walk forward animation (arcade sprite sheet)
	//forward.frames.PushBack({9, 136, 53, 83});
	up.PushBack({ 53, 2, 36, 14 });
	up.PushBack({ 5, 2, 36, 14 });
	up.speed = 0.1f;
	up.loop = false;

	down.PushBack({ 149, 1, 36, 14 });
	down.PushBack({ 198, 1, 36, 14 });
	down.speed = 0.1f;
	down.loop = false;

	death.PushBack({ 11, 386, 40, 42 });
	death.PushBack({ 67, 386, 40, 42 });
	death.PushBack({ 120, 386, 40, 42 });
	death.PushBack({ 174, 386, 40, 42 });
	death.PushBack({ 219, 386, 40, 42 });
	death.loop = false;
	death.speed = 0.3f;

	return true;
}

// Load assets
bool ModulePlayer::Start()
{
	dead = false;

	position.x = 84;
	position.y = 115;

	injecting = true;
	startime = 35;
	death.Reset();
	canMove = false;
	startBoost = false;

	LOG("Loading player textures");
	graphics = App->textures->Load("Assets/Sprites/MainCharacter/spr_maincharacter.png"); // arcade version

	deadsfx = App->audio->LoadFx("Assets/Audio/SFX/xmultipl-018.wav");

	SDL_Rect rect_collider = { position.x,position.y,36,14 };
	collider = App->collision->AddCollider(rect_collider, COLLIDER_PLAYER, this);

	current_animation = &idle;
	state = idl;

	return true;
}

bool ModulePlayer::CleanUp() {
	App->textures->Unload(graphics);
	graphics = nullptr;

	if(collider!=nullptr) collider->to_delete = true;
	collider = nullptr;

	App->audio->UnloadSFX(deadsfx);
	deadsfx = nullptr;

	return true;
}

void ModulePlayer::OnCollision(Collider * rect_a, Collider * rect_b)
{
	if (rect_b->type != COLLIDER_POWER_UP) Die();
}

// Update: draw background
update_status ModulePlayer::PreUpdate()
{
	if (injecting) {
		App->render->Blit(graphics, position.x, position.y, new SDL_Rect({101, 1, 35, 14}));
	}

	return UPDATE_CONTINUE;
}
update_status ModulePlayer::Update()
{
	if (startBoost)
	{
		if (startime == 35) {
			App->particles->AddParticle(App->particles->playerBoost, position.x - 42, position.y, COLLIDER_NONE, { 3,0 });
		}
		position.x += speed.x;
		startime--;
		current_animation = &idle;
		state = idl;

		if (startime == 0)
		{
			startBoost = false;
			canMove = true;
		}
	}
	if (canMove) {
		current_animation = &idle;
		state = idl;

		if (activePU[TENTACLES] == true) {
			
		}

		if (App->input->keyboard[SDL_SCANCODE_D] == KEY_STATE::KEY_REPEAT || App->input->keyboard[SDL_SCANCODE_RIGHT] == KEY_STATE::KEY_REPEAT)
		{
			current_animation = &idle;
			position.x += speed.x;
			if (((position.x + 36) * SCREEN_SIZE) > (App->render->camera.x + SCREEN_WIDTH * SCREEN_SIZE)) position.x -= speed.x; //36 is player width
			state = idl;
		}
		if (App->input->keyboard[SDL_SCANCODE_A] == KEY_STATE::KEY_REPEAT || App->input->keyboard[SDL_SCANCODE_LEFT] == KEY_STATE::KEY_REPEAT)
		{
			current_animation = &idle;
			position.x -= speed.x;
			if ((position.x * SCREEN_SIZE) < App->render->camera.x) position.x += speed.x;
			state = idl;
		}
		if (App->input->keyboard[SDL_SCANCODE_W] == KEY_STATE::KEY_REPEAT || App->input->keyboard[SDL_SCANCODE_UP] == KEY_STATE::KEY_REPEAT)
		{
			if (activePU[TENTACLES] == false) {
				current_animation = &up;
			}
			position.y -= speed.x;
			if ((position.y * SCREEN_SIZE) < App->render->camera.y) position.y += speed.x;
			state = top;
		}

		if (App->input->keyboard[SDL_SCANCODE_S] == KEY_STATE::KEY_REPEAT || App->input->keyboard[SDL_SCANCODE_DOWN] == KEY_STATE::KEY_REPEAT)
		{
			if (activePU[TENTACLES] == false) {
				current_animation = &down;
			}
			position.y += speed.x;
			if (((position.y + 44) * SCREEN_SIZE) > (App->render->camera.y + SCREEN_HEIGHT * SCREEN_SIZE)) position.y -= speed.x;
			state = bot;
		}

		if (App->input->keyboard[SDL_SCANCODE_SPACE] == KEY_STATE::KEY_DOWN)
		{
			App->particles->AddParticle(App->particles->baseShotExp, position.x + 30, position.y + 1);
			App->particles->AddParticle(App->particles->baseShot, position.x + 25, position.y + 5, COLLIDER_PLAYER_SHOT);
		}
		if (App->input->keyboard[SDL_SCANCODE_SPACE] == KEY_STATE::KEY_REPEAT)
		{
			if (cooldown < 25) {
				cooldown++;
			}
			else {
				App->particles->AddParticle(App->particles->baseShotExp, position.x + 30, position.y + 1);
				App->particles->AddParticle(App->particles->baseShot, position.x + 25, position.y + 5, COLLIDER_PLAYER_SHOT);
				if (activePU[BOMB] == true)App->particles->AddParticle(App->particles->bombshot, position.x + 25, position.y + 5, COLLIDER_PLAYER_SHOT);
				cooldown = 0;
			}
		}
		if (App->input->keyboard[SDL_SCANCODE_F4] == KEY_STATE::KEY_DOWN && !dead) Die();

		if (last_animation != current_animation) {
			current_animation->Reset();
		}
		last_animation = current_animation;
	}
	else {
		if (current_animation->isDone() && dead) App->ui->DeathFade();
	}	

	// Draw everything --------------------------------------
	SDL_Rect r = current_animation->GetCurrentFrame();
	collider->SetPos(position.x, position.y);
	App->render->Blit(graphics, position.x, position.y, &r);

	return UPDATE_CONTINUE;
}

void ModulePlayer::Die() {
	position.x -= 5;
	position.y -=10;
	dead = true;
	canMove = false;
	current_animation = &death;
	collider->to_delete = true;
	speed.x = 2;
	speed.y = 2;
	
	Mix_PlayChannel(-1, deadsfx, 0);
	for (int i = 0; i < sizeof(activePU); i++)
	{
		activePU[i] = false;
	}
}

void ModulePlayer::BlitPlayer() {
	App->render->Blit(graphics, position.x, position.y, &idle.GetCurrentFrame());
}