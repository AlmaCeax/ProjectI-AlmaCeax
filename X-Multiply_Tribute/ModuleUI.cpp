#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleFonts.h"
#include "ModuleRender.h"
#include "ModuleSceneStart.h"
#include "ModuleFadeToBlack.h"
#include "ModulePlayer.h"
#include "ModuleSceneStage3.h"
#include "ModuleCollision.h"
#include "ModuleTopScore.h"
#include "ModuleAudio.h"
#include "ModuleSceneContinue.h"
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
	blue_font = App->fonts->Load("Assets/Sprites/UI/blue_font.png", "0123456789'. ºººººººººººººabcdefghijklmnopqrstuvwxyz", 2);
	pink_font = App->fonts->Load("Assets/Sprites/UI/pink_font.png", "0123456789'. ºººººººººººººabcdefghijklmnopqrstuvwxyz", 2);
	graphics = App->textures->Load("Assets/Sprites/UI/UI_1.png");

	clear_song = App->audio->LoadMusic("Assets/Audio/Music/04_Stage_Clear.ogg");
	ready_song = App->audio->LoadMusic("Assets/Audio/Music/14_Player_Ready.ogg");
	over_song = App->audio->LoadMusic("Assets/Audio/Music/15_Game_Over.ogg");
	coin_sfx = App->audio->LoadFx("Assets/Audio/SFX/xmultipl-017.wav");
	Mix_VolumeChunk(coin_sfx, 5);

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
		App->fonts->BlitText(176, 240, score_font, top_score_text);
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
			App->fade->FadeToBlack(App->current_stage, App->top_score);
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
	//if (App->player->collider != nullptr) App->player->collider->to_delete = true; // this souldnt be here

	switch (current_step) {
		case clear_step::fade: {
			if (now >= total_time) {
				App->player->canMove = false;

				clear_position = { (float)((App->render->camera.x + (SCREEN_WIDTH*SCREEN_SIZE / 2))/SCREEN_SIZE)-18, (float)(App->render->camera.y/SCREEN_SIZE)+50 };
				origin_position = { App->player->position.x, App->player->position.y };
				distance = origin_position.DistanceTo(clear_position);
				direction = { clear_position.x / distance - origin_position.x / distance, clear_position.y / distance - origin_position.y / distance };


				if (clear_position.y < origin_position.y) App->player->current_animation = &App->player->up;
				if (clear_position.y > origin_position.y) App->player->current_animation = &App->player->down;

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
				App->player->current_animation = &App->player->idle;
				App->player->position = clear_position;
				current_step = clear_step::player_stopped;

				char base_text[19] = "stage bonus ";
				char value[6];
				_itoa_s(App->current_stage->score_bonus, value, 10);
				strcat_s(base_text, value);
				strcpy_s(final_text2, base_text);

				final_text1[6] = App->current_stage->index+'0';
			}
		
		}break;

		case clear_step::player_stopped: {
			normalized = 1.0;
			if (current_text1[14] != final_text1[14] && timer >= 1)
			{
				current_text1[letter] = final_text1[letter];
				letter++;
				timer = 0;
				if (letter > 14) letter = 0;
			}
			else if (current_text2[16] != final_text2[16] && timer>= 1) {
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
				current_checkpoint = 0;
				AddScore(App->current_stage->score_bonus);

				switch (App->current_stage->index) {
					case 1: App->fade->FadeToBlack(App->current_stage, App->stage3, 0.0f); break;
					case 3: App->fade->FadeToBlack(App->current_stage, App->stage1, 0.0f); break;
				}		
				current_step = clear_step::none;
			}
		}break;
	}

	SDL_SetRenderDrawColor(App->render->renderer, 0, 0, 0, (Uint8)(normalized * 255));
	SDL_RenderFillRect(App->render->renderer, &screen);

	App->fonts->BlitText(70, 120, App->ui->blue_font, current_text1);
	App->fonts->BlitText(54, 150, App->ui->blue_font, current_text2);
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

	if (score > top_score) {
		top_score = score;
		strcpy_s(top_score_text, score_text);
	}
}

