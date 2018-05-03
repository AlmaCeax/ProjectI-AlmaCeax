#ifndef __MODULESTAGE_H__
#define __MODULESTAGE_H__
#include "Module.h"
#include "ModuleUI.h"
#include "ModulePlayer.h"

class ModuleStage: public Module
{
public:
	int index, score_bonus;
	bool right,up,down,left,stopped;
	_Mix_Music* music;
	void OnFade(){ if (App->player->dead) App->ui->PlayerDeath(); }
};

#endif 
