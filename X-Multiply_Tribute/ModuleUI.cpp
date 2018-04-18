#include "Globals.h"
#include "Application.h"
#include "ModuleFonts.h"
#include "ModuleUI.h"



ModuleUI::ModuleUI()
{
}


ModuleUI::~ModuleUI()
{
}

bool ModuleUI::Init()
{
	score_font = App->fonts->Load("Assets/Sprites/UI/score_font.png", "0123456789$.-=^^()^^^!^^^?abcdefghijklmnopqrstuvwxyz", 2);
	blue_font = App->fonts->Load("Assets/Sprites/UI/blue_font.png", "0123456789$.-=            abcdefghijklmnopqrstuvwxyz", 2);
	pink_font = App->fonts->Load("Assets/Sprites/UI/pink_font.png", "0123456789$.-=            abcdefghijklmnopqrstuvwxyz", 2);
	return true;
}

update_status ModuleUI::Update()
{
	return update_status::UPDATE_CONTINUE;
}

bool ModuleUI::CleanUp()
{
	return true;
}
