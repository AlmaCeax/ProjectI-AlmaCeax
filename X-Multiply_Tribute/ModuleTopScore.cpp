#include "Application.h"
#include "ModuleUI.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleFonts.h"
#include "ModuleFadeToBlack.h"
#include "ModuleSceneContinue.h"
#include "Module.h";
#include "ModuleAudio.h"
#include "ModuleInput.h"
#include "ModuleTopScore.h"
#include "SDL_mixer/include/SDL_mixer.h"
#include <stdlib.h>
#include <string.h>



ModuleTopScore::ModuleTopScore()
{
}


ModuleTopScore::~ModuleTopScore()
{
}

bool ModuleTopScore::Init() {
	title_rect = { 18,78,304,94 };
	return true;
}
bool ModuleTopScore::Start()
{
	App->ui->ui_visible = false;
	App->ui->game_over = false;
	App->render->ResetCamera();
	doing_input = false;

	bool save = false;
	for (int i = 4; i >= 0; i--) {
		if (App->ui->score >= App->ui->top_5_scores[i]) {
			save = true;
			doing_input = true;
			if (i == 0) {
				player_id = 0;
				SaveScore();
			}
		}
		else {
			if (save) {
				player_id = i + 1;
				SaveScore();
				i = 0;
			}
			else {
				player_id = -1;
				i = 0;
			}
		}
	}
	
	for (int i = 0; i < 5; i++) {
		switch (i) {
		case 0:
			_itoa_s(App->ui->top_5_scores[i], score_1, 10);
			break;
		case 1:
			_itoa_s(App->ui->top_5_scores[i], score_2, 10);
			break;
		case 2:
			_itoa_s(App->ui->top_5_scores[i], score_3, 10);
			break;
		case 3:
			_itoa_s(App->ui->top_5_scores[i], score_4, 10);
			break;
		case 4:
			_itoa_s(App->ui->top_5_scores[i], score_5, 10);
			break;
		}
	}

	title = App->textures->Load("Assets/Sprites/UI/UI_2.png");
	music = App->audio->LoadMusic("Assets/Audio/Music/13_Body_Scratchers_Name_Entry_.ogg");
	bckg = App->textures->Load("Assets/Sprites/UI/backgroundTopScore.png");
	Mix_PlayMusic(music, 0);

	letter_swap = App->audio->LoadFx("Assets/Audio/SFX/xmultipl-082.wav");
	letter_input = App->audio->LoadFx("Assets/Audio/SFX/xmultipl-051.wav");

	return true;
}

update_status ModuleTopScore::Update()
{
	App->render->Blit(bckg, 0, 0, nullptr);
	App->render->Blit(title, 45, 6, &title_rect);

	for (int i = 0; i < 5; i++) {
		switch (i) {
		case 0:
			if (player_id == i) {
				App->fonts->BlitText(28, 120, App->ui->pink_font, "1st");
				App->fonts->BlitText(92, 120, App->ui->pink_font, score_1);
				App->fonts->BlitText(208, 120, App->ui->pink_font, player_input);
			}
			else {
				App->fonts->BlitText(28, 120, App->ui->blue_font, "1st");
				App->fonts->BlitText(92, 120, App->ui->blue_font, score_1);
				App->fonts->BlitText(208, 120, App->ui->blue_font, App->ui->names[i]);
			}
			break;
		case 1:
			if (player_id == i) {
				App->fonts->BlitText(28, 145, App->ui->pink_font, "2nd");
				App->fonts->BlitText(92, 145, App->ui->pink_font, score_2);
				App->fonts->BlitText(208, 145, App->ui->pink_font, player_input);
			}
			else {
				App->fonts->BlitText(28, 145, App->ui->blue_font, "2nd");
				App->fonts->BlitText(92, 145, App->ui->blue_font, score_2);
				App->fonts->BlitText(208, 145, App->ui->blue_font, App->ui->names[i]);
			}
			break;
		case 2:
			if (player_id == i) {
				App->fonts->BlitText(28, 170, App->ui->pink_font, "3rd");
				App->fonts->BlitText(92, 170, App->ui->pink_font, score_3);
				App->fonts->BlitText(208, 170, App->ui->pink_font, player_input);
			}
			else {
				App->fonts->BlitText(28, 170, App->ui->blue_font, "3rd");
				App->fonts->BlitText(92, 170, App->ui->blue_font, score_3);
				App->fonts->BlitText(208, 170, App->ui->blue_font, App->ui->names[i]);
			}
			break;
		case 3:
			if (player_id == i) {
				App->fonts->BlitText(28, 195, App->ui->pink_font, "4th");
				App->fonts->BlitText(92, 195, App->ui->pink_font, score_4);
				App->fonts->BlitText(208, 195, App->ui->pink_font, player_input);
			}
			else {
				App->fonts->BlitText(28, 195, App->ui->blue_font, "4th");
				App->fonts->BlitText(92, 195, App->ui->blue_font, score_4);
				App->fonts->BlitText(208, 195, App->ui->blue_font, App->ui->names[i]);
			}
			break;
		case 4:
			if (player_id == i) {
				App->fonts->BlitText(28, 220, App->ui->pink_font, "5th");
				App->fonts->BlitText(92, 220, App->ui->pink_font, score_5);
				App->fonts->BlitText(208, 220, App->ui->pink_font, player_input);
			}
			else {
				App->fonts->BlitText(28, 220, App->ui->blue_font, "5th");
				App->fonts->BlitText(92, 220, App->ui->blue_font, score_5);
				App->fonts->BlitText(208, 220, App->ui->blue_font, App->ui->names[i]);
			}
			break;
		}
	}

	if(doing_input) NameInput();
	else {
		if (App->input->keyboard[SDL_SCANCODE_1] == KEY_DOWN || App->input->controller_START_button == KEY_STATE::KEY_DOWN) App->fade->FadeToBlack(this, App->scene_continue);
	}

	return update_status::UPDATE_CONTINUE;
}

