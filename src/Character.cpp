#include "Character.h"
#include "ProjDefs.h"
#include "DebLog.h"
#include "Player.h"
#include "mdCollision.h"
#include "mdAudio.h"
#include "mdInput.h"
#include "mdSceneManager.h"
#include "mdParticleSystem.h"

Character::Character() {
	config = App->loadConfig("config.xml", config_doc);
	data = config.child("entities");

	//LUCAS

}


Character::~Character() {

}

void Character::update(const bool(&inputs)[MAX_INPUTS]) {

	//PROVISIONAL: Crazy provisional
	if (current_life <= 0 && !death) {
		updateState(DEAD);
		hurtbox->active = false;
		death = true;
	}
	if (App->input->getKey(SDL_SCANCODE_F2) == KEY_DOWN) {
		resetCharacter();
	}
	if (App->entities->traning) {
		current_life = max_life;
		current_super_gauge = max_super_gauge;
	}

	
	fillBuffer(inputs);


	switch (current_state) {
	case IDLE:
		//Input independent actions
		//One tick
		if (!state_first_tick) {
			updateAnimation(idle);
			state_first_tick = true;
		}
		if (hit) 
			updateState(HIT);
		// Input dependent
		if (checkForSuper(super_window))
			updateState(ATTACKING, SUPER);
		else if (inputs[SWITCH]) {
			updateState(SWAPPING);				// Is this necessary?
			swapRequested = true;						//Important!
		}
		else if (inputs[RIGHT] && !fliped || inputs[LEFT] && fliped)
			updateState(WALKING_FORWARD);
		else if (inputs[LEFT] && !fliped || inputs[RIGHT] && fliped)
			updateState(WALKING_BACK);
		else if (inputs[UP]) {
			velocity.y -= jump_power.y;
			grounded = false;
			updateState(JUMPING);
		}
		else if (inputs[DOWN])
			updateState(CROUCHING);
		else if (inputs[LIGHT_ATTACK]) 
			updateState(ATTACKING, ST_L); 
		else if (inputs[HEAVY_ATTACK]) 
			updateState(ATTACKING, ST_H);
		else if (inputs[SPECIAL_1] && standingSpecial1Condition()) 
			updateState(ATTACKING, ST_S1);
		else if (inputs[SPECIAL_2]) 
			updateState(ATTACKING, ST_S2);

		break;

	case WALKING_BACK:
		// Input independent actions
		//One tick
		if (!state_first_tick) {
			updateAnimation(walk_back);
			state_first_tick = true;
		}
		if (hit) {
			if (attack_recieving.block_type != BLOCK_TYPE::LOW)
				updateState(STAND_BLOCKING);
			else
				updateState(HIT);
		}
		//Continuous
		if (fliped)
			logic_position.x += walk_speed;
		else
			logic_position.x -= walk_speed;
		// Input dependent actions
		if (!fliped && !inputs[LEFT] || fliped && !inputs[RIGHT])
			updateState(IDLE);
		else if (inputs[UP]) {
			velocity.y -= jump_power.y;
			if (!fliped)
				velocity.x -= jump_power.x;
			else
				velocity.x += jump_power.x;
			grounded = false;
			updateState(JUMPING);
		}
		else if (inputs[DOWN])
			updateState(CROUCHING);
		else if (inputs[LIGHT_ATTACK]) 
			updateState(ATTACKING, ST_L);
		else if (inputs[HEAVY_ATTACK]) 
			updateState(ATTACKING, ST_H);
		else if (inputs[SPECIAL_1] && standingSpecial1Condition())
			updateState(ATTACKING, ST_S1);
		else if (inputs[SPECIAL_2]) 
			updateState(ATTACKING, ST_S2);
		break;

	case WALKING_FORWARD:
		// Input independent actions
		//One tick
		if (!state_first_tick) {
			updateAnimation(walk_forward);
			state_first_tick = true;
		}
		if (hit)
			updateState(HIT);
		// Continuous
		if (fliped)
			logic_position.x -= walk_speed;
		else
			logic_position.x += walk_speed;
		// Input dependent actions
		if (!fliped && !inputs[RIGHT] || fliped && !inputs[LEFT])
			updateState(IDLE);
		else if (checkForSuper(super_window))
			updateState(ATTACKING, SUPER);
		else if (inputs[UP]) {
			velocity.y -= jump_power.y;
			if (fliped)
				velocity.x -= jump_power.x;
			else
				velocity.x += jump_power.x;
			grounded = false;
			updateState(JUMPING);
		}
		else if (inputs[DOWN])
			updateState(CROUCHING);
		else if (inputs[LIGHT_ATTACK]) 
			updateState(ATTACKING, ST_L);
		else if (inputs[HEAVY_ATTACK]) 
			updateState(ATTACKING, ST_H);
		else if (inputs[SPECIAL_1] && standingSpecial1Condition())
			updateState(ATTACKING, ST_S1);
		else if (inputs[SPECIAL_2]) 
			updateState(ATTACKING, ST_S2);
		break;

	case CROUCHING:
		// Input independent anctions
		//OneTick
		if (!state_first_tick) {
			updateAnimation(crouch);
			state_first_tick = true;
		}
		if (!crouching_hurtbox) {
			setCrouchingHurtbox(true);
		}
		// Input dependent actions
		if (hit && inputs[LEFT] && !fliped || hit && inputs[RIGHT] && fliped) {
			if (attack_recieving.block_type != BLOCK_TYPE::OVERHEAD)
				updateState(CROUCH_BLOCKING);
			else
				updateState(HIT);
		}
		else if (hit)
			updateState(HIT, NO_ATT);
		if (!inputs[DOWN]) {
			updateState(IDLE, NO_ATT);
			setCrouchingHurtbox(false);
		}
		else if (inputs[SWITCH]) {
			swapRequested = true;		//Important!
			setCrouchingHurtbox(false);
		}
		else if (inputs[UP]) {
			setCrouchingHurtbox(false);
			velocity.y -= jump_power.y; // Put in the (one_tick thing)
			grounded = false;
			updateState(JUMPING);
		}
		else if (inputs[LIGHT_ATTACK])
			updateState(ATTACKING, CR_L);
		else if (inputs[HEAVY_ATTACK])
			updateState(ATTACKING, CR_H);
		else if (inputs[SPECIAL_1]) {
			updateState(ATTACKING, CR_S1);
			setCrouchingHurtbox(false);
		}
		else if (inputs[SPECIAL_2]) {
			updateState(ATTACKING, CR_S2);
			setCrouchingHurtbox(false);
		}
		break;

	case JUMPING:
		// Input independent actions
		if (!state_first_tick) {
			updateAnimation(jump);
			playCurrentSFX();
			state_first_tick = true;
		}
		// Input dependent actions
		if (grounded)
			updateState(IDLE);
		else if (hit)
			updateState(JUGGLE);
		else if (inputs[LIGHT_ATTACK]) 
			updateState(ATTACKING, JM_L);
		else if (inputs[HEAVY_ATTACK])
			updateState(ATTACKING, JM_H);
		else if (inputs[SPECIAL_1] && jumpingSpecial1Condition())
			updateState(ATTACKING, JM_S1);
		else if (inputs[SPECIAL_2] && jumpingSpecial2Condition())
			updateState(ATTACKING, JM_S2);
		break;

	case ATTACKING:
		//One Tick
		if (!state_first_tick) {
			playCurrentSFX();
			current_super_gauge += super_gauge_gain_strike;

		}
		// Continuous
		doAttack(inputs);

		if (hit) {
			instanciated_hitbox = false;
			updateState(HIT);
			hurtbox->type = HURTBOX;
		}
		break;

	case STAND_BLOCKING:
		//Input independent actions
		// One tick
		if (!state_first_tick) {
			playCurrentSFX();
			emmitCurrentParticle();
			updateAnimation(standing_block);
			state_first_tick = true;
		}
		if (hit) {
			playCurrentSFX();
			emmitCurrentParticle();
			hit = false;
			current_super_gauge += super_gauge_gain_block;
		}
		if (SDL_GetTicks() - moment_hit > attack_recieving.blockstun)
			updateState(IDLE, NO_ATT);
		// Continuous
		if (!fliped)
			logic_position.x -= attack_recieving.pushblock;
		else
			logic_position.x += attack_recieving.pushblock;
		
		break;

	case CROUCH_BLOCKING:
		//Input independent actions
		// One tick
		if (!state_first_tick) {
			playCurrentSFX();
			emmitCurrentParticle();
			updateAnimation(crouching_block);
			state_first_tick = true;
		}
		if (hit) {
			playCurrentSFX();
			emmitCurrentParticle();
			hit = false;
			current_super_gauge += super_gauge_gain_block;
		}
		if (SDL_GetTicks() - moment_hit > attack_recieving.blockstun)
			updateState(CROUCHING);
		// Continuous
		if (!fliped)
			logic_position.x -= attack_recieving.pushblock;
		else
			logic_position.x += attack_recieving.pushblock;

		break;

	case HIT:
		//Input independent actions
		// One tick
		if (!state_first_tick) {
			updateAnimation(standing_hit);
			emmitCurrentParticle();
			state_first_tick = true;
		}
		if (hit) { 
			playCurrentSFX();
			emmitCurrentParticle();
			if (attack_recieving.knockdown)
				updateState(JUGGLE);
			else
				hit = false;
			current_life -= attack_recieving.damage;
			current_super_gauge += super_gauge_gain_hit;
		}
		else if (SDL_GetTicks() - moment_hit > attack_recieving.hitstun) {
			updateState(IDLE);
			setCrouchingHurtbox(false);
		}
		
		// Continuous
		if (!fliped)
			logic_position.x -= attack_recieving.pushhit;
		else
			logic_position.x += attack_recieving.pushhit;
	
		break;

	case JUGGLE:
		if (!state_first_tick) {
			emmitCurrentParticle();
			updateAnimation(standing_hit);
			state_first_tick = true;
		}
		if (hit) {
			playCurrentSFX();
			emmitCurrentParticle();
			if (!fliped)
				velocity.x = -attack_recieving.juggle_speed.x;
			else
				velocity.x = attack_recieving.juggle_speed.x;

			velocity.y = -attack_recieving.juggle_speed.y;
			
			current_life -= attack_recieving.damage;
			hit = false;
			grounded = false;
		}
		if (grounded)
			updateState(KNOCKDOWN);
		break;

	case KNOCKDOWN:
		//Input independent actions
		if (!state_first_tick) {
			//playCurrentSFX(); Maybe knockdown should play something?
			updateAnimation(knockdown);
			state_first_tick = true;
			hit = false;
		}
		hurtbox->active = false;
		if (current_animation->Finished()){
			makeInvencibleFor(invencibility_on_wakeup);
			updateState(IDLE);
			hurtbox->active = true;
		}
		break;
	case SWAPPING:
		//TODO: Define swapping
		//manageSwap();
		updateState(IDLE);
		break;
	case RECOVERY:
		// One tick
		if (!state_first_tick) {
			updateAnimation(idle);
			state_first_tick = true;
			setCrouchingHurtbox(false);
		}
		if (hit) 
			updateState(HIT);
		if (recovery_timer.read() > current_recovery)
			updateState(IDLE);
		break;
	case DEAD:
		if (!state_first_tick) {
			updateAnimation(dead);
			playCurrentSFX();
			state_first_tick = true;
		}
		break;

	}

	// PROVISIONAL: (?)
	if (logic_position.x < App->render->camera.x + lateral_limit) {
		if (!App->entities->moveCamera(true))
			logic_position.x = App->render->camera.x + lateral_limit;
		else if(logic_position.x < App->render->camera.x + lateral_limit)
			logic_position.x = App->render->camera.x + lateral_limit;
	}
	if (logic_position.x > App->render->camera.x + App->render->camera.w - lateral_limit) {
		if (!App->entities->moveCamera(false))
			logic_position.x = App->render->camera.x + App->render->camera.w - lateral_limit;
		else if (logic_position.x > App->render->camera.x + App->render->camera.w - lateral_limit)
			logic_position.x = App->render->camera.x + App->render->camera.w - lateral_limit;

	}

	manageGroundPosition();

	// State independent actions
	updateInvecibility();

	// Calculate draw position out of logic position
	draw_position.x = calculateDrawPosition(0, draw_size.x* scale, true);
	draw_position.y = calculateDrawPosition(0, draw_size.y * scale, false);

	// Hurtbox and pushbox allways next to the player
	int offset;
	if (crouching_hurtbox)
		offset = crouching_hurtbox_offset;
	else
		offset = 0;

	hurtbox->SetPos(calculateDrawPosition(0, hurtbox->rect.w, true), calculateDrawPosition(offset, hurtbox->rect.h, false));
	pushbox->SetPos(calculateDrawPosition(0, pushbox->rect.w, true), calculateDrawPosition(crouching_hurtbox_offset, pushbox->rect.h, false));

	// Gauge Limit
	if (current_super_gauge > max_super_gauge)
		current_super_gauge = max_super_gauge;

	if (!grounded) 		{
		applyGravity();
		setIfGrounded();
	}

	characterSpecificUpdates();
	// Delete out of life colliders
	deleteDeadHitboxes();
}

