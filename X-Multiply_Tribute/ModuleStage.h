#ifndef __MODULESTAGE_H__
#define __MODULESTAGE_H__
#include "Module.h"
#include "ModuleUI.h"
#include "ModulePlayer.h"

class ModuleStage: public Module
{
public:
	int index, score_bonus;
	void OnFade(){ if (App->player->dead) App->ui->PlayerDeath(); }
};

#endif 
