#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleParticles.h"
#include "ModuleAudio.h"
#include "ModuleCollision.h"
#include "ModuleFadeToBlack.h"
#include "ModuleSceneStart.h"
#include "ModuleUI.h"
#include "ModulePowerUPS.h"
#include "SDL_mixer/include/SDL_mixer.h"
#include "ModulePlayer.h"

enum pu_type;

ModulePlayer::ModulePlayer()
{}

ModulePlayer::~ModulePlayer()
{}

bool ModulePlayer::Init() {
	// idle animation (arcade sprite sheet)
	idle.PushBack({ 100, 1, 36, 14 });
	idle.speed = 0.2f;

	// walk forward animation (arcade sprite sheet)
	up.PushBack({ 53, 2, 36, 14 });
	up.PushBack({ 5, 2, 36, 14 });
	up.speed = 0.1f;
	up.loop = false;

	uptoidle.PushBack({ 5, 2, 36, 14 });
	uptoidle.PushBack({ 53, 2, 36, 14 });
	uptoidle.PushBack({ 100, 1, 36, 14 });
	uptoidle.speed = 0.2f;
	uptoidle.loop = false;

	down.PushBack({ 149, 1, 36, 14 });
	down.PushBack({ 198, 1, 36, 14 });
	down.speed = 0.1f;
	down.loop = false;

	downtoidle.PushBack({ 198, 1, 36, 14 });
	downtoidle.PushBack({ 149, 1, 36, 14 });
	downtoidle.PushBack({ 100, 1, 36, 14 });
	downtoidle.speed = 0.2f;
	downtoidle.loop = false;

	death.PushBack({ 11, 386, 40, 42 });
	death.PushBack({ 67, 386, 40, 42 });
	death.PushBack({ 120, 386, 40, 42 });
	death.PushBack({ 174, 386, 40, 42 });
	death.PushBack({ 219, 386, 40, 42 });
	death.loop = false;
	death.speed = 0.3f;

	firecircle = { 150,113,28,26 };

	return true;
}

// Load assets
bool ModulePlayer::Start()
{
	dead = false;
	injecting = true;
	startime = 35;
	death.Reset();
	canMove = false;
	startBoost = false;
	current_animation = &idle;

	LOG("Loading player textures");
	graphics = App->textures->Load("Assets/Sprites/MainCharacter/spr_maincharacter.png"); // arcade version

	deadsfx = App->audio->LoadFx("Assets/Audio/SFX/xmultipl-018.wav");

	SDL_Rect rect_collider = { position.x,position.y,36,14 };
	SDL_Rect rect_tentaclecol = { tentacle.position.x,tentacle.position.y,19,10 };
	SDL_Rect rect_tentaclecol2 = { tentacle2.position.x,tentacle2.position.y,19,10 };
	SDL_Rect rect_tentacle_piece1 = { tentacle_piece1.position.x,tentacle_piece1.position.y,6,6 };
	SDL_Rect rect_tentacle_piece2 = { tentacle_piece2.position.x,tentacle_piece2.position.y,6,6 };


	collider = App->collision->AddCollider(rect_collider, COLLIDER_PLAYER, this);

	//tentacle.base_position = { position.x, position.y - 50};
	//tentacle2.base_position = { position.x, position.y + 54};
	tentacle_piece1.position = { position.x, position.y};
	tentacle_piece2.position = { position.x, position.y };

	//tentacle.collider = App->collision->AddCollider(rect_tentaclecol, COLLIDER_PLAYER_SHOT);
	//tentacle2.collider = App->collision->AddCollider(rect_tentaclecol2, COLLIDER_PLAYER_SHOT);
	tentacle_piece1.collider = App->collision->AddCollider(rect_tentaclecol2, COLLIDER_PLAYER_SHOT);
	tentacle_piece2.collider = App->collision->AddCollider(rect_tentaclecol2, COLLIDER_PLAYER_SHOT);

	circle_1.my_tentacle = &tentacle;
	circle_1.Prepare();
	circle_2.my_tentacle = &tentacle2;
	circle_2.Prepare();

	if (!activePU[TENTACLES]) {
		//tentacle.collider->enable = false;
		//tentacle2.collider->enable = false;
		tentacle_piece1.collider->enable = false;
		tentacle_piece2.collider->enable = false;
	}

	current_animation = &idle;
	state = idl;

	return true;
}

bool ModulePlayer::CleanUp() {
	App->textures->Unload(graphics);
	graphics = nullptr;

	if(collider!=nullptr) collider->to_delete = true;
	collider = nullptr;

	App->audio->UnloadSFX(deadsfx);
	deadsfx = nullptr;

	/*if(tentacle.collider)tentacle.collider->to_delete = true;
	tentacle.collider = nullptr;

	if (tentacle2.collider)tentacle2.collider->to_delete = true;
	tentacle2.collider = nullptr;*/

	if (tentacle_piece1.collider)tentacle_piece1.collider->to_delete = true;
	tentacle_piece1.collider = nullptr;

	if (tentacle_piece2.collider)tentacle_piece2.collider->to_delete = true;
	tentacle_piece2.collider = nullptr;

	return true;
}

