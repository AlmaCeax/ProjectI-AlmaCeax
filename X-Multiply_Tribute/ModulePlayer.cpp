#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModulePlayer.h"

ModulePlayer::ModulePlayer()
{
	position.x = 10;
	position.y = 50;

	// idle animation (arcade sprite sheet)
	idle.PushBack({ 100, 1, 36, 14 });
	idle.speed = 0.2f;

	// walk forward animation (arcade sprite sheet)
	//forward.frames.PushBack({9, 136, 53, 83});
	up.PushBack({ 78, 131, 60, 88 });
	up.PushBack({ 162, 128, 64, 92 });
	up.PushBack({ 259, 128, 63, 90 });
	up.PushBack({ 352, 128, 54, 91 });
	up.PushBack({ 432, 131, 50, 89 });
	up.speed = 0.1f;

}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player textures");
	bool ret = true;
	graphics = App->textures->Load("Assets/Sprites/MainCharacter/spr_maincharacter.png"); // arcade version
	return ret;
}

// Update: draw background
update_status ModulePlayer::Update()
{
	Animation* current_animation = &idle;

	int speed = 1;

	if (App->input->keyboard[SDL_SCANCODE_D] == 1)
	{
		current_animation = &up;
		position.x += speed;
	}

	// Draw everything --------------------------------------
	SDL_Rect r = current_animation->GetCurrentFrame();

	App->render->Blit(graphics, position.x, position.y - r.h, &r);

	return UPDATE_CONTINUE;
}