#ifndef __MODULEPOWERUPS_H__
#define __MODULEPOWERUPS_H__

#include "Module.h"
#include "Globals.h"

#define MAX_POWERUPS 20

struct  Mix_Chunk;

enum pu_type
{
	LASER,
	MISSILE,
	SHADOW,
	BOMB,
	SPEED_BOOST,
	SPEED_DOWN,
	LIFE,
	TENTACLES,
};

struct PowerUP 
{
	bool enabled;
	Animation anim;
	iPoint position;
	pu_type type;
	Mix_Chunk* sfx = nullptr;
	int h, w = 0;
	Collider* collider = nullptr;

	PowerUP();
	PowerUP(const PowerUP& pu);
	~PowerUP();
	void Effect();
	void Tentacles();
	bool Update();
	bool isEnabled() { return enabled; }
	void setEnabled(bool _enable) { enabled = _enable; }
};

class ModulePowerUPS :  public Module
{
public:
	ModulePowerUPS();
	~ModulePowerUPS();

	bool Init();
	bool Start();
	update_status Update();
	bool CleanUp();

	void OnCollision(Collider* c1, Collider* c2);
	PowerUP* AddPowerUp(const PowerUP& powerup, int x, int y, COLLIDER_TYPE collider_type = COLLIDER_NONE);
	

private:
	SDL_Texture * graphics = nullptr;
	PowerUP* powerUps[MAX_POWERUPS];

public:
	PowerUP life;
	PowerUP missile;
	PowerUP speedBoost;
	PowerUP speedDown;
	PowerUP bomb;
	PowerUP laser;
	PowerUP shadow;
	PowerUP tentacle;
};

#endif