void Character::onCollision(collider* c1, collider* c2) {

	if ((c1->type == HURTBOX || c1->type == PROJECTILE_INVENCIBLE_HURTBOX) && (c2->type == HITBOX || c2->type == PROJECTILE_HITBOX)) {
		attack_recieving = c2->character->getAttackData(c2->attack_type); 
		hit = true;
		moment_hit = SDL_GetTicks();
		deleteAllMeleeHitboxes(); // When you get hit all your melee  hitboxes are deleted
	}
	else if (c1->type == PUSHBOX && c2->type == PUSHBOX && !App->entities->paused) { 
		if (!fliped)
			logic_position.x -= walk_speed;
		else
			logic_position.x += walk_speed;
	}
	else if ((c1->type == HITBOX || c1->type == PROJECTILE_HITBOX) && (c2->type == HURTBOX || c2->type == PROJECTILE_INVENCIBLE_HURTBOX)) {
		// Allways delete hitboxes on collision
		deleteAttackHitbox(c1->attack_type, c1);
	}
	
}
void Character::applyGravity() {
	if(current_state != JUGGLE)
		velocity.y += gravity; // Maybe it shuld apply other gravity on juggle state
	else
		velocity.y += 1.0;

	if (velocity.y > 0) {
		if (logic_position.y < ground_position)
			logic_position.y += velocity.y;
			logic_position.x += velocity.x;
	}
	else {
		logic_position.y += velocity.y;
		logic_position.x += velocity.x;
	}
}