void ModulePlayer::OnCollision(Collider * rect_a, Collider * rect_b)
{
	if (rect_b->type != COLLIDER_POWER_UP) Die();
}

// Update: draw background
update_status ModulePlayer::PreUpdate()
{
	if (injecting) {
		App->render->Blit(graphics, position.x, position.y, new SDL_Rect({101, 1, 35, 14}));
	}

	return UPDATE_CONTINUE;
}
update_status ModulePlayer::Update()
{
	if (startBoost)
	{
		if (startime == 35) {
			App->particles->AddParticle(App->particles->playerBoost, position.x - 42, position.y, COLLIDER_NONE, { 3,0 });

			/*tentacle2.position.x = position.x;
			tentacle2.position.y = position.y + 54;
			tentacle.position.x = position.x;
			tentacle.position.y = position.y - 50;*/
		}
		lastY = position.y;
		position.x += speed.x;
		startime--;
		current_animation = &idle;
		state = idl;

		if (startime == 0)
		{
			startBoost = false;
			canMove = true;
		}
	}
	if (canMove) {

		if (lastY > position.y)state = top;
		else if (lastY < position.y)state = bot;
		else state = idl;

		lastY = position.y;
		/*tentacle.base_position = { position.x, position.y - 50 };
		tentacle2.base_position = { position.x, position.y + 54 };*/
		tentacle_piece1.position = { position.x + 18,position.y - 6 };
		tentacle_piece2.position = { position.x + 18,position.y + 10 };

		/*tentacle.MoveTentacle(tentacle.rest, 1);
		tentacle2.MoveTentacle(tentacle2.rest, 2);

		tentacle.collider->SetPos(tentacle.position.x, tentacle.position.y);
		tentacle2.collider->SetPos(tentacle2.position.x, tentacle2.position.y);*/
		tentacle_piece1.collider->SetPos(tentacle_piece1.position.x, tentacle_piece1.position.y);
		tentacle_piece2.collider->SetPos(tentacle_piece2.position.x, tentacle_piece2.position.y);




		if (App->input->keyboard[SDL_SCANCODE_D] == KEY_STATE::KEY_REPEAT || SDL_GameControllerGetAxis(App->input->controller, SDL_CONTROLLER_AXIS_LEFTX) > CONTROLLER_DEAD_ZONE)
		{
			current_animation = &idle;
			position.x += speed.x;
			angle+=0.1;
			/*tentacle.MoveTentacle(tentacle.right, 1);
			tentacle2.MoveTentacle(tentacle2.right, 2);*/
			

			if (((position.x + 36) * SCREEN_SIZE) > (App->render->camera.x + SCREEN_WIDTH * SCREEN_SIZE)) position.x -= speed.x; //36 is player width
		}
		if (App->input->keyboard[SDL_SCANCODE_A] == KEY_STATE::KEY_REPEAT || SDL_GameControllerGetAxis(App->input->controller, SDL_CONTROLLER_AXIS_LEFTX) < -CONTROLLER_DEAD_ZONE)
		{
			current_animation = &idle;
			position.x -= speed.x;
			angle-=0.1;
			/*tentacle.MoveTentacle(tentacle.left, 1);
			tentacle2.MoveTentacle(tentacle2.left, 2);*/

			if ((position.x * SCREEN_SIZE) < App->render->camera.x) position.x += speed.x;
		}
		if (App->input->keyboard[SDL_SCANCODE_W] == KEY_STATE::KEY_REPEAT  && !movedDown)
		{
			
			if (App->input->keyboard[SDL_SCANCODE_S] == KEY_STATE::KEY_REPEAT)current_animation = &uptoidle;
			else {
				current_animation = &up;
				position.y -= speed.x;
				/*tentacle.MoveTentacle(tentacle.up, 1);
				tentacle2.MoveTentacle(tentacle2.up, 2);*/
			}
			
			if ((position.y * SCREEN_SIZE) < App->render->camera.y) position.y += speed.x;
		}
		else if ( SDL_GameControllerGetAxis(App->input->controller, SDL_CONTROLLER_AXIS_LEFTY) < -CONTROLLER_DEAD_ZONE && !movedDown)
		{

			if (SDL_GameControllerGetAxis(App->input->controller, SDL_CONTROLLER_AXIS_LEFTY) > CONTROLLER_DEAD_ZONE) current_animation = &uptoidle;
			else {
				current_animation = &up;
				position.y -= speed.x;
				angle-=0.1;
				/*tentacle.MoveTentacle(tentacle.up, 1);
				tentacle2.MoveTentacle(tentacle2.up, 2);*/
				gamepadActived = true;
			}

			if ((position.y * SCREEN_SIZE) < App->render->camera.y) position.y += speed.x;
		}
		else if (App->input->keyboard[SDL_SCANCODE_S] == KEY_STATE::KEY_REPEAT)
		{
			
			if (App->input->keyboard[SDL_SCANCODE_W] == KEY_STATE::KEY_REPEAT)current_animation = &downtoidle; 
			else {
				current_animation = &down;
				position.y += speed.x;
				/*tentacle.MoveTentacle(tentacle.down, 1);
				tentacle2.MoveTentacle(tentacle2.down, 2);*/
				movedDown = true;
			}
			
			if (((position.y + 44) * SCREEN_SIZE) > (App->render->camera.y + SCREEN_HEIGHT * SCREEN_SIZE)) position.y -= speed.x;
		}
		else if (SDL_GameControllerGetAxis(App->input->controller, SDL_CONTROLLER_AXIS_LEFTY) > CONTROLLER_DEAD_ZONE)
		{

			if (SDL_GameControllerGetAxis(App->input->controller, SDL_CONTROLLER_AXIS_LEFTY) < -CONTROLLER_DEAD_ZONE)current_animation = &downtoidle;		
			else {
				current_animation = &down;
				position.y += speed.x;
				angle+=0.1;
				/*tentacle.MoveTentacle(tentacle.down, 1);
				tentacle2.MoveTentacle(tentacle2.down, 2);*/
				movedDown = true;
				gamepadActived = true;
			}

			if (((position.y + 44) * SCREEN_SIZE) > (App->render->camera.y + SCREEN_HEIGHT * SCREEN_SIZE)) position.y -= speed.x;
		}
		else if (App->input->keyboard[SDL_SCANCODE_S] == KEY_STATE::KEY_UP) {
			current_animation = &downtoidle;
			movedDown = false;
		}
		else if (App->input->keyboard[SDL_SCANCODE_W] == KEY_STATE::KEY_UP)current_animation = &uptoidle;
		else if (SDL_GameControllerGetAxis(App->input->controller, SDL_CONTROLLER_AXIS_LEFTY) < CONTROLLER_DEAD_ZONE &&
			SDL_GameControllerGetAxis(App->input->controller, SDL_CONTROLLER_AXIS_LEFTY) > -CONTROLLER_DEAD_ZONE && gamepadActived && movedDown) {
			current_animation = &downtoidle;
			movedDown = false;
			gamepadActived = false;
		}
		else if(SDL_GameControllerGetAxis(App->input->controller, SDL_CONTROLLER_AXIS_LEFTY) < CONTROLLER_DEAD_ZONE &&
			SDL_GameControllerGetAxis(App->input->controller, SDL_CONTROLLER_AXIS_LEFTY) > -CONTROLLER_DEAD_ZONE && gamepadActived){
			current_animation = &uptoidle;
			movedDown = false;
			gamepadActived = false;
		}
		
		if (App->input->keyboard[SDL_SCANCODE_SPACE] == KEY_STATE::KEY_DOWN || App->input->controller_A_button == KEY_STATE::KEY_DOWN)
		{
			if (activePU[LASER]) {
				App->particles->AddParticle(App->particles->laserstart, tentacle.position.x + 5, tentacle.position.y, COLLIDER_PLAYER_SHOT);
				App->particles->AddParticle(App->particles->laser, tentacle.position.x + 5, tentacle.position.y, COLLIDER_PLAYER_SHOT);

				App->particles->AddParticle(App->particles->laserstart, position.x + 25, position.y + 5, COLLIDER_PLAYER_SHOT);
				App->particles->AddParticle(App->particles->laser, position.x + 25, position.y + 5, COLLIDER_PLAYER_SHOT);

				App->particles->AddParticle(App->particles->laserstart, tentacle2.position.x + 5, tentacle2.position.y, COLLIDER_PLAYER_SHOT);
				App->particles->AddParticle(App->particles->laser, tentacle2.position.x + 5, tentacle2.position.y, COLLIDER_PLAYER_SHOT);
			}
			else if (activePU[SHADOW]) {
				App->particles->AddParticle(App->particles->blueCircle, tentacle.position.x, tentacle.position.y, COLLIDER_PLAYER_SHOT, { circle_1.shot_vector.x, circle_1.shot_vector.y});
				App->particles->AddParticle(App->particles->blueCircle, tentacle.position.x, tentacle.position.y, COLLIDER_PLAYER_SHOT, { circle_1.shot_vector2.x, circle_1.shot_vector2.y});

				App->particles->AddParticle(App->particles->blueCircle, tentacle2.position.x, tentacle2.position.y, COLLIDER_PLAYER_SHOT, { circle_2.shot_vector.x, circle_2.shot_vector.y});
				App->particles->AddParticle(App->particles->blueCircle, tentacle2.position.x, tentacle2.position.y, COLLIDER_PLAYER_SHOT, { circle_2.shot_vector2.x, circle_2.shot_vector2.y});
			}
			else
			{
				App->particles->AddParticle(App->particles->baseShotExp, position.x + 30, position.y + 1);
				App->particles->AddParticle(App->particles->baseShot, position.x + 25, position.y + 5, COLLIDER_PLAYER_SHOT);

				if (activePU[TENTACLES])
				{
					App->particles->AddParticle(App->particles->baseShotExp, tentacle.position.x + 30, tentacle.position.y + 1);
					App->particles->AddParticle(App->particles->tentacleBaseShot, tentacle.position.x, tentacle.position.y, COLLIDER_PLAYER_SHOT);

					App->particles->AddParticle(App->particles->baseShotExp, tentacle2.position.x + 30, tentacle2.position.y + 1);
					App->particles->AddParticle(App->particles->tentacleBaseShot, tentacle2.position.x, tentacle2.position.y, COLLIDER_PLAYER_SHOT);
				}
			}		
			cooldown = 0;
		}
		if (App->input->keyboard[SDL_SCANCODE_SPACE] == KEY_STATE::KEY_REPEAT || App->input->controller_A_button == KEY_STATE::KEY_REPEAT)
		{
			if (activePU[BOMB] == true && cooldownBombs >= 40)
			{
				App->particles->AddParticle(App->particles->bombshot, position.x + 25, position.y + 5, COLLIDER_PLAYER_SHOT);
				cooldownBombs = 0;
			}
			if (activePU[MISSILE] == true && cooldownMissiles >= 60)
			{
				App->particles->AddParticle(App->particles->missile, position.x + 25, position.y, COLLIDER_PLAYER_SHOT, { 0, 0 }, 0, 1, false, false, false, true, {-2, -4});
				App->particles->AddParticle(App->particles->missile, position.x , position.y, COLLIDER_PLAYER_SHOT, { 0, 0 }, 0, 1, false, false, false, true, {0, +7});
				App->particles->AddParticle(App->particles->missile, position.x + 25, position.y, COLLIDER_PLAYER_SHOT, { 0, 0 }, 0, 1, false, false, false, false, {0, 2 });
				App->particles->AddParticle(App->particles->missile, position.x , position.y, COLLIDER_PLAYER_SHOT, { 0, 0 }, 0, 1, false, false, false, false, { -2, +12 });
				cooldownMissiles = 0;
			}
			if (cooldown >= 25)
			{
				if (activePU[LASER]) {
					App->particles->AddParticle(App->particles->laserstart, tentacle.position.x + 5, tentacle.position.y, COLLIDER_PLAYER_SHOT);
					App->particles->AddParticle(App->particles->laser, tentacle.position.x + 5, tentacle.position.y, COLLIDER_PLAYER_SHOT);

					App->particles->AddParticle(App->particles->laserstart, position.x + 25, position.y + 5, COLLIDER_PLAYER_SHOT);
					App->particles->AddParticle(App->particles->laser, position.x + 25, position.y + 5, COLLIDER_PLAYER_SHOT);

					App->particles->AddParticle(App->particles->laserstart, tentacle2.position.x + 5, tentacle2.position.y, COLLIDER_PLAYER_SHOT);
					App->particles->AddParticle(App->particles->laser, tentacle2.position.x + 5, tentacle2.position.y, COLLIDER_PLAYER_SHOT);
				}
				else if (activePU[SHADOW]) {
         			App->particles->AddParticle(App->particles->blueCircle, tentacle.position.x, tentacle.position.y, COLLIDER_PLAYER_SHOT, { circle_1.shot_vector.x, circle_1.shot_vector.y});
					App->particles->AddParticle(App->particles->blueCircle, tentacle.position.x, tentacle.position.y, COLLIDER_PLAYER_SHOT, { circle_1.shot_vector2.x, circle_1.shot_vector2.y});

					App->particles->AddParticle(App->particles->blueCircle, tentacle2.position.x, tentacle2.position.y, COLLIDER_PLAYER_SHOT, { circle_2.shot_vector.x, circle_2.shot_vector.y});
					App->particles->AddParticle(App->particles->blueCircle, tentacle2.position.x, tentacle2.position.y, COLLIDER_PLAYER_SHOT, { circle_2.shot_vector2.x, circle_2.shot_vector2.y});
				}
				else
				{
					App->particles->AddParticle(App->particles->baseShotExp, position.x + 30, position.y + 1);
					App->particles->AddParticle(App->particles->baseShot, position.x + 25, position.y + 5, COLLIDER_PLAYER_SHOT);

					if (activePU[TENTACLES])
					{
						App->particles->AddParticle(App->particles->baseShotExp, tentacle.position.x + 30, tentacle.position.y + 1);
						App->particles->AddParticle(App->particles->tentacleBaseShot, tentacle.position.x, tentacle.position.y, COLLIDER_PLAYER_SHOT);

						App->particles->AddParticle(App->particles->baseShotExp, tentacle2.position.x + 30, tentacle2.position.y + 1);
						App->particles->AddParticle(App->particles->tentacleBaseShot, tentacle2.position.x, tentacle2.position.y, COLLIDER_PLAYER_SHOT);
					}
				}			
				cooldown = 0;
			}
		}
		if (App->input->keyboard[SDL_SCANCODE_F3] == KEY_STATE::KEY_DOWN) App->ui->StageCleared();
		if (App->input->keyboard[SDL_SCANCODE_F6] == KEY_STATE::KEY_DOWN) App->ui->IncreaseLife();
		if (App->input->keyboard[SDL_SCANCODE_F4] == KEY_STATE::KEY_DOWN && !dead) Die();
		if (App->input->keyboard[SDL_SCANCODE_F2] == KEY_STATE::KEY_DOWN && !activePU[TENTACLES]) {
			activePU[TENTACLES] = true;
			/*App->player->tentacle.collider->enable = true;
			App->player->tentacle2.collider->enable = true;*/
			App->player->tentacle_piece1.collider->enable = true;
			App->player->tentacle_piece2.collider->enable = true;
		}

		if (last_animation != current_animation) {
			current_animation->Reset();
		}
		last_animation = current_animation;
	}
	else {
		if (current_animation->isDone() && dead) App->ui->DeathFade();
	}	

	// Draw everything --------------------------------------
	SDL_Rect r = current_animation->GetCurrentFrame();
	if(collider != nullptr) collider->SetPos(position.x, position.y);
	App->render->Blit(graphics, position.x, position.y, &r);
	if (activePU[SHADOW])
	{
		circle_1.Move(position.x, position.y);
		circle_2.Move(position.x, position.y);

		for (int i = 0; i < 5; i++) {
			App->render->Blit(graphics, circle_1.chunks[i].position.x, circle_1.chunks[i].position.y, &circle_1.chunks[i].srcRect);
			App->render->Blit(graphics, circle_2.chunks[i].position.x, circle_2.chunks[i].position.y, &circle_2.chunks[i].srcRect);
		}

		App->render->Blit(graphics, circle_1.position.x, circle_1.position.y, &firecircle);
		App->render->Blit(graphics, circle_2.position.x, circle_2.position.y, &firecircle);
	}
	if (activePU[TENTACLES])
	{
		/*App->render->Blit(graphics, tentacle.position.x, tentacle.position.y, &(tentacle.anim.GetCurrentFrame()));
		App->render->Blit(graphics, tentacle2.position.x, tentacle2.position.y, &(tentacle2.anim.GetCurrentFrame()));*/
		tentacle_piece1.RenderTentacle(graphics, false, angle);
		tentacle_piece2.RenderTentacle(graphics, true, angle);
	}


	if (cooldown < 25) {
		cooldown++;
	}

	if (cooldownBombs < 40) {
		cooldownBombs++;
	}

	if (cooldownMissiles < 60) {
		cooldownMissiles++;
	}

	return UPDATE_CONTINUE;
}

