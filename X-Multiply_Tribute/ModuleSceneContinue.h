#ifndef __ModuleSceneContinue_H__
#define __ModuleSceneContinue_H__

#include "Module.h"

#define MAX_SEQUENCE 35
#define BALL_WIDTH 16
#define BALL_HEIGHT 14
struct _Mix_Music;

class ModuleSceneContinue :
	public Module
{
public:
	ModuleSceneContinue();
	~ModuleSceneContinue();

	bool Init();
	update_status Update();
	bool CleanUp();
	bool Start();
	void OnFade();

private:
	bool number_display[7][7];
	int current_sequence[MAX_SEQUENCE];
	void BlitNumber();
	void ResetNumber();
	void NumberSwap();


	SDL_Texture* graphics;
	_Mix_Music* music;
	SDL_Rect ball_rect;
	SDL_Rect fade_rect = { 0,0, SCREEN_WIDTH * SCREEN_SIZE, SCREEN_HEIGHT / 2 };

	int alpha;
	bool fading, hold;

	Uint32 number_total_time;
	Uint32 number_start_time;

	Uint32 fade_total_time;
	Uint32 fade_start_time;

	int sequences[9][MAX_SEQUENCE] = {
		{ 04,03,10,20,14,24,30,40,34,44,50,61,54,66,56,46,36,26,16,05,04,03,02,01,-1 },
		{ 20,10,01,14,02,24,05,44,54,16,26,35,43,42,51,60,61,-1 },
		{ 10,20,02,10,04,02,16,04,34,16,33,43,46,34,56,65,42,51,64,63,60,63,50,64,65,66,-1 },
		{ 04,13,01,10,40,31,22,02,03,04,50,51,16,52,53,54,26,34,55,56,46,33,15,56,25,45,64,63,62,61,50,-1 },
		{ 00,10,13,20,22,30,32,40,33,34,35,51,52,53,65,64,54,55,46,61,62,63,45,35,25,15,01,02,03,06,65,-1 },
		{ 16,05,04,03,02,01,00,40,50,61,62,63,63,62,61,50,01,02,03,04,05,06,-1 },
		{ 20,10,00,10,20,06,26,40,50,61,62,63,64,65,66,36,35,34,33,32,31,30,-1 },
		{ 40,50,61,62,63,64,65,56,46,00,06,36,46,56,66,31,32,33,34,35,-1 },
		{ 40,05,04,03,02,01,10,20,31,32,33,34,35,36,05,04,03,02,01,10,20,31,32,33,34,35,-1 }
	};

	int sequence_iterator;
	bool numberswap = false;
	int current_number = 8;
};

#endif