void Character::setIfGrounded() {

	if (logic_position.y >= ground_position)
	{ 
		grounded = true;
		logic_position.y = ground_position;
		velocity.y = 0;
		velocity.x = 0;
	}
}

void Character::draw(SDL_Texture* graphic){
	if (current_animation != nullptr){ 
		int hardcoded_offset = 0;
		if (fliped)
			hardcoded_offset = 15;
		else
			hardcoded_offset = -15;
		App->render->drawSprite(3, graphic, logic_position.x - ((shadow_rect.w/2)*scale) + hardcoded_offset, ground_position + shadow_offset, &shadow_rect, scale, fliped, 1.0f,0);
		App->render->drawSprite(4,graphic, draw_position.x, draw_position.y, &current_animation->GetCurrentFrame(),scale, fliped, 1.0f, current_animation->angle);
	}

}

bool Character::manageSwap()
{
	if (partner == nullptr)
		return false;

	swapDone = false;
	velocity.y = -40;
	applyGravity();

	if (swapRequested) {
		partner->getCurrCharacter()->current_state = SWAPPING;
		swapRequested = false;
	}

	if (logic_position.y < -200) { //margin to make it go out of the screen
		readyToSwap = true;
		if (partner->getCurrCharacter()->readyToSwap) {
			updateState(JUMPING);
			grounded = false;
			if (lane == 1)
				lane = 2;
			else
				lane = 1;

			swapDone = true;

			if (partner->getCurrCharacter()->swapDone)
				readyToSwap = false;
			partner->getCurrCharacter()->readyToSwap = false;
		}
	}
		
	return true;
}

