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
	over_song = App->audio->LoadMusic("Assets/Audio/Music/15_Game_Over.ogg");

	ui_rect = { 0,0,384,32 };
	life = { 392,0,8,16 };
	screen = { 0, 0, SCREEN_WIDTH * SCREEN_SIZE, SCREEN_HEIGHT * SCREEN_SIZE };

	current_step = clear_step::none;
	current_ready_step = ready_step::not;

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
		if(game_over) App->fonts->BlitText(120, 100, pink_font, "game over");

		for (int i = 0; i < player_lives; i++) {
			App->render->Blit(graphics, 62 + (life.w*i), 224, &life, false);
		}
	}	
	
	
	if (current_ready_step != ready_step::not) ReadyUpdate();
	if (game_over) {
		Uint32 now = SDL_GetTicks() - start_time;
		if (now >= total_time) {
			App->fade->FadeToBlack(App->current_scene, App->start);
		}
	}
	
	if(current_step == clear_step::none) return update_status::UPDATE_CONTINUE;
	ClearUpdate();

	
	return update_status::UPDATE_CONTINUE;
}

void ModuleUI::ReadyUpdate() {

	Uint32 now = SDL_GetTicks() - start_time;

	if (now >= 800 && now <= 1200) current_ready_step = ready_step::dontshow_text;
	else current_ready_step = ready_step::show_text;

	if (current_ready_step == ready_step::show_text) App->fonts->BlitText(80, 85, pink_font, "player 1 ready");
	if (now >= total_time) ReadyDone();
}

void ModuleUI::ClearUpdate() {

	Uint32 now = SDL_GetTicks() - start_time;
	float normalized = MIN(1.0f, (float)now / (float)total_time);
	if (App->player->collider != nullptr) App->player->collider->to_delete = true; // this souldnt be here

	switch (current_step) {
		case clear_step::fade: {
			if (now >= total_time) {
				App->player->canMove = false;
				clear_position = { (float)((App->render->camera.x + (SCREEN_WIDTH*SCREEN_SIZE / 2))/SCREEN_SIZE)-18, (float)(App->render->camera.y/SCREEN_SIZE)+50 };
				origin_position = { App->player->position.x, App->player->position.y };
				distance = origin_position.DistanceTo(clear_position);
				direction = { clear_position.x / distance - origin_position.x / distance, clear_position.y / distance - origin_position.y / distance };

				current_step = clear_step::player_moving;
				start_time = SDL_GetTicks();
				total_time = (Uint32)(9.0f * 1000.0f);
			}
		}break;

		case clear_step::player_moving: {
			normalized = 1.0;

			App->player->position.x += direction.x;
			App->player->position.y += direction.y;

			if (origin_position.DistanceTo(App->player->position) >= distance)
			{
				App->player->position = clear_position;
				current_step = clear_step::player_stopped;
			}
		
		}break;

		case clear_step::player_stopped: {
			normalized = 1.0;
			if (current_text1[14] != final_text1[14] && timer >= 1)
			{
				LOG("text 1 letter is = %f", letter);
				current_text1[letter] = final_text1[letter];
				letter++;
				timer = 0;
				if (letter > 14) letter = 0;
			}
			else if (current_text2[16] != final_text2[16] && timer>= 1) {
				LOG("text 2 letter is = %f", letter);
				current_text2[letter] = final_text2[letter];
				letter++;
				timer = 0;
				if (letter > 16) letter = 0;
			}
			timer += 0.2f;

			if (now >= total_time) {
				timer = 0;
				memset(current_text1, 0, sizeof(current_text1));
				memset(current_text2, 0, sizeof(current_text2));
				App->fade->FadeToBlack(App->current_scene, App->start, 0.0f);
				current_step = clear_step::none;
			}
		}break;
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
	App->player->dead = false;
	game_over = false;
	player_lives = 2;
	score = 0;
	memset(score_text, '0', 8);
}

void ModuleUI::DeathFade() {
	if (player_lives > 0) App->fade->FadeToBlack(App->current_scene, App->current_scene);
	else if(!game_over) { 
		Mix_PlayMusic(over_song,false);
		total_time = (Uint32)(4.0f * 1000.0f);
		start_time = SDL_GetTicks();
		game_over = true;
	}
}

void ModuleUI::PlayerReady() {
	Mix_PlayMusic(ready_song, false);
	App->stage1->first_time = false;
	total_time = (Uint32)(3.5f * 1000.0f);
	start_time = SDL_GetTicks();
	current_ready_step = ready_step::show_text;
}

void ModuleUI::ReadyDone() {
	App->player->Enable();
	App->player->position.x = -36;
	App->player->injecting = false;
	App->player->startBoost = true;
	App->stage1->right = true;
	Mix_PlayMusic(App->stage1->music, true);
	current_ready_step = ready_step::not;
}

void ModuleUI::StageCleared() {
	if (current_step != clear_step::none || current_ready_step != ready_step::not || App->player->dead) return;

	Mix_PlayMusic(clear_song, false);
	//if(App->player->collider != nullptr) App->player->collider->to_delete = true;
	App->stage1->right = false;
	total_time = (Uint32)(4.0f * 1000.0f);
	start_time = SDL_GetTicks();
	current_step = clear_step::fade;
}