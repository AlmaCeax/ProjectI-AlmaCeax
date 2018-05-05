#ifndef __ModuleSceneContinue_H__
#define __ModuleSceneContinue_H__

#include "Module.h"

#define MAX_SEQUENCE 30
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

private:
	bool number_display[7][7];
	//int current_sequence[MAX_SEQUENCE];
	void BlitNumber();
	void ResetNumber();


	SDL_Texture* graphics;
	_Mix_Music* music;
	SDL_Rect ball_rect;

	int current_sequence[MAX_SEQUENCE] = { 40,05,04,03,02,01,10,20,31,32,33,34,35,36,05,04,03,02,01,10,20,31,32,33,34,35,-1 };
	int sequence_iterator;
	bool numberswap = false;
	/*	0123456
	
	0	0111110
	1	1000001
	2	1000001
	3	0111111
	4	0000001
	5	1000001
	6	0111110

	*/
};

#endif