void Character::manageOponent()
{
	for (int i = 0; i < 4; i++) {
		Player* curr_player = nullptr;

		curr_player = App->entities->players[i];

		if (curr_player == nullptr)
			continue;

		if (curr_player->getCurrCharacter() == this)
			continue;

		if (curr_player->getLane() == this->lane) {
			oponent = curr_player->getCurrCharacter();
		}

	}
}

void Character::doAttack(const bool(&inputs)[MAX_INPUTS]) {
	switch (attack_doing) {
	case ST_L:
		if (!state_first_tick) {
			updateAnimation(light_attack);
			state_first_tick = true;
		}
		if (current_animation->Finished()) 			{
			askRecovery(st_l.recovery);
			instanciated_hitbox = false;
		}
		else if (current_animation->GetState() == ACTIVE && !instanciated_hitbox) {
			instanciateHitbox(ST_L);
			manageCancel(inputs);
		}
		break;
	case ST_H:
		if (!state_first_tick) {
			updateAnimation(heavy_attack);
			state_first_tick = true;
		}
		if (current_animation->Finished()) {
			askRecovery(st_h.recovery);
			instanciated_hitbox = false;
		}
		else if (current_animation->GetState() == ACTIVE && !instanciated_hitbox) {
			instanciateHitbox(ST_H);
			manageCancel(inputs);
		}
		break;
	case CR_L:
		if (!state_first_tick) {
			updateAnimation(crouching_light);
			state_first_tick = true;
		}
		if (current_animation->Finished()) {
			askRecovery(cr_l.recovery);
			instanciated_hitbox = false;
		}
		else if (current_animation->GetState() == ACTIVE && !instanciated_hitbox) {
			instanciateHitbox(CR_L);
			manageCancel(inputs);
		}
		break;
	case CR_H: 
		if (!state_first_tick) {
			updateAnimation(crouching_heavy);
			state_first_tick = true;
		}
		if (current_animation->Finished()) {
			askRecovery(cr_h.recovery);
			instanciated_hitbox = false;
		}
		else if (current_animation->GetState() == ACTIVE && !instanciated_hitbox) {
			instanciateHitbox(CR_H);
			manageCancel(inputs);
		}
		break;
	case JM_L:
		if (!state_first_tick) {
			updateAnimation(jumping_light);
			state_first_tick = true;
		}
		if (grounded) {
			instanciated_hitbox = false;
			collider* hitbox = getCurrentAttackHitbox();
			if(hitbox != nullptr){ // Just for safety
				deleteAttackHitbox(JM_L);
			}
			askRecovery(jm_l.recovery);
		}	
		else if (current_animation->GetState() == ACTIVE && !instanciated_hitbox)
			instanciateHitbox(JM_L);	
		{
			collider* hitbox = getCurrentAttackHitbox();
			// Set the hitbox to follow the player
			if (hitbox != nullptr) 			{
				hitbox->SetPos(calculateDrawPosition(jm_l.pos_rel_char.x, jm_l.hitbox.w, true), calculateDrawPosition(jm_l.pos_rel_char.y, jm_l.hitbox.h, false));
			}
		}
		break;
	case JM_H:
		if (!state_first_tick) {
			updateAnimation(jumping_heavy);
			state_first_tick = true;
		}
		if (grounded) {
			instanciated_hitbox = false;
			collider* hitbox = getCurrentAttackHitbox();
			if (hitbox != nullptr){  // Just for safety
				deleteAttackHitbox(JM_H);
			}
			askRecovery(jm_h.recovery);
		}
		else if (current_animation->GetState() == ACTIVE && !instanciated_hitbox) {
			instanciateHitbox(JM_H);
		}	
		{
			// Set the hitbox to follow the player
			collider* hitbox = getCurrentAttackHitbox();
			if (hitbox != nullptr) {
				hitbox->SetPos(calculateDrawPosition(jm_h.pos_rel_char.x, jm_h.hitbox.w, true), calculateDrawPosition(jm_h.pos_rel_char.y, jm_h.hitbox.h, false));
			}
		}
		break;
	case ST_S1:
		if (!state_first_tick) {
			updateAnimation(standing_special1);
			state_first_tick = true;
		}
		standingSpecial1(inputs); 
		break;
	case ST_S2:
		if (!state_first_tick) {
			updateAnimation(standing_special2);
			state_first_tick = true;
		}
		standingSpecial2(inputs);
		break;
	case CR_S1:
		if (!state_first_tick) {
			updateAnimation(crouching_special1);
			state_first_tick = true;
		}
		crouchingSpecial1();
		break;
	case CR_S2:
		crouchingSpecial2();
		break;
	case JM_S1:
		jumpingSpecial1(inputs);
		break;
	case JM_S2:
		jumpingSpecial2(inputs);
		break;
	case SUPER:
		doSuper();
		break;
	default:
		current_state = IDLE;
		break;
	}
}

