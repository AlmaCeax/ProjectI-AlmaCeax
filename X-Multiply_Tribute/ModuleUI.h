#ifndef __ModuleUI_H__
#define __ModuleUI_H__

#include "Module.h"
#include "p2Point.h"

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

	void AddScore(int points);

	bool ui_visible = false;

private:
	SDL_Texture* graphics;
	SDL_Rect ui_rect;
	SDL_Rect screen;

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

	enum clear_step
	{
		none,
		player_moving,
		player_stopped
	} current_step = clear_step::none;
};

#endif