#ifndef __ModuleUI_H__
#define __ModuleUI_H__

#include "Module.h"
class ModuleUI :
	public Module
{
public:
	ModuleUI();
	~ModuleUI();

	bool Init();
	update_status Update();
	bool CleanUp();

	int score_font = -1;
	int blue_font = -1;
	int pink_font = -1;

	uint score = 0;
	char score_text[10];
};

#endif