void Character::updateAnimation(Animation & new_animation) {
	if (current_animation != &new_animation) {
		new_animation.Reset();
		current_animation = &new_animation;
	}
}

void Character::instanciateHitbox(CHAR_ATT_TYPE type) 	{
	SDL_Rect collider;
	int life;
	switch (type) 		{
		case ST_L:
			collider = { calculateDrawPosition(st_l.pos_rel_char.x,st_l.hitbox.w, true), calculateDrawPosition(st_l.pos_rel_char.y,st_l.hitbox.h, false),st_l.hitbox.w, st_l.hitbox.h };
			life = st_l.active_time;
			break;
		case ST_H:
			collider = { calculateDrawPosition(st_h.pos_rel_char.x,st_h.hitbox.w, true), calculateDrawPosition(st_h.pos_rel_char.y,st_h.hitbox.h, false),st_h.hitbox.w, st_h.hitbox.h };
			life = st_h.active_time;
			break;
		case CR_L:
			collider = { calculateDrawPosition(cr_l.pos_rel_char.x,cr_l.hitbox.w, true), calculateDrawPosition(cr_l.pos_rel_char.y,cr_l.hitbox.h, false),cr_l.hitbox.w, cr_l.hitbox.h };
			life = cr_l.active_time;
			break;
		case CR_H:
			collider = { calculateDrawPosition(cr_h.pos_rel_char.x,cr_h.hitbox.w, true), calculateDrawPosition(cr_h.pos_rel_char.y,cr_h.hitbox.h, false),cr_h.hitbox.w, cr_h.hitbox.h };
			life = cr_h.active_time;
			break;
		case JM_L:
			collider = { calculateDrawPosition(jm_l.pos_rel_char.x,jm_l.hitbox.w, true), calculateDrawPosition(jm_l.pos_rel_char.y,jm_l.hitbox.h, false),jm_l.hitbox.w, jm_l.hitbox.h };
			life = jm_l.active_time;
			break;
		case JM_H:
			collider = { calculateDrawPosition(jm_h.pos_rel_char.x,jm_h.hitbox.w, true), calculateDrawPosition(jm_h.pos_rel_char.y,jm_h.hitbox.h, false),jm_h.hitbox.w, jm_h.hitbox.h };
			life = jm_h.active_time;
			break;
		case ST_S2:
			collider = { calculateDrawPosition(st_s2.pos_rel_char.x, st_s2.hitbox.w, true), calculateDrawPosition(st_s2.pos_rel_char.y, st_s2.hitbox.h, false), st_s2.hitbox.w, st_s2.hitbox.h };
			life = st_s2.active_time;
			break;
		case CR_S1:
			collider = { calculateDrawPosition(cr_s1.pos_rel_char.x, cr_s1.hitbox.w, true), calculateDrawPosition(cr_s1.pos_rel_char.y, cr_s1.hitbox.h, false), cr_s1.hitbox.w, cr_s1.hitbox.h };
			life = cr_s1.active_time;
			break;
		case CR_S2:
			collider = { calculateDrawPosition(cr_s2.pos_rel_char.x, cr_s2.hitbox.w, true), calculateDrawPosition(cr_s2.pos_rel_char.y, cr_s2.hitbox.h, false), cr_s2.hitbox.w, cr_s2.hitbox.h };
			life = cr_s2.active_time;
			break;
		case JM_S1:
			collider = { calculateDrawPosition(jm_s1.pos_rel_char.x, jm_s1.hitbox.w, true), calculateDrawPosition(jm_s1.pos_rel_char.y, jm_s1.hitbox.h, false), jm_s1.hitbox.w, jm_s1.hitbox.h };
			life = jm_s1.active_time;
			break;
		case JM_S2:
			collider = { calculateDrawPosition(jm_s2.pos_rel_char.x, jm_s2.hitbox.w, true), calculateDrawPosition(jm_s2.pos_rel_char.y, jm_s2.hitbox.h, false), jm_s2.hitbox.w, jm_s2.hitbox.h };
			life = jm_s2.active_time;
	}
	hitboxes.push_back(App->collision->AddCollider(collider, HITBOX,life ,type, App->entities, this));
	instanciated_hitbox = true;
}