void ModulePlayer::Die() {
	if (App->ui->current_step != none) return;
	position.x -= 5;
	position.y -=10;
	dead = true;
	canMove = false;
	current_animation = &death;
	collider->to_delete = true;
	speed.x = 2;
	speed.y = 2;
	
	Mix_PlayChannel(-1, deadsfx, 0);
	ResetPU();
}

void ModulePlayer::BlitPlayer() {
	if(App->player->IsEnabled()) App->render->Blit(graphics, position.x, position.y, &current_animation->GetCurrentFrame());

	if (!canMove && activePU[TENTACLES]) {
		fPoint clear_position = { position.x,position.y - 50 };
		fPoint origin_position = { tentacle.position.x, tentacle.position.y };
		float distance = origin_position.DistanceTo(clear_position);
		fPoint direction;
		if (distance <= 0.01f && distance >= -0.01f) tentacle.position = clear_position;
		else
		{
			direction = { clear_position.x / distance - origin_position.x / distance, clear_position.y / distance - origin_position.y / distance };

			tentacle.position.x += direction.x * 2;
			tentacle.position.y += direction.y;

			tentacle.collider->SetPos(tentacle.position.x, tentacle.position.y);
			tentacle2.collider->SetPos(tentacle2.position.x, tentacle2.position.y);

			if (origin_position.DistanceTo(tentacle.position) >= distance)
			{
				tentacle.position = clear_position;
			}
		}

		clear_position = { position.x,position.y + 54 };
		origin_position = { tentacle2.position.x, tentacle2.position.y };
		distance = origin_position.DistanceTo(clear_position);

		if (distance <= 0.01f && distance >= -0.01f) tentacle2.position = clear_position;
		else {
			direction = { clear_position.x / distance - origin_position.x / distance, clear_position.y / distance - origin_position.y / distance };

			tentacle2.position.x += direction.x * 2;
			tentacle2.position.y += direction.y;

			if (origin_position.DistanceTo(tentacle2.position) >= distance)
			{
				tentacle2.position = clear_position;
			}
		}
	}
	if(activePU[TENTACLES]){

	App->render->Blit(graphics, tentacle.position.x, tentacle.position.y, &(tentacle.anim.GetCurrentFrame()));
	App->render->Blit(graphics, tentacle2.position.x, tentacle2.position.y, &(tentacle2.anim.GetCurrentFrame()));
	App->render->Blit(graphics, tentacle_piece1.position.x, tentacle_piece1.position.y, &(tentacle_piece1.anim.GetCurrentFrame()));
	App->render->Blit(graphics, tentacle_piece2.position.x, tentacle_piece2.position.y, &(tentacle_piece2.anim.GetCurrentFrame()));

		App->render->Blit(graphics, tentacle.position.x, tentacle.position.y, &(tentacle.anim.GetCurrentFrame()));
		App->render->Blit(graphics, tentacle2.position.x, tentacle2.position.y, &(tentacle2.anim.GetCurrentFrame()));

	}
}

