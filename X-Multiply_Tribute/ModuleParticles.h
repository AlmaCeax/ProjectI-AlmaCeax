#ifndef __MODULEPARTICLES_H__
#define __MODULEPARTICLES_H__

#include "Module.h"
#include "Animation.h"
#include "Globals.h"
#include "p2Point.h"
#include "ModuleCollision.h"

#define MAX_ACTIVE_PARTICLES 50

struct SDL_Texture;
struct Collider;
struct Mix_Chunk;
enum COLLIDER_TYPE;

struct Particle
{
	Collider* collider = nullptr;
	Animation anim;
	uint fx = 0;
	iPoint position;
	fPoint speed;
	int w, h = 0;
	Uint32 born = 0;
	Uint32 life = 0;
	int offsetx = 0;
	int offsety = 0;
	bool fx_played = false;
	bool isPlayerAttached = false;
	Mix_Chunk* sfx = nullptr;
	int id = 0;
	int nTimes;
	bool isMultiple;
	int timebeforeanotherexplosion = 0;

	Particle();
	Particle(const Particle& p);
	~Particle();
	bool Update();
};

class ModuleParticles : public Module
{
public:
	ModuleParticles();
	~ModuleParticles();

	bool Start();
	update_status Update();
	bool CleanUp();

	void AddParticle(const Particle& particle, int x, int y, COLLIDER_TYPE collider_type = COLLIDER_NONE, fPoint speed = { 0,0 }, Uint32 delay = 0, int Ntimes = 1, bool isMultiple = false);
	void OnCollision(Collider* c1, Collider* c2);

private:

	SDL_Texture * graphics = nullptr;
	Particle* active[MAX_ACTIVE_PARTICLES];
	uint last_particle = 0;

public:
	Particle baseShot;
	Particle tentacleBaseShot;
	Particle baseShotExp;
	Particle baseShotColExp;
	Particle bombshot;
	Particle bombExplosion;
	Particle blueBall;
	Particle enemyExplosion;
	Particle enemyBossExplosion;
	Particle playerBoost;
	Particle cyclopshot;
	Particle alienshot;
	Particle browneyeshot1;
	Particle browneyeshot2;
	Particle browneyeshot3;
	Particle browneyeshot4;
	Particle browneyeshot5;
	Particle browneyeshot6;
	Particle browneyeshot7;
	Particle browneyeshot8;
	Particle multipleLittleExplosion;
	Particle multipleBigExplosion;
	Particle greenBall;
	Particle yellowCircle;
	Particle blueCircle;
};

#endif // __MODULEPARTICLES_H__