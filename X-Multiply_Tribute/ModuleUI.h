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

	bool CleanUp();

	void AddScore(uint points);

	void PlayerDeath();
	void Reset();
	void DeathFade();
	void PlayerReady();
	void ReadyDone();

	bool ui_visible = false;

private:
	SDL_Texture* graphics;
	SDL_Rect ui_rect;
	SDL_Rect screen;
	SDL_Rect life;
	_Mix_Music* clear_song;
	_Mix_Music* ready_song;

	void ClearUpdate();
	void ReadyUpdate();
	int player_lives = 2;
	uint score = 0;
	char score_text[9] = "00000000";

	int score_font = -1;
	int blue_font = -1;
	int pink_font = -1;

	float alpha = 0;
	Uint32 start_time = 0;
	Uint32 total_time = 0;

	char current_text2[18] = "";
	char current_text1[16] = "";
	const char final_text1[16] = "stage 1 cleared";
	const char final_text2[18] = "stage bonus 10000";
	float letter = 0;

	enum ready_step {
		not,
		show_text,
		dontshow_text
	} current_ready_step = ready_step::not;

	enum clear_step
	{
		none,
		player_moving,
		player_stopped
	} current_step = clear_step::none;
};

#endif