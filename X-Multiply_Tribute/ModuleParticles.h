#ifndef __MODULEPARTICLES_H__
#define __MODULEPARTICLES_H__

#include "Module.h"
#include "Animation.h"
#include "Globals.h"
#include "p2Point.h"
#include "ModuleCollision.h"
#include "Enemy.h"

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
	bool hastargeted = false;
	int w, h = 0;
	Uint32 born = 0;
	Uint32 life = 0;
	int offsetx = 0;
	int offsety = 0;
	bool fx_played = false;
	bool isPlayerAttached = false;
	bool flipX, flipY;
	Mix_Chunk* sfx = nullptr;
	int id = 0;
	int nTimes;
	bool isMultiple;
	int timebeforeanotherexplosion = 0;
	bool preparation = true;
	int preparationtimer = 0;
	bool missileUp = false;
	SDL_Point center = {0,0};
	float rangle = 0;

	Particle();
	Particle(const Particle& p);
	~Particle();
	bool Update();
	Enemy* closerTarget(iPoint _position);
	Enemy* target;
};

class ModuleParticles : public Module
{
public:
	ModuleParticles();
	~ModuleParticles();

	bool Start();
	update_status Update();
	bool CleanUp();


	void AddParticle(const Particle& particle, int x, int y, COLLIDER_TYPE collider_type = COLLIDER_NONE, fPoint speed = { 0,0 }, Uint32 delay = 0, int Ntimes = 1, bool isMultiple = false, bool flipX = false, bool flipY = false, bool _up = false, iPoint offset = { 0, 0 });
	void OnCollision(Collider* c1, Collider* c2);
	float AbsoluteRotation(iPoint originPos, iPoint targetPos);

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
	Particle redBall;
	Particle hosturball;
	Particle hosturballmid;
	Particle hosturballmiddeath;
	Particle hosturballdeath;
	Particle hostursmallshot;
	Particle hostursmallverticalshot;
	Particle hosturlongshot;
	Particle hosturbigshot;
	Particle hosturbiglargeshot;
	Particle missile;
	Particle wallshot;
	Particle wallshot2;
	Particle walltentshot;
	Particle walltentshot2;
	Particle walltentshot3;
	Particle missileexplosion;
	Particle zarikasubeam;
};

#endif // __MODULEPARTICLES_H__