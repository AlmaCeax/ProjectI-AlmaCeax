#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleUI.h"
#include "ModuleStage.h"
#include "ModuleFonts.h"
#include "ModuleSceneStart.h"
#include "ModuleFadeToBlack.h"
#include "SDL_mixer/include/SDL_mixer.h"
#include "SDL/include/SDL_render.h"
#include "SDL/include/SDL_timer.h"
#include "ModuleSceneContinue.h"
#include <stdlib.h>

ModuleSceneContinue::ModuleSceneContinue()
{
}


ModuleSceneContinue::~ModuleSceneContinue()
{
}

bool ModuleSceneContinue::Init()
{
	ball_rect = {26,198,BALL_WIDTH,BALL_HEIGHT};
	return true;
}

update_status ModuleSceneContinue::Update()
{
	if (App->input->keyboard[SDL_SCANCODE_5] == KEY_DOWN) App->ui->InsertCoin();
	if (App->input->keyboard[SDL_SCANCODE_1] == KEY_DOWN && App->ui->coins > 0) {
		App->fade->FadeToBlack(this, App->current_stage);
		App->ui->is_continue = true;
	}

	App->ui->ContinueTextBlit();
	NumberSwap();
	BlitNumber();

	return update_status::UPDATE_CONTINUE;
}

bool ModuleSceneContinue::CleanUp()
{
	SDL_DestroyTexture(graphics);
	graphics = nullptr;

	App->audio->UnloadMusic(music);
	music = nullptr;

	return true;
}

void ModuleSceneContinue::NumberSwap() {
	Uint32 now = SDL_GetTicks() - number_start_time;

	if (numberswap) {
		if (current_sequence[sequence_iterator] >= 0) {
			char test[3];

			if (current_sequence[sequence_iterator] < 10) {
				test[0] = 0 + '0';
				test[1] = current_sequence[sequence_iterator] + '0';
			}
			else _itoa_s(current_sequence[sequence_iterator], test, 10);

			number_display[test[0] - '0'][test[1] - '0'] = !number_display[test[0] - '0'][test[1] - '0'];
			sequence_iterator++;
		}
		else {
			numberswap = false;
			current_number--;
			sequence_iterator = 0;
		}
	}

	if (now >= number_total_time) {

		if (current_number < 0) {
			App->fade->FadeToBlack(this, App->start, 1.0f);
		}
		else {
			for (int i = 0; i < MAX_SEQUENCE; i++) {
				current_sequence[i] = sequences[current_number][i];
			}

			number_total_time = (Uint32)(1.0f * 1000.0f);
			number_start_time = SDL_GetTicks();
			numberswap = true;
		}


	}


	//continue text fade
	now = SDL_GetTicks() - fade_start_time;

	if (!hold) {
		if (fading) {
			alpha += 10;
			if (alpha >= 255) {
				fading = false;
				alpha = 255;
			}
		}
		else {
			alpha -= 10;
			if (alpha <= 1) {
				hold = true;
				alpha = 0;
				fade_total_time = (Uint32)(1.5f * 1000.0f);
				fade_start_time = SDL_GetTicks();
			}
		}
	}
	else {
		if (now >= fade_total_time) {
			fading = true;
			hold = false;
		}
	}

	SDL_SetRenderDrawColor(App->render->renderer, 0, 0, 0, (Uint8)(alpha));
	SDL_RenderFillRect(App->render->renderer, &fade_rect);

}
bool ModuleSceneContinue::Start()
{
	App->ui->ui_visible = false;

	App->render->ResetCamera();
	graphics = App->textures->Load("Assets/Sprites/UI/UI_2.png");
	music = App->audio->LoadMusic("Assets/Audio/Music/16_Continue.ogg");
	Mix_PlayMusic(music, 0);
	
	ResetNumber();
	alpha = 255;
	fading = false;
	current_number = 8;
	sequence_iterator = 0;
	numberswap = false;

	number_total_time = (Uint32)(1.0f * 1000.0f);
	number_start_time = SDL_GetTicks();

	return true;
}
void ModuleSceneContinue::BlitNumber()
{
	int x, y;
	for (int line = 0; line < 7; line++) {
		for (int row = 0; row < 7; row++) {
			if (number_display[line][row]) {
				x = 145; y = 85;
				x += row * BALL_WIDTH;
				y += line * BALL_HEIGHT;
				App->render->Blit(graphics, x, y, &ball_rect);
			}
		}
	}
}

void ModuleSceneContinue::ResetNumber() {
	for (int line = 0; line < 7; line++) {
		for (int row = 0; row < 7; row++) {
			number_display[line][row] = false;
		}
	}

	number_display[0][1] = true;
	number_display[0][2] = true;
	number_display[0][3] = true;
	number_display[0][4] = true;
	number_display[0][5] = true;
	number_display[1][0] = true;
	number_display[1][6] = true;
	number_display[2][0] = true;
	number_display[2][6] = true;
	number_display[3][1] = true;
	number_display[3][2] = true;
	number_display[3][3] = true;
	number_display[3][4] = true;
	number_display[3][5] = true;
	number_display[3][6] = true;
	number_display[4][6] = true;
	number_display[5][0] = true;
	number_display[5][6] = true;
	number_display[6][1] = true;
	number_display[6][2] = true;
	number_display[6][3] = true;
	number_display[6][4] = true;
	number_display[6][5] = true;
}

void ModuleSceneContinue::OnFade() {
	App->ui->Reset();
}



