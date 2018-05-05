#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleUI.h"
#include "ModuleSceneStage1.h"
#include "ModuleFonts.h"
#include "ModuleSceneContinue.h"
#include "ModuleFadeToBlack.h"
#include "SDL_mixer/include/SDL_mixer.h"
#include "SDL/include/SDL_render.h"
#include "SDL/include/SDL_timer.h"
#include "ModuleSceneStart.h"

update_status ModuleSceneStart::Update()
{
	if (App->input->keyboard[SDL_SCANCODE_1] == 1 && App->ui->coins > 0) App->fade->FadeToBlack(this, App->stage1, 2);
	if (App->input->keyboard[SDL_SCANCODE_5] == 1) App->ui->InsertCoin();
	if (App->input->keyboard[SDL_SCANCODE_L] == 1) App->fade->FadeToBlack(this, App->scene_continue);

	if (App->ui->coins > 0 && current_step == start_steps::logo_movement) {
		Mix_PauseMusic();
		current_step = start_steps::show_coins;
	}

	switch (current_step) {
		case start_steps::logo_movement: {
			App->render->Blit(graphics, 0, 0, &background);
			App->render->Blit(graphics, 0, 0, &title);
			App->render->Blit(graphics, 116, 200, &company);
		} break;
		case start_steps::show_coins: {
			alpha+=10;
			if (alpha >= 255) {
				alpha = 255;
				current_step = start_steps::hold;
				total_time = (Uint32)(1.5f * 1000.0f);
				start_time = SDL_GetTicks();
			}
			App->ui->PlayerCoins(font_color);

		} break;
		case start_steps::hide_coins: {
			alpha-=10;
			if (alpha <= 1) {
				alpha = 1;
				font_color = !font_color;
				current_step = start_steps::hold;
				total_time = (Uint32)(0.3f * 1000.0f);
				start_time = SDL_GetTicks();
			}
			App->ui->PlayerCoins(font_color);
		}break;
		case start_steps::hold: {
			Uint32 now = SDL_GetTicks() - start_time;

			if (now >= total_time) {
				if (alpha == 255) current_step = start_steps::hide_coins;
				else current_step = start_steps::show_coins;
			}

			App->ui->PlayerCoins(font_color);
		}break;
									  
	}
	SDL_SetRenderDrawColor(App->render->renderer, 0, 0, 0, (Uint8)(alpha * 255));
	SDL_RenderFillRect(App->render->renderer, &App->ui->screen);

	return update_status::UPDATE_CONTINUE;
}


bool ModuleSceneStart::CleanUp()
{
	App->textures->Unload(graphics);
	graphics = nullptr;

	App->audio->UnloadMusic(music);
	music = nullptr;

	return true;
}

bool ModuleSceneStart::Start()
{
	App->render->ResetCamera();
	App->ui->ui_visible = false;
	graphics = App->textures->Load("Assets/Sprites/UI/UI_2.png");
	music = App->audio->LoadMusic("Assets/Audio/Music/01_X-Multiply_Title.ogg");
	Mix_PlayMusic(music, -1);

	return true;
}

ModuleSceneStart::ModuleSceneStart()
{
	title = {0,13,384,51};
	background = {61,240,384,256};
	company = {354,71,152,38};
}

ModuleSceneStart::~ModuleSceneStart()
{
}

void ModuleSceneStart::OnFade() {
	App->ui->Reset();
}
