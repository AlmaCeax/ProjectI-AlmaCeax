#ifndef __ModuleTopScore_H__
#define __ModuleTopScore_H__

#include "Module.h"

class ModuleTopScore :
	public Module
{
public:
	ModuleTopScore();
	~ModuleTopScore();

	bool Start();

	bool Init();
	update_status Update();
	bool CleanUp();


private :
	void SaveScore();
	void NameInput();

	int timer = 0;
	int player_id;
	char player_input[10] = "a";
	char score_1[9];
	char score_2[9];
	char score_3[9];
	char score_4[9];
	char score_5[9];
	SDL_Texture* title;
	SDL_Texture* bckg;
	SDL_Rect title_rect;
	_Mix_Music* music;
	Mix_Chunk* letter_swap;
	Mix_Chunk* letter_input;

	char possible_inputs[40] = "abcdefghijklmnopqrstuvwxyz0123456789'. ";
	int current_char = 0;
	int char_spot = 0;
	bool doing_input;
};

#endif