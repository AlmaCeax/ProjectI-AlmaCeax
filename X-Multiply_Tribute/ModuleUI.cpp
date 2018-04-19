#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleFonts.h"
#include "ModuleRender.h"
#include "ModuleUI.h"
#include "ModuleSceneStart.h"
#include "ModuleFadeToBlack.h"
#include "ModulePlayer.h"
#include "ModuleCollision.h"
#include "SDL/include/SDL_render.h"
#include "SDL/include/SDL_timer.h"

#include <string.h>
#include <stdlib.h>


ModuleUI::ModuleUI()
{
}


ModuleUI::~ModuleUI()
{
}

bool ModuleUI::Init()
{
 	score_font = App->fonts->Load("Assets/Sprites/UI/score_font.png", "0123456789$.-=^^()^^^!^^^?abcdefghijklmnopqrstuvwxyz", 2);
	blue_font = App->fonts->Load("Assets/Sprites/UI/blue_font.png", "0123456789$.-= ºººººººººººabcdefghijklmnopqrstuvwxyz", 2);
	pink_font = App->fonts->Load("Assets/Sprites/UI/pink_font.png", "0123456789$.-= ºººººººººººabcdefghijklmnopqrstuvwxyz", 2);
	graphics = App->textures->Load("Assets/Sprites/UI/UI_1.png");

	ui_rect = { 0,0,384,32 };
	screen = { 0, 0, SCREEN_WIDTH * SCREEN_SIZE, SCREEN_HEIGHT * SCREEN_SIZE };

	return true;
}

update_status ModuleUI::Update()
 {
	if (ui_visible) { 
		App->render->Blit(graphics, 0, 224, &ui_rect, false); 
		App->fonts->BlitText(30, 240, score_font, "1up");
		App->fonts->BlitText(62, 240, score_font, score_text);
		App->fonts->BlitText(144, 240, score_font, "top");
		App->fonts->BlitText(176, 240, score_font, "00500000");
		App->fonts->BlitText(258, 240, score_font, "2up");
		App->fonts->BlitText(290, 240, score_font, "00000000");
	}



	if(current_step == clear_step::none) return update_status::UPDATE_CONTINUE;

	Uint32 now = SDL_GetTicks() - start_time;
	float normalized = MIN(1.0f, (float)now / (float)total_time);

	if (current_step == clear_step::player_moving) {

		if (now >= total_time){
			current_step = clear_step::player_stopped;
			start_time = SDL_GetTicks();
			total_time = (Uint32)(10.0f * 0.5f * 1000.0f);
		}
	}
	else {
		normalized = 1.0;
		Uint32 now = SDL_GetTicks() - start_time;

		if (current_text1[14] != final_text1[14])
		{
			current_text1[(int)letter] = final_text1[(int)letter];
			if(letter >= 14) letter = 0;
		}
		else if(current_text2[16] != final_text2[16]){
			current_text2[(int)letter] = final_text2[(int)letter];
			if (letter >= 16) letter = 0;
		}
		letter += 0.2f;

		if (now >= total_time) {
			App->collision->GodMode();
			memset(current_text1, 0, sizeof(current_text1));
			memset(current_text2, 0, sizeof(current_text2));
			App->fade->FadeToBlack(App->current_scene, App->start, 0.0f);
			current_step = clear_step::none;
		}
	}
	SDL_SetRenderDrawColor(App->render->renderer, 0, 0, 0, (Uint8)(normalized * 255));
	SDL_RenderFillRect(App->render->renderer, &screen);

	App->fonts->BlitText(70, 120, App->ui->blue_font, current_text1);
	App->fonts->BlitText(52, 150, App->ui->blue_font, current_text2);

	App->player->BlitPlayer();
	
	return update_status::UPDATE_CONTINUE;
}

void ModuleUI::StageCleared() {
	if (!App->collision->god) App->collision->GodMode();
	total_time = (Uint32)(4.0f * 0.5f * 1000.0f);
	start_time = SDL_GetTicks();
	ui_visible = false;
	current_step = clear_step::player_moving;
}

bool ModuleUI::CleanUp()
{
	return true;
}

void ModuleUI::AddScore(int points) {
	score += points;

	char str[8];
	_itoa_s(score, str, 10);

	int new_position = 7;
	for (int i = 7; i >= 0; i--) {
		if (str[i] != 'Ì' && str[i] != '\0') { 
			score_text[new_position] = str[i];
			new_position--;
		}
	}
}
