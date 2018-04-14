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
#include "SDL_mixer/include/SDL_mixer.h"
#include "ModulePlayer.h"

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

	return true;
}

// Load assets
bool ModulePlayer::Start()
{
	position.x = 75;
	position.y = 105;

	LOG("Loading player textures");
	graphics = App->textures->Load("Assets/Sprites/MainCharacter/spr_maincharacter.png"); // arcade version
	LOG("Loading player audio");
	baseshotsfx = App->audio->LoadFx("Assets/Audio/SFX/xmultipl-083.wav");

	SDL_Rect rect_collider = { position.x,position.y,36,14 };
	collider = App->collision->AddCollider(rect_collider, COLLIDER_PLAYER, this);

	return true;
}

bool ModulePlayer::CleanUp() {
	App->textures->Unload(graphics);
	graphics = nullptr;

	App->collision->RemoveCollider(collider);
	collider = nullptr;

	App->audio->UnloadSFX(baseshotsfx);
	baseshotsfx = nullptr;

	return true;
}

void ModulePlayer::OnCollision(Collider * rect_a, Collider * rect_b)
{
	switch (rect_b->type) {
	case COLLIDER_WALL: App->fade->FadeToBlack(App->current_scene, App->start); break;

	}
}

// Update: draw background
update_status ModulePlayer::Update()
{
	Animation* current_animation = &idle;
	state = idl;
	int speed = 2;

	if (App->input->keyboard[SDL_SCANCODE_D] == KEY_STATE::KEY_REPEAT)
	{
		current_animation = &idle;
		position.x += speed;
		if (((position.x+36) * SCREEN_SIZE) > (App->render->camera.x + SCREEN_WIDTH)) position.x -= speed; //36 is player width
		state = idl;
	}
	if (App->input->keyboard[SDL_SCANCODE_A] == KEY_STATE::KEY_REPEAT)
	{
		current_animation = &idle;
		position.x -= speed;
		if ((position.x * SCREEN_SIZE) < App->render->camera.x) position.x += speed;
		state = idl;
	}
	if (App->input->keyboard[SDL_SCANCODE_W] == KEY_STATE::KEY_REPEAT)
	{
		current_animation = &up;
		position.y -= speed;
		if ((position.y * SCREEN_SIZE) < App->render->camera.y) position.y += speed;
		state = top;
	}

	if (App->input->keyboard[SDL_SCANCODE_S] == KEY_STATE::KEY_REPEAT)
	{
		current_animation = &down;
		position.y += speed;
		if (((position.y + 14) * SCREEN_SIZE) > (App->render->camera.y + SCREEN_HEIGHT)) position.y -= speed;
		state = bot;
	}

	if (App->input->keyboard[SDL_SCANCODE_B] == KEY_STATE::KEY_DOWN)
	{	
		App->particles->AddParticle(App->particles->baseShotExp, position.x + 30, position.y+1);
		Mix_PlayChannel(-1, baseshotsfx, 0);
		App->particles->AddParticle(App->particles->baseShot, position.x+25, position.y+5);
	}

	collider->SetPos(position.x, position.y);

	if (last_animation != current_animation) {
		current_animation->Reset();
	}
	last_animation = current_animation;

	// Draw everything --------------------------------------
	SDL_Rect r = current_animation->GetCurrentFrame();

	App->render->Blit(graphics, position.x, position.y, &r);

	return UPDATE_CONTINUE;
}