void ModulePlayer::ResetPU() {
	for (int i = 0; i < sizeof(activePU); i++)
	{
		activePU[i] = false;
	}
}

// -------------------------------------------------------------
// -------------------------------------------------------------


Tentacle::Tentacle()
{

	anim.PushBack({ 90, 18, 19, 9 });
	anim.PushBack({ 122, 19, 19, 7 });
	anim.PushBack({ 154, 20, 19, 6 });
	anim.PushBack({ 186, 20, 19, 7 });
	anim.PushBack({ 218, 19, 19, 9 });
	anim.PushBack({ 10, 34, 19, 10 });
	anim.loop = true;
	anim.speed = 0.2f;

	anim.PushBack({ 90, 18, 19, 9 });
	anim.PushBack({ 122, 19, 19, 7 });
	anim.PushBack({ 154, 20, 19, 6 });
	anim.PushBack({ 186, 20, 19, 7 });
	anim.PushBack({ 218, 19, 19, 9 });
	anim.PushBack({ 10, 34, 19, 10 });
	anim.loop = true;
	anim.speed = 0.2f;

	position = base_position;

}

Tentacle::~Tentacle()
{
	if (collider != nullptr)
		collider->to_delete = true;
}

void Tentacle::MoveTentacle(movement movement, int location_position)
{
	switch (movement)
	{
	case Tentacle::up:
		if (location_position == 1) {
			if (position.y < App->player->position.y + 5) {
				position.y += App->player->speed.x;
			}
			final_position = { position.x,App->player->position.y + 5 };
			origin_position = { position.x, position.y };
			distance = origin_position.DistanceTo(final_position);
			if (8 >= distance)
			{
				position = final_position;
			}
			final_position = { App->player->position.x - 50,position.y };
			origin_position = { position.x, position.y };
			distance = origin_position.DistanceTo(final_position);
			if (8 >= distance)
			{
				position = final_position;
			}
			final_position = { App->player->position.x + 50,position.y };
			origin_position = { position.x, position.y };
			distance = origin_position.DistanceTo(final_position);
			if (8 >= distance)
			{
				position = final_position;
			}
		}
		else
		{
			if (position.y < App->player->position.y + 54) {
				position.y += App->player->speed.x;
			}
			final_position = { position.x,App->player->position.y + 54 };
			origin_position = { position.x, position.y };
			distance = origin_position.DistanceTo(final_position);
			if (8 >= distance)
			{
				position = final_position;
			}
			final_position = { App->player->position.x - 50,position.y };
			origin_position = { position.x, position.y };
			distance = origin_position.DistanceTo(final_position);
			if (8 >= distance)
			{
				position = final_position;
			}
			final_position = { App->player->position.x + 50,position.y };
			origin_position = { position.x, position.y };
			distance = origin_position.DistanceTo(final_position);
			if (8 >= distance)
			{
				position = final_position;
			}
		}
		break;
	case Tentacle::down:
		if (location_position == 1) {
			if (position.y > App->player->position.y - 50) {
				position.y -= App->player->speed.x;
			}
			final_position = { position.x,App->player->position.y - 50 };
			origin_position = { position.x, position.y };
			distance = origin_position.DistanceTo(final_position);
			if (8 >= distance)
			{
				position = final_position;
			}
			final_position = { App->player->position.x - 50,position.y };
			origin_position = { position.x, position.y };
			distance = origin_position.DistanceTo(final_position);
			if (8 >= distance)
			{
				position = final_position;
			}
			final_position = { App->player->position.x + 50,position.y };
			origin_position = { position.x, position.y };
			distance = origin_position.DistanceTo(final_position);
			if (8 >= distance)
			{
				position = final_position;
			}
		}
		else
		{
			if (position.y >= App->player->position.y - 5) {
				position.y -= App->player->speed.x;
			}
			final_position = { position.x,App->player->position.y - 5 };
			origin_position = { position.x, position.y };
			distance = origin_position.DistanceTo(final_position);
			if (8 >= distance)
			{
				position = final_position;
			}
			final_position = { App->player->position.x - 50,position.y };
			origin_position = { position.x, position.y };
			distance = origin_position.DistanceTo(final_position);
			if (8 >= distance)
			{
				position = final_position;
			}
			final_position = { App->player->position.x + 50, position.y };
			origin_position = { position.x, position.y };
			distance = origin_position.DistanceTo(final_position);
		}
		break;
	case Tentacle::right:
		if (location_position == 1) {
			if (position.x >= App->player->position.x - 50) {
				position.x -= (App->player->speed.x * 2);
			}
			else
			{
				final_position = { App->player->position.x - 50,position.y };
				origin_position = { position.x, position.y };
				distance = origin_position.DistanceTo(final_position);
				if (8 >= distance)
				{
					position = final_position;
				}

				if (position.y < App->player->position.y + 4) {
					position.y += App->player->speed.x * 2;
				}

				final_position = { position.x,App->player->position.y + 4 };
				origin_position = { position.x, position.y };
				distance = origin_position.DistanceTo(final_position);
				if (8 >= distance)
				{
					position = final_position;
				}
			}
		}
		else
		{
			if (position.x >= App->player->position.x - 50) {
				position.x -= App->player->speed.x * 2;
			}
			else
			{
				final_position = { App->player->position.x - 50,position.y };
				origin_position = { position.x, position.y };
				distance = origin_position.DistanceTo(final_position);
				if (8 >= distance)
				{
					position = final_position;
				}
				if (position.y > App->player->position.y + 4) {
					position.y -= App->player->speed.x * 2;
				}
				final_position = { position.x,App->player->position.y + 4 };
				origin_position = { position.x, position.y };
				distance = origin_position.DistanceTo(final_position);
				if (8 >= distance)
				{
					position = final_position;
				}
			}
		}
		break;
	case Tentacle::left:

		if (location_position == 1) {
			if (position.x <= App->player->position.x + 50) {
				position.x += (App->player->speed.x * 2);
			}
			else
			{
				final_position = { App->player->position.x + 50,position.y };
				origin_position = { position.x, position.y };
				distance = origin_position.DistanceTo(final_position);
				if (8 >= distance)
				{
					position = final_position;
				}

				if (position.y < App->player->position.y + 4) {
					position.y += App->player->speed.x * 2;
				}

				final_position = { position.x, App->player->position.y + 4 };
				origin_position = { position.x, position.y };
				distance = origin_position.DistanceTo(final_position);
				if (8 >= distance)
				{
					position = final_position;
				}
			}
		}
		else {
			if (position.x <= App->player->position.x + 50) {
				position.x += App->player->speed.x * 2;
			}
			else
			{
				final_position = { App->player->position.x + 50,position.y };
				origin_position = { position.x, position.y };
				distance = origin_position.DistanceTo(final_position);
				if (8 >= distance)
				{
					position = final_position;
				}
				if (position.y > App->player->position.y + 4) {
					position.y -= App->player->speed.x * 2;
				}
				final_position = { App->player->position.x,position.y + 4 };
				origin_position = { position.x, position.y };
				distance = origin_position.DistanceTo(final_position);
				if (8 >= distance)
				{
					position = final_position;
				}
			}
		}
		break;
	default:
		origin_position = { position.x, position.y };
		distance = origin_position.DistanceTo(base_position);
		fPoint direction;
		if (distance <= 0.01f && distance >= -0.01f) position = base_position;
		else
		{
			direction = { base_position.x / distance - origin_position.x / distance, base_position.y / distance - origin_position.y / distance };

			position.x += direction.x * 2;
			position.y += direction.y;

			if (origin_position.DistanceTo(position) >= distance)
			{
				position = base_position;
			}
		}
		break;
	}
}