void Character::manageGroundPosition() {
	if (lane == 1)
		ground_position = bottom_lane;
	if (lane == 2)
		ground_position = upper_lane;
}



basic_attack_deff Character::getAttackData(CHAR_ATT_TYPE attack_type) const {
	switch (attack_type) {
		case ST_L:
			return st_l;
			break;
		case ST_H:
			return st_h;
			break;
		case CR_L:
			return cr_l;
			break;
		case CR_H:
			return cr_h;
			break;
		case JM_L:
			return jm_l;
			break;
		case JM_H:
			return jm_h;
			break;
		case ST_S1:
			return st_s1;
			break;
		case ST_S2:
			return st_s2;
			break;
		case CR_S1:
			return cr_s1;
			break;
		case CR_S2:
			return cr_s2;
			break;
		case JM_S1:
			return jm_s1;
			break;
		case JM_S2:
			return jm_s2;
			break;
		case SUPER:
			return super;
		case NO_ATT:
			LOG("FATAL ERROR");
			break;
	}
}

int Character::calculateDrawPosition(int offset, int size, bool x) 	{
	if (x) {
		if (!fliped)
			return logic_position.x + offset - size / 2;
		else
			return logic_position.x - offset - size / 2;
	}
	else
		return logic_position.y + offset - size/2;
}

iPoint Character::getPos() const{
	return { (int)logic_position.x, (int)logic_position.y };
}
int Character::getLateralLimit() const {
	return lateral_limit;
}
void Character::setFlip(bool flip) {
	fliped = flip;
}
void Character::updateInvecibility() {
	if(invencible_timer.isActive())
	{ 
		if (invencible_timer.read() > stop_invencibility) {
			hurtbox->active = true;
			invencible_timer.stop();
		}
		else
			hurtbox->active = false;
	}
}
void Character::makeInvencibleFor(int time_invencible) {
	invencible_timer.start();
	stop_invencibility = time_invencible;
}

int Character::getCurrentLife() const{
	
	return current_life;
}

int Character::getMaxLife() const{

	return max_life;
}

int Character::getCurrentSuperGauge() const{

	return current_super_gauge;
}

int Character::getMaxSuperGauge() const{

	return max_super_gauge;
}

bool Character::notAllowFlip() {
	if (current_state != ATTACKING)
		return false;
	for (std::list<CHAR_ATT_TYPE>::iterator it = non_flip_attacks.begin(); it != non_flip_attacks.end(); ++it) {
		CHAR_ATT_TYPE att = *it;
		if (att == attack_doing) 
			return true;
	}
	return false;
}

CHAR_TYPE Character::getType() const {
	return type;
}
void Character::resetCharacter()	{
	current_life = max_life;
	current_state = CHAR_STATE::IDLE;
	logic_position.x = starting_position.x;
	logic_position.y = bottom_lane;
	hurtbox->type = HURTBOX;
	hurtbox->active = true;
	death = false;
	hit = false;
	state_first_tick = false;
	current_super_gauge = 0;
	App->scene_manager->current_time = App->scene_manager->max_time;	//This should be done from the scene manager
	App->entities->paused = false;
	App->render->camera.x = (App->render->resolution.first - App->render->camera.w) / 2; //This should be done from the scene manager
	velocity.x = velocity.y = 0;//This should be done from the scene manager
	instanciated_hitbox = false;

}
void Character::deleteDeadHitboxes() 	{
	// Compute what hitboxes need to be deleted

	std::list<collider*> hitboxes_to_delete;

	for (std::list<collider*>::iterator it = hitboxes.begin(); it != hitboxes.end(); ++it) {
		collider* c = *it;
		if (c->life != -1 && SDL_GetTicks() - c->born > c->life){
			c->to_delete = true;
			hitboxes_to_delete.push_back(c);
		}
	}

	// Remove the colliders
	for (std::list<collider*>::iterator it = hitboxes_to_delete.begin(); it != hitboxes_to_delete.end(); ++it) {
		collider* c = *it;
		hitboxes.remove(c);
	}

	hitboxes_to_delete.clear();
}

