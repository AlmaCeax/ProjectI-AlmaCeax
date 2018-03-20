#include "Globals.h"
#include "Application.h"
#include "ModuleAudio.h"

#include "SDL/include/SDL.h"
#include "SDL_mixer/include/SDL_mixer.h"
#pragma comment( lib, "SDL_mixer/libx86/SDL2_mixer.lib" )

ModuleAudio::ModuleAudio() : Module()
{
	// Initialize all music pointers to nullptr
	for (int i = 0; i < MAX_MUSICS; i++) {
		musics[i] = nullptr;
	}
	music = nullptr;
	// Initialize all fx pointers to nullptr
	for (int i = 0; i < MAX_FXS; i++) {
		fxs[i] = nullptr;
	}
	fx = nullptr;
}

// Destructor
ModuleAudio::~ModuleAudio()
{}

// Called before audio is available
bool ModuleAudio::Init()
{
	LOG("Init Audios library");
	Mix_Init(MIX_INIT_OGG);
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	
	bool ret = true;

	// load support for the PNG image format
	int flags = MIX_INIT_OGG;
	int init = Mix_Init(flags);

	if ((init & flags) != flags)
	{
		LOG("Could not initialize mixer lib. Mix_Init: %s", Mix_GetError());
		ret = false;
	}

	music = LoadMusic("../../Assets/Audio/Music/02_Into_the_Human_Body_Stage_1_.ogg");
	fx = LoadFx("../../Assets/Audio/SFX/xmultipl-017.wav");
	Mix_PlayMusic(music, -1);
	Mix_PlayChannel(-1, fx, 0);

	return ret;
}

update_status ModuleAudio::PreUpdate()
{
	
	return update_status::UPDATE_CONTINUE;
}

update_status ModuleAudio::Update()
{
	return update_status::UPDATE_CONTINUE;
}

update_status ModuleAudio::PostUpdate()
{
	return update_status::UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleAudio::CleanUp()
{
	LOG("Freeing music and fx library");

	// Free all music and fx
	for (int i = 0; i < MAX_MUSICS; i++) {
		Mix_FreeMusic(musics[i]);
	}
	for (int i = 0; i < MAX_FXS; i++) {
		Mix_FreeChunk(fxs[i]);
	}

	Mix_CloseAudio();
	return true;
}

// Load new music from file path
Mix_Music * const ModuleAudio::LoadMusic(const char* path)
{
	// Load a music from a path (must be a ogg)
	// and check for errors
	music = Mix_LoadMUS(path);
	if (!music) {
		LOG("Mix_LoadMUS: %s\n", Mix_GetError());
		// handle error
	}
	else {

		if (musics[MAX_MUSICS] != nullptr) {
			LOG("Musics aren't empty");
		}
		else {
			for (int i = 0; i < MAX_MUSICS; i++) {
				if (musics[i] == nullptr) {
					musics[i] = music;
				}
			}
		}
	}
	return music;
}

// Load new fx from file path
Mix_Chunk * const ModuleAudio::LoadFx(const char* path)
{

	// Load a music from a path (must be a ogg)
	// and check for errors
	fx = Mix_LoadWAV(path);
	if (!fx) {
		LOG("Mix_LoadWAV: %s\n", Mix_GetError());
		// handle error
	}
	else {

		if (fxs[MAX_FXS] != nullptr) {
			LOG("fxs aren't empty");
		}
		else {
			for (int i = 0; i < MAX_FXS; i++) {
				if (fxs[i] == nullptr) {
					fxs[i] = fx;
				}
			}
		}
	}

	return fx;
}