#ifndef __MODULEPOWERUP_H__
#define __MODULEPOWERUP_H__

#include "Module.h"
#include "Animation.h"
#include "Globals.h"
#include "p2Point.h"
#include "ModuleCollision.h"

#define MAX_POWERUP 20

struct SDL_Texture;
struct Collider;
enum COLLIDER_TYPE;

struct PowerUp {
private:
	bool enable;
	Animation anim;
	iPoint position;
	int h, w = 0;
	Collider* collider = nullptr;
	COLLIDER_TYPE coll_type;

	PowerUp(iPoint _position);
	bool Update();
	bool isEnable() { return enable; }
	void setEnable(bool _enable) { enable = _enable; }

};

class ModulePowerUP : public Module
{
public:
	ModulePowerUP();
	~ModulePowerUP();

	enum pu_type { LASER, MISSILE, SHADOW, BOMB, SPEED_BOOST, SPEED_DOWN, LIFE };

	bool Init();
	bool Start();
	update_status Update();
	bool CleanUp();

	void OnCollision(Collider* c1, Collider* c2);

private:
	SDL_Texture * graphics = nullptr;
	PowerUp* poweUps[MAX_POWERUP];

public:
	PowerUp life;
	PowerUp speedBoost;
	PowerUp speedDown;
	PowerUp bomb;
	PowerUp tentacles;
};

#endif

