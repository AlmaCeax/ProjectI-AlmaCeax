#include "Application.h"
#include "ModuleAudio.h"
#include "ModuleTextures.h"
#include "ModuleParticles.h"
#include "ModulePowerUPS.h"
#include "ModuleRender.h"
#include "ModuleCollision.h"
#include "SDL_mixer\include\SDL_mixer.h"
#include "ModuleSceneStage3.h"



ModuleSceneStage3::ModuleSceneStage3()
{
	for (int i = 0; i < 3; ++i)
	{
		textures[i] = nullptr;
	}
}


ModuleSceneStage3::~ModuleSceneStage3()
{

}


bool ModuleSceneStage3::Start() {
	App->current_stage = this;

	App->player->Enable();
	App->collision->Enable();
	App->powerups->Enable();
	App->particles->Enable();
	App->ui->PlayerReady();
	App->player->position = { -100, 450 };

	App->render->SetCameraPosition(0, 336);

	music = App->audio->LoadMusic("Assets/Audio/Music/06_The_Rolling_Worms_Stage_3-1_.ogg");
	textures[0] = App->textures->Load("Assets/Sprites/Stages/Stage3/Backgroundbg03.png");
	textures[1] = App->textures->Load("Assets/Sprites/Stages/Stage3/Layerbg03.png");

	right = false;
	left = false;
	up = false;
	down = false;


	return true;
}

bool ModuleSceneStage3::Init()
{
	score_bonus = 20000;
	index = 3;
	rect[0] = { 0,0,3546,544 };
	rect[1] = { 0,0,5104,561 };
	return true;
}

void ModuleSceneStage3::UpdateCamera()
{
	int speed = 1;
	if (stopped || !App->player->canMove) return;

	if (right) {
		App->render->camera.x += speed * SCREEN_SIZE;
		App->player->position.x += speed;
	}
	if (left)App->render->camera.x -= speed * SCREEN_SIZE;
	if (up) {
		timer++;
		if (timer >= 3) {
			App->render->camera.y -= speed * SCREEN_SIZE;
			App->player->position.y -= 1;
			timer = 0;
		}
	}
	if (down) {
		timer++;
		if (timer >= 3) {
			App->render->camera.y += speed * SCREEN_SIZE;
			App->player->position.y += 1;
			timer = 0;
		}

	}
}

update_status ModuleSceneStage3::Update()
{

	App->render->Blit(textures[0], 0, -110, &rect[0], 0.65f);
	App->render->Blit(textures[1], 0, 0, &rect[1]);

	if (App->render->camera.x > 3150 && App->render->camera.x < 3874)
	{
		if (!up) up = true;
	}
	else if (up) up = false;

	UpdateCamera();

	return update_status::UPDATE_CONTINUE;
}

bool ModuleSceneStage3::CleanUp()
{
	App->player->Disable();
	App->collision->Disable();
	App->powerups->Disable();
	App->particles->Disable();

	for (int i = 0; i < 3; ++i)
	{
		App->textures->Unload(textures[i]);
		textures[i] = nullptr;
	}

	App->audio->UnloadMusic(music);
	music = nullptr;

	return true;
}
