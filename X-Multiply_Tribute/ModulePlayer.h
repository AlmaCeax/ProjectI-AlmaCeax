#ifndef __ModulePlayer_H__
#define __ModulePlayer_H__

#include "Module.h"
#include "Animation.h"
#include "Globals.h"
#include "p2Point.h"
#include <math.h>

struct SDL_Texture;
struct Mix_Chunk;
struct Collider;

class Tentacle 
{
public:
	Collider* collider = nullptr;
	Animation anim;
	fPoint position;
	fPoint base_position;
	fPoint final_position;
	fPoint origin_position;
	float distance;
	int w, h = 0;
	enum movement { up, down, right, left, rest };

	Tentacle();
	~Tentacle();
	void MoveTentacle(movement movement, int location_position);
};

struct FireChunk {
	fPoint position;
	SDL_Rect srcRect;
	int parent_id;
	int current_time;
	int timer;

	void Move(fPoint pos) {
		if (current_time == 0) {
			position = pos;
			current_time = timer;
		}
		else current_time--;
	}
};
struct FireCircle {
	Tentacle* my_tentacle;
	fPoint position;
	fPoint shot_vector, shot_vector2;
	FireChunk chunks[5];

	void Prepare() {
		chunks[0].timer = 1;
		chunks[1].timer = 3;
		chunks[2].timer = 5;
		chunks[3].timer = 7;
		chunks[4].timer = 9;

		chunks[0].srcRect = { 65,113,14,14 };
		chunks[1].srcRect = {65,113,14,14};
		chunks[2].srcRect = {50,115,14,14 };
		chunks[3].srcRect = {35,116,14,14 };
		chunks[4].srcRect = {20,117,14,14 };

		chunks[0].current_time = 0;
		chunks[1].current_time = 0;
		chunks[2].current_time = 0;
		chunks[3].current_time = 0;
		chunks[4].current_time = 0;

		chunks[0].parent_id = -1;
		chunks[1].parent_id = 0;
		chunks[2].parent_id = 1;
		chunks[3].parent_id = 2;
		chunks[4].parent_id = 3;
	}
	void Move(float x, float y) {
		position = { my_tentacle->position.x-1,my_tentacle->position.y-8};
		fPoint temp = { my_tentacle->position.x - (x), my_tentacle->position.y - (y)};
		float magnitude = sqrt((pow(temp.x, 2)) + (pow(temp.y, 2)));
		shot_vector = { temp.x / magnitude, temp.y / magnitude };

		temp = { my_tentacle->position.x - 4 - (x), my_tentacle->position.y - 4 - (y) };
		magnitude = sqrt((pow(temp.x, 2)) + (pow(temp.y, 2)));
		shot_vector2 = { temp.x / magnitude, temp.y / magnitude };

		if (shot_vector.x < 0) {
			shot_vector.x *= 8;
			shot_vector.y *= 15;
			shot_vector2.x *= 8;
			shot_vector2.y *= 15;
		}
		else {
			shot_vector.x *= 10;
			shot_vector.y *= 15;
			shot_vector2.x *= 10;
			shot_vector2.y *= 15;
		}


		for (int i = 0; i < 5; i++) {
			if (i != 0) chunks[i].Move(chunks[chunks[i].parent_id].position);
			else chunks[i].Move(position);
		}
	}
};

class ModulePlayer : public Module
{
public:
	ModulePlayer();
	~ModulePlayer();

	bool Init();
	bool Start();
	bool CleanUp();
	void OnCollision(Collider*, Collider*);
	update_status PreUpdate();
	update_status Update();
	void ResetPU();

	Mix_Chunk* deadsfx = nullptr;

	fPoint position;
	enum state { top, bot, idl };
	bool activePU[9];
	state state;
	fPoint speed = {2, 0};
	bool dead = false;
	bool canMove = false;
	bool injecting = true;
	bool startBoost = false;
	void BlitPlayer();
	Collider* collider;
	Tentacle tentacle;
	Tentacle tentacle2;
	FireCircle circle_1;
	FireCircle circle_2;
	SDL_Rect firecircle;
	//Alma Ceax

	Animation* current_animation = nullptr;
	Animation up;
	Animation down;
	Animation idle;
	Animation uptoidle;
	Animation downtoidle;

private:
	void Die();

	SDL_Texture * graphics = nullptr;
	Animation* last_animation = nullptr;
	Animation death;
	Animation ejecting;
	int cooldown = 0;
	int cooldownBombs = 0;
	float lastY;
	uint startime;
	bool movedDown = false;
	bool gamepadActived = false;

};

#endif