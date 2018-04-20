#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleFonts.h"
#include "ModuleRender.h"
#include "ModuleSceneStart.h"
#include "ModuleFadeToBlack.h"
#include "ModulePlayer.h"
#include "ModuleCollision.h"
#include "ModuleAudio.h"
#include "ModuleSceneStage1.h"
#include "SDL/include/SDL_render.h"
#include "SDL/include/SDL_timer.h"
#include "ModuleUI.h"
#include "SDL_mixer/include/SDL_mixer.h"

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

	clear_song = App->audio->LoadMusic("Assets/Audio/Music/04_Stage_Clear.ogg");
	ready_song = App->audio->LoadMusic("Assets/Audio/Music/14_Player_Ready.ogg");

	ui_rect = { 0,0,384,32 };
	life = { 392,0,8,16 };
	screen = { 0, 0, SCREEN_WIDTH * SCREEN_SIZE, SCREEN_HEIGHT * SCREEN_SIZE };

	return true;
}

update_status ModuleUI::Update()
 {
	if (ui_visible) { 
		if (App->collision->god) App->fonts->BlitText(0, 0, score_font, "godmode");
		App->render->Blit(graphics, 0, 224, &ui_rect, false); 
		App->fonts->BlitText(30, 240, score_font, "1up");
		App->fonts->BlitText(62, 240, score_font, score_text);
		App->fonts->BlitText(144, 240, score_font, "top");
		App->fonts->BlitText(176, 240, score_font, "00500000");
		App->fonts->BlitText(258, 240, score_font, "2up");
		App->fonts->BlitText(290, 240, score_font, "00000000");

		for (int i = 0; i < player_lives; i++) {
			App->render->Blit(graphics, 62 + (life.w*i), 224, &life, false);
		}
	}	
	
	
	if (current_ready_step != ready_step::not) ReadyUpdate();
	
	if(current_step == clear_step::none) return update_status::UPDATE_CONTINUE;
	ClearUpdate();

	
	return update_status::UPDATE_CONTINUE;
}

void ModuleUI::StageCleared() {
	Mix_PlayMusic(clear_song, false);
	if (!App->collision->god) App->collision->GodMode();
	total_time = (Uint32)(4.0f * 0.5f * 1000.0f);
	start_time = SDL_GetTicks();
	ui_visible = false;
	current_step = clear_step::player_moving;
}

void ModuleUI::ReadyUpdate() {

	Uint32 now = SDL_GetTicks() - start_time;

	if (now >= 500 && now < 1000) current_ready_step = ready_step::dontshow_text;
	else current_ready_step = ready_step::show_text;
	if (now >= 1500 && now < 2000) current_ready_step = ready_step::dontshow_text;
	else current_ready_step = ready_step::show_text;

	if (current_ready_step == ready_step::show_text) App->fonts->BlitText(80, 85, pink_font, "player 1 ready");
	if (now >= total_time) ReadyDone();
}

void ModuleUI::ClearUpdate() {

	Uint32 now = SDL_GetTicks() - start_time;
	float normalized = MIN(1.0f, (float)now / (float)total_time);

	if (current_step == clear_step::player_moving) {

		if (now >= total_time) {
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
			if (letter >= 14) letter = 0;
		}
		else if (current_text2[16] != final_text2[16]) {
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
}

bool ModuleUI::CleanUp()
{
	return true;
}

void ModuleUI::AddScore(uint points) {
	score += points;

	char str[8];
	_itoa_s(score, str, 10);
	int new_position = 7;
	int total = 0;

	for (int i = 0; i < 7; i++) {

		if (str[i] == NULL) break;
		total++;
	}

	for (int i = total-1; i >= 0; i--) {
		score_text[new_position] = str[i];
		new_position--;
	}
}

void ModuleUI::PlayerDeath() {
	player_lives--;
	PlayerReady();
}

void ModuleUI::Reset() {
	player_lives = 2;
	score = 0;
	memset(score_text, '0', 8);
}

void ModuleUI::DeathFade() {
	if (player_lives > 0) App->fade->FadeToBlack(App->current_scene, App->current_scene);
	else { 
		App->fade->FadeToBlack(App->current_scene, App->start);
		App->player->dead = false;
	}
}

void ModuleUI::PlayerReady() {
	Mix_PlayMusic(ready_song, false);
	App->stage1->first_time = false;
	total_time = (Uint32)(3.0f * 1000.0f);
	start_time = SDL_GetTicks();
	current_ready_step = ready_step::show_text;
}

void ModuleUI::ReadyDone() {
	App->player->Enable();
	App->player->position.x = -36;
	App->player->injecting = false;
	App->player->startBoost = true;
	App->stage1->right = true;
	current_ready_step = ready_step::not;
}