bool ModuleTopScore::CleanUp()
{
	App->textures->Unload(title);
	title = nullptr;
	App->audio->UnloadMusic(music);
	music = nullptr;
	App->audio->UnloadSFX(letter_swap);
	letter_swap = nullptr;
	App->audio->UnloadSFX(letter_input);
	letter_input = nullptr;
	App->textures->Unload(bckg);
	bckg = nullptr;

	player_id = 0;
	memset(player_input, ' ', 9);
	player_input[0] = 'a';
	doing_input = false;
	current_char = 0;
	char_spot = 0;

	return true;
}


void ModuleTopScore::SaveScore() {
	int last_score;
	char last_name[10];
	for (int i = player_id; i < 5; i++) {
		if (i == player_id) {
			last_score = App->ui->top_5_scores[player_id];
			strcpy_s(last_name, App->ui->names[player_id]);
			App->ui->top_5_scores[player_id] = App->ui->score;
		}
		else {
			if (i == 4) {
				App->ui->top_5_scores[i] = last_score;
				strcpy_s(App->ui->names[i], last_name);
			}
			else {
				int temp_score = App->ui->top_5_scores[i];
				char temp_name[10];
				strcpy_s(temp_name, App->ui->names[i]);
				App->ui->top_5_scores[i] = last_score;
				strcpy_s(App->ui->names[i], last_name);
				last_score = temp_score;
				strcpy_s(last_name, temp_name);
			}
		}
	}
}

void ModuleTopScore::NameInput() {
	if (App->input->keyboard[SDL_SCANCODE_W] == KEY_DOWN || (SDL_GameControllerGetAxis(App->input->controller, SDL_CONTROLLER_AXIS_LEFTY) < -CONTROLLER_DEAD_ZONE)) {
		current_char++;
		if (current_char == 39) current_char = 0;
		player_input[char_spot] = possible_inputs[current_char];
		Mix_PlayChannel(-1, letter_swap, 0);
	}
	else if (App->input->keyboard[SDL_SCANCODE_S] == KEY_DOWN || (SDL_GameControllerGetAxis(App->input->controller, SDL_CONTROLLER_AXIS_LEFTY) > CONTROLLER_DEAD_ZONE)) {
		current_char--;
		if (current_char == -1) current_char = 38;
		player_input[char_spot] = possible_inputs[current_char];
		Mix_PlayChannel(-1, letter_swap, 0);
	}

	if (App->input->keyboard[SDL_SCANCODE_5] == KEY_DOWN || App->input->controller_Y_button == KEY_STATE::KEY_DOWN) {
		char_spot++;
		if (char_spot == 9) {
			doing_input = false;
			strcpy_s(App->ui->names[player_id], player_input);
		}
		else {
			player_input[char_spot] = possible_inputs[current_char];
		}
		Mix_PlayChannel(-1, letter_input, 0);
	}
	if (App->input->keyboard[SDL_SCANCODE_1] == KEY_DOWN || App->input->controller_START_button == KEY_STATE::KEY_DOWN) {
		Mix_PlayChannel(-1, letter_input, 0);
		strcpy_s(App->ui->names[player_id], player_input);
		doing_input = false;
	}
}