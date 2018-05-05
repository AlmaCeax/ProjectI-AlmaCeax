#ifndef __ModuleUI_H__
#define __ModuleUI_H__

#include "Module.h"
#include "p2Point.h"

struct _Mix_Music;


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

	bool ui_visible = false;
	int coins = 0;
	SDL_Rect screen;
	enum clear_step
	{
		none,
		fade,
		player_moving,
		player_stopped
	} current_step = clear_step::none;

private:
	SDL_Texture* graphics;
	SDL_Rect ui_rect;
	SDL_Rect life;
	_Mix_Music* clear_song;
	_Mix_Music* ready_song;
	_Mix_Music* over_song;
	Mix_Chunk* coin_sfx;

	void ClearUpdate();
	void ReadyUpdate();
	int player_lives = 2;
	uint score = 0;
	char score_text[9] = "00000000";
	uint top_score = 0;
	char top_score_text[9] = "00500000";

	int score_font = -1;
	int blue_font = -1;
	int pink_font = -1;

	bool game_over = false;

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