void ModuleUI::ContinueTextBlit()
{
	App->fonts->BlitText(140, 45, pink_font, "continue");

	if (coins > 0) {
		char credit_text[10] = { "credit " };
		if (coins<10) credit_text[7] = coins + '0';
		else {
			char str[3];
			_itoa_s(coins, str, 10);
			credit_text[7] = str[0];
			credit_text[8] = str[1];
		}
		App->fonts->BlitText(135, 210, pink_font, credit_text);
	}
	else App->fonts->BlitText(40, 210, pink_font, "please deposit coin");
}

void ModuleUI::PlayerDeath() {
	player_lives--;
	//PlayerReady();
}

void ModuleUI::Reset() {
	App->player->dead = false;
	game_over = false;
	player_lives = 2;

	/*if (score > top_score && !App->collision->god_used && !life_increased) {
		top_score = score;
		char str[8];
		_itoa_s(top_score, str, 10);
		int new_position = 7;
		int total = 0;

		for (int i = 0; i < 7; i++) {
			if (str[i] == NULL) break;
			total++;
		}
		for (int i = total - 1; i >= 0; i--) {
			top_score_text[new_position] = str[i];
			new_position--;
		}
	}*/
	life_increased = false;
	App->collision->god_used = false;
	if (App->collision->god)App->collision->GodMode();

	score = 0;
	memset(score_text, '0', 8);
	App->player->ResetPU();
}

void ModuleUI::DeathFade() {
	App->stage1->first_time = false;
	if (player_lives > 0) App->fade->FadeToBlack(App->current_stage, App->current_stage);
	else if(!game_over) {
		Mix_PlayMusic(over_song,false);
		total_time = (Uint32)(4.0f * 1000.0f);
		start_time = SDL_GetTicks();
		game_over = true;
		App->ui->coins--;
	}
}

void ModuleUI::PlayerReady() {
	if (current_ready_step == ready_step::not) {
		Mix_PlayMusic(ready_song, false);
		if (App->current_stage->index == 1) App->stage1->first_time = false;
		is_continue = false;
		App->render->SetCameraPosition(App->current_stage->checkpoints[current_checkpoint].x, App->current_stage->checkpoints[current_checkpoint].y);
		App->player->position = { (float)App->current_stage->checkpoints[current_checkpoint].x - 36, (float)App->current_stage->checkpoints[current_checkpoint].y + ((SCREEN_HEIGHT / 2) - 20) };

		total_time = (Uint32)(3.5f * 1000.0f);
		start_time = SDL_GetTicks();
		current_ready_step = ready_step::show_text;
	}
}

void ModuleUI::ReadyDone() {
	App->player->Enable();

	App->player->injecting = false;
	App->player->startBoost = true;

	App->current_stage->right = true;
	Mix_PlayMusic(App->current_stage->music, true);

	current_ready_step = ready_step::not;
}

void ModuleUI::InsertCoin()
{
	if (coins < 99) {
		coins++;
		Mix_PlayChannel(-1, coin_sfx, 0);
	}
}

void ModuleUI::PlayerCoins(bool pink)
{
	int font;
	if (pink) font = pink_font;
	else font = blue_font;

	char credit_text[10] = { "credit " };
	if (coins<10) credit_text[7] = coins + '0';
	else {
		char str[3];
		_itoa_s(coins, str, 10);
		credit_text[7] = str[0];
		credit_text[8] = str[1];
	}

	App->fonts->BlitText(55, 35, font, "push start button");
	if (coins > 1) App->fonts->BlitText(80, 96, font, "1 or 2 players");
	else App->fonts->BlitText(80, 96, font, "1 player only");
	App->fonts->BlitText(120, 160, font, credit_text);
}

void ModuleUI::StageCleared() {
	if (current_step != clear_step::none || current_ready_step != ready_step::not || App->player->dead) return;

	Mix_PlayMusic(clear_song, false);
	if(App->player->collider != nullptr) App->player->collider->to_delete = true;
	App->stage1->right = false;
	total_time = (Uint32)(4.0f * 1000.0f);
	start_time = SDL_GetTicks();
	current_step = clear_step::fade;
}

void ModuleUI::IncreaseLife() {
	if (player_lives < 8) player_lives++;
	life_increased = true;
}