collider* Character::getCurrentAttackHitbox() 	{
	for (std::list<collider*>::iterator it = hitboxes.begin(); it != hitboxes.end(); ++it) {
		collider* c = *it;
		if (c->attack_type == attack_doing){
			return c;
		}
	}
	return nullptr;
}
void Character::deleteAttackHitbox(CHAR_ATT_TYPE type, collider* hitbox) 	{
	// Compute what hitboxes need to be deleted

	std::list<collider*> hitboxes_to_delete;

	for (std::list<collider*>::iterator it = hitboxes.begin(); it != hitboxes.end(); ++it) {
		collider* c = *it;
		if (c->attack_type == type && (c == hitbox || hitbox ==nullptr)) {
			c->to_delete = true;
			hitboxes_to_delete.push_back(c);
		}
	}
	// Remove the colliders
	for (std::list<collider*>::iterator it = hitboxes_to_delete.begin(); it != hitboxes_to_delete.end(); ++it) {
		collider* c = *it;
		hitboxes.remove(c);
	}

	hitboxes_to_delete.clear();
}
void Character::deleteAllMeleeHitboxes() {
	// Compute what hitboxes need to be deleted

	std::list<collider*> hitboxes_to_delete;

	for (std::list<collider*>::iterator it = hitboxes.begin(); it != hitboxes.end(); ++it) {
		collider* c = *it;
		if(c->type != PROJECTILE_HITBOX){ 
		c->to_delete = true;
		hitboxes_to_delete.push_back(c);
		}
	}
	// Remove the colliders
	for (std::list<collider*>::iterator it = hitboxes_to_delete.begin(); it != hitboxes_to_delete.end(); ++it) {
		collider* c = *it;
		hitboxes.remove(c);
	}

	hitboxes_to_delete.clear();
}

CHAR_ATT_TYPE Character::getAttackDoing() const{
	return attack_doing;
}
CHAR_STATE Character::getCurrentState() const{
	return current_state;
}

void Character::askRecovery(int recovery) 	{
	current_state = RECOVERY;
	attack_doing = NO_ATT;
	state_first_tick = false;
	recovery_timer.start();
	current_recovery = recovery;
}

void Character::setCrouchingHurtbox(bool crouch) {
	if (crouch) {
		hurtbox->rect.h /= 2;
		crouching_hurtbox = true;
	}
	else {
		hurtbox->rect.h = standing_hurtbox_size.y;
		crouching_hurtbox = false;
	}

}

void Character::updateState(CHAR_STATE state, CHAR_ATT_TYPE attack) {
	current_state = state;
	attack_doing = attack;
	state_first_tick = false;
}
void Character::playCurrentSFX() {
	switch (current_state) {
	case JUMPING:
		App->audio->playSFX(s_jump);
	case ATTACKING:
		switch (attack_doing) {
		case ST_L:
		case JM_L:
		case CR_L:
			App->audio->playSFX(s_light_sword_whiff);
			break;
		case ST_S1:
			App->audio->playSFX(s_standing_special_1);
			break;
		case JM_S1:
		case JM_S2:
			App->audio->playSFX(s_jumping_special_1);
			break;
		case CR_S1:
			App->audio->playSFX(s_crouching_special_1);
			break;
		case CR_S2:
			App->audio->playSFX(s_crouching_special_2);
			break;
		case ST_H:
		case CR_H:
		case JM_H:
			App->audio->playSFX(s_heavy_sword_whiff);
			break;
		case ST_S2:
			App->audio->playSFX(s_standing_special_2);
			break;
		case SUPER:
			App->audio->playSFX(s_super);
			break;
		}
		break;
	case CROUCH_BLOCKING:
	case STAND_BLOCKING:
		switch (attack_recieving.type) {
		case ST_L:
		case JM_L:
		case CR_L:
		case ST_S1:
			App->audio->playSFX(s_light_sword_block);
			break;
		case ST_H:
		case CR_H:
		case JM_H: 
		case ST_S2: 
		case CR_S1:
		case CR_S2:
			App->audio->playSFX(s_heavy_sword_block);
			break;
		}
		break;
	case HIT:
	case JUGGLE:
		switch (attack_recieving.type) {
		case ST_L:
		case JM_L:
		case CR_L:
		case ST_S1:
			App->audio->playSFX(s_light_sword_impact);
			break;
		case ST_H:
		case CR_H:
		case JM_H:
		case ST_S2:
		case CR_S1:
		case CR_S2:
		case JM_S1:
		case JM_S2:
			App->audio->playSFX(s_heavy_sword_impact);
			break;
		}
		break;
	case KNOCKDOWN:
		break;
	case SWAPPING:
		break;
	case DEAD:
		App->audio->playSFX(s_man_death);
		break;
	}

}
void Character::fillBuffer(const bool(&inputs)[MAX_INPUTS]) {
	bool button_pressed = false;
	for (int i = 0; i < MAX_INPUTS; i++) {
		if (inputs[i]){
			pushIntoBuffer((CHARACTER_INPUTS)i);
			button_pressed = true;
		}
	}
	if (!button_pressed) {
		pushIntoBuffer(CHARACTER_INPUTS::NULL_INPUT);
	}
}
void Character::pushIntoBuffer(CHARACTER_INPUTS input) {
	
	for (int i = 0; i < MAX_INPUT_BUFFER; i++) {
		input_buffer[i] = input_buffer[i + 1];
	}
	input_buffer[MAX_INPUT_BUFFER - 1] = input;
}
bool Character::lookInBuffer(CHARACTER_INPUTS input, int window) {
	for (int i = MAX_INPUT_BUFFER -1 -window; i < MAX_INPUT_BUFFER; i++) {
		if (input_buffer[i] == input)
			return true;
	}
	return false;
}

