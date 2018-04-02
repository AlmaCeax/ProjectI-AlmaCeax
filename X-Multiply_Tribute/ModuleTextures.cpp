#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"

#include "SDL/include/SDL.h"
#include "SDL_image/include/SDL_image.h"
#pragma comment( lib, "SDL_image/libx86/SDL2_image.lib" )

ModuleTextures::ModuleTextures() : Module()
{
	for (int i = 0; i < MAX_TEXTURES; i++) {
		textures[i] = nullptr;
	}
}

// Destructor
ModuleTextures::~ModuleTextures()
{}

// Called before render is available
bool ModuleTextures::Init()
{
	LOG("Init Image library");
	bool ret = true;

	// load support for the PNG image format
	int flags = IMG_INIT_PNG;
	int init = IMG_Init(flags);

	if((init & flags) != flags)
	{
		LOG("Could not initialize Image lib. IMG_Init: %s", IMG_GetError());
		ret = false;
	}

	return ret;
}

// Called before quitting
bool ModuleTextures::CleanUp()
{
	LOG("Freeing textures and Image library");

	for (int i = 0; i < MAX_TEXTURES; i++) {
		SDL_DestroyTexture(textures[i]);
	}
	IMG_Quit();
	return true;
}

// Load new texture from file path
SDL_Texture* const ModuleTextures::Load(const char* path)
{
	surface = IMG_Load(path);
	if (!surface) {
		LOG("IMG_Load: %s\n", IMG_GetError());
		// handle error
	}

	SDL_Texture *texture = SDL_CreateTextureFromSurface(App->render->renderer, surface);
	if (!texture) {
		LOG("CreateTextureFromSurface failed: %s\n", SDL_GetError());
	}
	else {
		
		if (textures[MAX_TEXTURES] != nullptr) {
			LOG("Textures aren't empty");
		}
		else {
			for (int i = 0; i < MAX_TEXTURES; i++) {
				if (textures[i] == nullptr) {
					textures[i] = texture;
				}
			}
		}
		
		
	}

	SDL_FreeSurface(surface);

	return texture;
}