// -------------------------------------------------------------
// -------------------------------------------------------------

Tentacle_Piece::Tentacle_Piece()
{
	anim.PushBack({ 6, 19, 4, 11});
	/*anim.PushBack({ 10, 9, 5, 4});
	anim.PushBack({ 18, 10, 4, 5});
	anim.PushBack({ 25, 10, 3, 5});
	anim.PushBack({ 33, 11, 2, 6});*/

	anim.loop = true;
	anim.speed = 0.2f;

}

Tentacle_Piece::~Tentacle_Piece()
{}

void Tentacle_Piece::MoveTentacle_Piece(movement movement, int location_position)
{
}
void Tentacle_Piece::RenderTentacle(SDL_Texture * graphics, bool flipPiece, float angle)
{
	Tentacle_Piece piece2;
	Tentacle_Piece piece3;
	Tentacle_Piece piece4;
	Tentacle_Piece piece5;
	Tentacle_Piece piece6;
	Tentacle head;
	angle = angle;
	int hipotenusa = sqrt((4 * 4) + (11 * 11));

	if (flipPiece)
	{

		App->render->BlitFlipped(graphics, position.x, position.y, &(anim.GetCurrentFrame()), false, true, (0, angle), { 2,5 });

		piece2.position.x = position.x - cosf(angle)*hipotenusa;
		piece2.position.y = position.y - sinf(angle)*hipotenusa;

		App->render->BlitFlipped(graphics, piece2.position.x, piece2.position.y, &(piece2.anim.GetCurrentFrame()), false, true, (0, angle+5), { 2,5 });

		piece3.position.x = piece2.position.x - cosf(angle)*hipotenusa;
		piece3.position.y = piece2.position.y - sinf(angle)*hipotenusa;

		App->render->BlitFlipped(graphics, piece3.position.x, piece3.position.y, &(piece3.anim.GetCurrentFrame()), false, true, (0, angle+10), { 2,5 });
		
		piece4.position.x = piece3.position.x - cosf(angle)*hipotenusa;
		piece4.position.y = piece3.position.y - sinf(angle)*hipotenusa;
		
		App->render->BlitFlipped(graphics, piece4.position.x, piece4.position.y, &(piece4.anim.GetCurrentFrame()), false, true, (0, angle+15), { 2,5 });
		
		piece5.position.x = piece4.position.x - cosf(angle)*hipotenusa;
		piece5.position.y = piece4.position.y - sinf(angle)*hipotenusa;
		
		App->render->BlitFlipped(graphics, piece5.position.x, piece5.position.y, &(piece5.anim.GetCurrentFrame()), false, true, (0, angle+20), { 2,5 });
		
		piece6.position.x = piece5.position.x - cosf(angle)*hipotenusa;
		piece6.position.y = piece5.position.y - sinf(angle)*hipotenusa;
		
		App->render->BlitFlipped(graphics, piece6.position.x, piece6.position.y, &(piece6.anim.GetCurrentFrame()), false, true, (0, angle+25), { 2,5 });
		
		head.position.x = (piece6.position.x);
		head.position.y = piece6.position.y;

		App->render->Blit(graphics, head.position.x, head.position.y, &(head.anim.GetCurrentFrame()));
	}
	else
	{

		App->render->BlitFlipped(graphics, position.x, position.y, &(anim.GetCurrentFrame()), false, false, (0, -angle), { 2,5 });
		
		piece2.position.x = position.x - cosf(angle)*hipotenusa;
		piece2.position.y = position.y + sinf(angle)*hipotenusa;
		
		App->render->BlitFlipped(graphics, piece2.position.x, piece2.position.y, &(piece2.anim.GetCurrentFrame()), false, false, (0, -angle-5), { 2,5 });
		
		piece3.position.x = piece2.position.x - cosf(angle)*hipotenusa;
		piece3.position.y = piece2.position.y + sinf(angle)*hipotenusa;
		
		App->render->BlitFlipped(graphics, piece3.position.x, piece3.position.y, &(piece3.anim.GetCurrentFrame()), false, false, (0, -angle-10), { 2,5 });
		
		piece4.position.x = piece3.position.x - cosf(angle)*hipotenusa;
		piece4.position.y = piece3.position.y + sinf(angle)*hipotenusa;
		
		App->render->BlitFlipped(graphics, piece4.position.x, piece4.position.y, &(piece4.anim.GetCurrentFrame()), false, false, (0, -angle-15), { 2,5 });
		
		piece5.position.x = piece4.position.x - cosf(angle)*hipotenusa;
		piece5.position.y = piece4.position.y + sinf(angle)*hipotenusa;

		App->render->BlitFlipped(graphics, piece5.position.x, piece5.position.y, &(piece5.anim.GetCurrentFrame()), false, false, (0, -angle-20), { 2,5 });
		
		piece6.position.x = piece5.position.x - cosf(angle)*hipotenusa;
		piece6.position.y = piece5.position.y + sinf(angle)*hipotenusa;
		
		App->render->BlitFlipped(graphics, piece6.position.x, piece6.position.y, &(piece6.anim.GetCurrentFrame()), false, false, (0, -angle-25), { 2,5 });
		
		head.position.x = (piece6.position.x);
		head.position.y = piece6.position.y;
		
		App->render->Blit(graphics, head.position.x, head.position.y, &(head.anim.GetCurrentFrame()));
	}
}
