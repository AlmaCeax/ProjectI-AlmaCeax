#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleUI.h"
#include "ModuleFonts.h"
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
	//strcpy_s(current_sequence, sequence_9to8);
	if (App->input->keyboard[SDL_SCANCODE_O] == 1) numberswap = true;

	if (numberswap) {
		if (current_sequence[sequence_iterator] > 0) {
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
		}
	}

	BlitNumber();
	return update_status::UPDATE_CONTINUE;
}

bool ModuleSceneContinue::CleanUp()
{
	return true;
}

bool ModuleSceneContinue::Start()
{
	App->render->ResetCamera();
	graphics = App->textures->Load("Assets/Sprites/UI/UI_2.png");
	ResetNumber();
	//music = App->audio->LoadMusic("Assets/Audio/Music/01_X-Multiply_Title.ogg");
	//Mix_PlayMusic(music, -1);

	return true;
}

void ModuleSceneContinue::ResetNumber() {
	for (int row = 0; row < 7; row++) {
		for (int line = 0; line < 7; line++) {
			number_display[row][line] = false;
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


