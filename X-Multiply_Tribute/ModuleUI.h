#ifndef __ModuleUI_H__
#define __ModuleUI_H__

#include "Module.h"
#include "p2Point.h"

struct _Mix_Music;
struct SDL_Texture;
struct Mix_Chunk;


class ModuleUI :
	public Module
{
public:
	ModuleUI();
	~ModuleUI();

	bool Init();
	update_status Update();
	void StageCleared();

	void IncreaseLife();

	bool CleanUp();

	void AddScore(uint points);

	void ContinueTextBlit();
	void PlayerDeath();
	void Reset();
	void DeathFade();
	void PlayerReady();
	void ReadyDone();
	void InsertCoin();
	void PlayerCoins(bool pink);

	Uint8 current_checkpoint;
	bool ui_visible = false;
	bool is_continue = false;
	int coins = 0;
	SDL_Rect screen;
	uint score = 0;

	int top_5_scores[5] = { 200000,100000,5000,1337,0 };
	/*char name_1[10] = "cere";
	char name_2[10] = "marc";
	char name_3[10] = "alejandro";
	char name_4[10] = "axel";
	char name_5[10] = "";*/
	char names[5][10] = { "cere","marc","alejandro","axel","" };

	int score_font = -1;
	int blue_font = -1;
	int pink_font = -1;

	bool game_over = false;

	enum clear_step
	{
		none,
		fade,
		player_moving,
		player_stopped
	} current_step = clear_step::none;

private:

	SDL_Rect ui_rect;
	SDL_Rect life;
	_Mix_Music* clear_song;
	_Mix_Music* ready_song;
	_Mix_Music* over_song;
	Mix_Chunk* coin_sfx;
	SDL_Texture* graphics;

	void ClearUpdate();
	void ReadyUpdate();
	int player_lives = 2;

	char score_text[9] = "00000000";
	uint top_score = 200000;
	char top_score_text[9] = "00200000";



	Uint32 start_time = 0;
	Uint32 total_time = 0;
	fPoint clear_position;
	fPoint origin_position;
	fPoint direction;
	float distance;

	char current_text2[18] = "";
	char current_text1[16] = "";
	char final_text1[16] = "stage   cleared";
	char final_text2[18] = "";
	int letter = 0;
	float timer = 0;

	int life_increased = false;

	enum ready_step {
		not,
		show_text,
		dontshow_text
	} current_ready_step = ready_step::not;



};

#endif