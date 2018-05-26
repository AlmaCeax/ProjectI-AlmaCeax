#ifndef __MODULESTAGE_H__
#define __MODULESTAGE_H__
#include "Module.h"
#include "ModuleUI.h"
#include "ModulePlayer.h"

#define MAX_CHECKPOINTS 8

class ModuleStage: public Module
{
public:
	int index, score_bonus;
	bool right,up,down,left,stopped,wallboss;
	_Mix_Music* music;
	_Mix_Music* bossTrack;
	void OnFade(){ if (App->player->dead) App->ui->PlayerDeath(); }
	iPoint checkpoints[MAX_CHECKPOINTS];

	void CheckpointsUpdate()
	{
		for (int i = 0; i < MAX_CHECKPOINTS; i++) {
			if (checkpoints[i+1].x != -1) {
				if (App->player->position.x >= checkpoints[i].x && App->player->position.x < checkpoints[i + 1].x) {
					App->ui->current_checkpoint = i;
				}
			}
			else {
				if (App->player->position.x >= checkpoints[i].x) App->ui->current_checkpoint = i;
				return;
			}
		}
	}
};

#endif 