void Character::manageCancel(const bool(&inputs)[MAX_INPUTS]) {
	if (checkForSuper(super_window)) {
		updateState(ATTACKING, SUPER);
		instanciated_hitbox = false;
	}
	else if (lookInBuffer(SPECIAL_1, cancelability_window) && !inputs[DOWN] && standingSpecial1Condition() /*&& !projectile*//* !projectile*/) {
		updateState(ATTACKING, ST_S1);
		instanciated_hitbox = false;
	}
	else if (lookInBuffer(SPECIAL_2, cancelability_window) && !inputs[DOWN]) {
		updateState(ATTACKING, ST_S2);
		instanciated_hitbox = false;
	}
	else if (lookInBuffer(SPECIAL_1, cancelability_window) && inputs[DOWN]) {
		updateState(ATTACKING, CR_S1);
		instanciated_hitbox = false;
	}
	else if (lookInBuffer(SPECIAL_2, cancelability_window) && inputs[DOWN]) {
		updateState(ATTACKING, CR_S2);
		instanciated_hitbox = false;
	}
}
bool Character::checkForSuper(int window) {
	if (current_super_gauge < max_super_gauge)
		return false;

	int counter = 0;
	for (int i = MAX_INPUT_BUFFER - 1 - window; i < MAX_INPUT_BUFFER; i++) {
		if (input_buffer[i] == DOWN && counter == 0)
			counter++;
		if (!fliped) {
			if (input_buffer[i] == RIGHT && counter == 1)
				counter++;
		}
		else {
			if (input_buffer[i] == LEFT && counter == 1)
				counter++;
		}
		if (input_buffer[i] == SPECIAL_1 && counter == 2)
			counter++;
	}
	if (counter == 3)
		return true;
	else
		return false;
}
void Character::emmitCurrentParticle() {
	int offset_x = 80;
	int offset_y = 380;
	switch (attack_recieving.type) {
		case ST_L:
		case CR_L:
		case JM_L:
		case ST_S1:
			switch (current_state) {
			case STAND_BLOCKING:
				offset_x = 0;
				offset_y = 0;
				if (fliped)
					App->particle_system->createEmitter({ (float)logic_position.x - offset_x,(float)logic_position.y - offset_y }, "particles/heavy-block.xml");
				else
					App->particle_system->createEmitter({ (float)logic_position.x + offset_x,(float)logic_position.y - offset_y }, "particles/heavy-block.xml");
				break;
			case CROUCH_BLOCKING:
				offset_x = 0;
				offset_y = 0;
				if (fliped)
					App->particle_system->createEmitter({ (float)logic_position.x - offset_x,(float)logic_position.y - offset_y }, "particles/heavy-block.xml");
				else
					App->particle_system->createEmitter({ (float)logic_position.x + offset_x,(float)logic_position.y - offset_y }, "particles/heavy-block.xml");
				break;
			case HIT:
				offset_x = 10;
				offset_y = 0;
				if (fliped)
					App->particle_system->createEmitter({ (float)logic_position.x - offset_x,(float)logic_position.y - offset_y }, "particles/hit-light-l.xml");
				else{
					App->particle_system->createEmitter({ (float)logic_position.x + offset_x,(float)logic_position.y - offset_y }, "particles/hit-light-r.xml");

				}
			case JUGGLE:
				App->particle_system->createEmitter({ (float)logic_position.x,(float)logic_position.y + crouch_particle_offset }, "particles/hit-light-r.xml");
				break;
			}

			break;
		case ST_H:
		case CR_H:
		case JM_H:
		case ST_S2:
		case CR_S2:
		case CR_S1:
			switch (current_state) {
			case STAND_BLOCKING:
				App->particle_system->createEmitter({ (float)logic_position.x,(float)logic_position.y }, "particles/heavy-block.xml");
				break;
			case CROUCH_BLOCKING:
				App->particle_system->createEmitter({ (float)logic_position.x,(float)logic_position.y + crouch_particle_offset }, "particles/heavy-block.xml");
				break;
			case HIT:
				offset_x = 10;
				offset_y = 0;
				if (fliped)
					App->particle_system->createEmitter({ (float)logic_position.x - offset_x,(float)logic_position.y - offset_y }, "particles/hit-light-r.xml");
				else {
					App->particle_system->createEmitter({ (float)logic_position.x + offset_x,(float)logic_position.y - offset_y }, "particles/hit-light-r.xml");

				}
				break;
			case JUGGLE:
				App->particle_system->createEmitter({ (float)logic_position.x,(float)logic_position.y + crouch_particle_offset }, "particles/hit-light-r.xml");
				break;
			}
			break;
	}
}

