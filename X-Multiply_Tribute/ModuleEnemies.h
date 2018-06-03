#ifndef __ModuleEnemies_H__
#define __ModuleEnemies_H__

#include "Module.h"


#define MAX_ENEMIES 200

struct Mix_Chunk;

class Enemy;

struct EnemyInfo
{
	ENEMY_TYPES type = ENEMY_TYPES::NO_TYPE;
	int x, y, powerUpid, speed;
	bool going_up;
	bool normal_spawn;
	bool tail;
};

class ModuleEnemies : public Module
{
public:

	ModuleEnemies();
	~ModuleEnemies();
	bool Start();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();
	void OnCollision(Collider* c1, Collider* c2);
	bool AddEnemy(ENEMY_TYPES type, int x, int y, bool going_up = false, int speed = 0, int powerUPid = -1, bool normal_spawn = true, bool tail = false);
	ENEMY_TYPES type = ENEMY_TYPES::NO_TYPE;
	void SpawnEnemy(const EnemyInfo& info);
	Enemy* SpawnEnemyRet(const EnemyInfo& info);
	Enemy* enemies[MAX_ENEMIES];
	void Kill(Enemy* e);

private:
	EnemyInfo queue[MAX_ENEMIES];
	SDL_Texture* sprites;
	int lives[MAX_ENEMIES];

protected:

	Mix_Chunk * pushipDeadsfx;
	Mix_Chunk * nemonaDeadsfx;
	Mix_Chunk * hellballDeadsfx;
	Mix_Chunk * flyerDeadsfx;
	Mix_Chunk * hitEnemysfx;

};

#endif // __ModuleEnemies_H__