#include "Character.h"
#include "ProjDefs.h"
#include "DebLog.h"
#include "Player.h"
#include "mdCollision.h"
#include "mdAudio.h"
#include "mdInput.h"
#include "mdSceneManager.h"
#include "mdParticleSystem.h"
#include "mdEntities.h"

Character::Character(character_deff character, int x_pos, int _fliped, int skin) {
	// Constructor inicialization
	fliped = _fliped;
	logic_position.x = x_pos;
	skin_id = skin;
	type = character.type;
	// Basic attack definitions
	st_l = character.st_l;
	st_h = character.st_h;
	cr_l = character.cr_l;
	cr_h = character.cr_h;
	jm_l = character.jm_l;
	jm_h = character.jm_h;
	st_s1 = character.st_s1;
	st_s2 = character.st_s2;
	cr_s1 = character.cr_s1;
	cr_s2 = character.cr_s2;
	jm_s1 = character.jm_s1;
	jm_s2 = character.jm_s2;
	super = character.super;
	// XML inicialization
	draw_size.x = 195;
	draw_size.y = 158;
	max_life = character.max_life;
	max_super_gauge = character.max_super_gauge;
	super_window = character.super_window;
	cancelability_window = character.cancelability_window;
	super_gauge_gain_hit = character.super_gauge_gain_hit;
	super_gauge_gain_block = character.super_gauge_gain_block;
	super_gauge_gain_strike = character.super_gauge_gain_strike;
	crouching_hurtbox_offset = character.crouching_hurtbox_offset;
	standing_hurtbox_size = character.standing_hurtbox_size;
	jump_power = character.jump_power;
	walk_speed = character.walk_speed;
	gravity = character.gravity;
	invencibility_on_wakeup = character.invencibility_on_wakeup;
	scale = character.scale;
	non_flip_attacks = character.non_flip_attacks;
	crouching_hurtbox_attacks = character.crouching_hurtbox_attacks;
	normal_taunt_duration = 1500;
	shadow_rect = { 452, 3719, 68, 14 };
	shadow_offset = 105;
	// Runtime inicialization
	grounded = true;
	instanciated_hitbox = false;
	hit = false;
	crouching_hurtbox = false;
	death = false;
	current_life = max_life;
	current_super_gauge = 0;
	velocity.y = 0;
	velocity.x = 0;
	current_state = CHAR_STATE::IDLE;
	logic_position.y = 800;
	starting_position.x = logic_position.x;
	starting_position.y = -1000;
	state_first_tick = false;
	taunt_start = 0;
	taunt_duration = 0;
	combo_counter = 0;
	// Others
	ground_position = 800;
	lateral_limit = 50;

	// Sound effects
	s_jump					= character.sfxs[CHAR_SOUNDS::S_JUMP];
	s_light_sword_block		= character.sfxs[CHAR_SOUNDS::S_LIGHT_SWORD_BLOCK];
	s_heavy_sword_block		= character.sfxs[CHAR_SOUNDS::S_HEAVY_SWORD_BLOCK];
	s_light_sword_whiff		= character.sfxs[CHAR_SOUNDS::S_LIGHT_SWORD_WHIFF];
	s_heavy_sword_whiff		= character.sfxs[CHAR_SOUNDS::S_HEAVY_SWORD_WHIFF];
	s_light_sword_impact	= character.sfxs[CHAR_SOUNDS::S_LIGHT_SWORD_IMPACT];
	s_heavy_sword_impact	= character.sfxs[CHAR_SOUNDS::S_HEAVY_SWORD_IMPACT];
	s_standing_special_1	= character.sfxs[CHAR_SOUNDS::S_STANDING_SPECIAL_1];
	s_standing_special_2	= character.sfxs[CHAR_SOUNDS::S_STANDING_SPECIAL_2];
	s_jumping_special_1		= character.sfxs[CHAR_SOUNDS::S_JUMPING_SPECIAL_1];
	s_crouching_special_1	= character.sfxs[CHAR_SOUNDS::S_CROUCHING_SPECIAL_1];
	s_crouching_special_2	= character.sfxs[CHAR_SOUNDS::S_CROUCHING_SPECIAL_2];
	s_death					= character.sfxs[CHAR_SOUNDS::S_DEATH];
	s_super					= character.sfxs[CHAR_SOUNDS::S_SUPER];

	current_animation = &idle;
	current_state = IDLE;

	// Collider creation
	hurtbox = App->collision->AddCollider({ 0, 0, standing_hurtbox_size.x, standing_hurtbox_size.y }, HURTBOX, -1, basic_attack_deff(), (Character*)this);
	pushbox = App->collision->AddCollider({ 0, 0, standing_hurtbox_size.x, standing_hurtbox_size.y / 2 }, PUSHBOX, -1, basic_attack_deff(), (Character*)this);

	draw_position.x = calculateDrawPosition(0, draw_size.x* scale, true);
	draw_position.y = calculateDrawPosition(0, draw_size.y * scale, false);
}


Character::~Character() {
	deleteAllHitboxes();
}

void Character::update(const bool(&inputs)[MAX_INPUTS]) {

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
		else if (inputs[GRAB])
			tauntFor(normal_taunt_duration);

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
		// Input dependent actions
		if (hit && inputs[LEFT] && !fliped || hit && inputs[RIGHT] && fliped) {
			if (attack_recieving.block_type != BLOCK_TYPE::OVERHEAD)
				updateState(CROUCH_BLOCKING);
			else
				updateState(HIT);
		}
		else if (hit)
			updateState(HIT, NO_ATT);

		if (!inputs[DOWN]) 
			updateState(IDLE, NO_ATT);

		else if (inputs[UP]) {
			velocity.y -= jump_power.y; // Put in the (one_tick thing)
			grounded = false;
			updateState(JUMPING);
		}
		else if (inputs[LIGHT_ATTACK])
			updateState(ATTACKING, CR_L);
		else if (inputs[HEAVY_ATTACK])
			updateState(ATTACKING, CR_H);
		else if (inputs[SPECIAL_1]) 
			updateState(ATTACKING, CR_S1);
		else if (inputs[SPECIAL_2])
			updateState(ATTACKING, CR_S2);
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
			emmitCurrentParticle();
			if (attack_recieving.knockdown)
				updateState(JUGGLE);
			else {
				App->delayFrame(attack_recieving.frame_delay);
				playCurrentSFX();
				hit = false;
				combo_counter++;
			}
			current_life -= attack_recieving.damage;
			current_super_gauge += super_gauge_gain_hit;
		}
		else if (SDL_GetTicks() - moment_hit > attack_recieving.hitstun){
			updateState(IDLE);
			combo_counter = 0;
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
			App->delayFrame(attack_recieving.frame_delay);
			emmitCurrentParticle();
			juggle_attacks_recieved.push_back(attack_recieving.type);
			iPoint current_juggle_speed = { 0,0 };
			if (!juggleLimit(attack_recieving.type)) {
				if (!fliped)
					current_juggle_speed.x = -attack_recieving.juggle_speed.x;
				else
					current_juggle_speed.x = attack_recieving.juggle_speed.x;

				current_juggle_speed.y = -attack_recieving.juggle_speed.y;
			}

			velocity.create((float)current_juggle_speed.x, (float)current_juggle_speed.y);
			
			current_life -= attack_recieving.damage;
			hit = false;
			combo_counter++;
			grounded = false;
		}
		if (grounded){
			juggle_attacks_recieved.clear();
			updateState(KNOCKDOWN);
			combo_counter = 0;
		}
		break;
	case KNOCKDOWN:
		//Input independent actions
		if (!state_first_tick) {
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
	case STOPPED:
		if (!state_first_tick) {
			updateAnimation(idle);
			state_first_tick = true;
		}
		break;
	case RECOVERY:
		// One tick
		if (!state_first_tick) {
			updateAnimation(idle);
			state_first_tick = true;
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
	case TAUNT:
		if (!state_first_tick) {
			updateAnimation(taunt);
			playCurrentSFX();
			state_first_tick = true;
		}
		if (taunt_duration < (SDL_GetTicks() - taunt_start))
			updateState(IDLE);
		else if (hit)
			updateState(HIT);
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

	// Calculate collider position
	hurtbox->SetPos(calculateDrawPosition(0, hurtbox->rect.w, true), calculateDrawPosition(offset, hurtbox->rect.h, false));
	pushbox->SetPos(calculateDrawPosition(0, pushbox->rect.w, true), calculateDrawPosition(crouching_hurtbox_offset, pushbox->rect.h, false));

	// Make sure gauge is not exceded
	if (current_super_gauge > max_super_gauge)
		current_super_gauge = max_super_gauge;

	// Make sure gravity is applyed
	if (!grounded) 		{
		applyGravity();
		setIfGrounded();
	}
	// Crouching/Standing hurtbox
	hurtboxSizeManagement();


	characterSpecificUpdates();
	deleteDeadHitboxes();
}

void Character::onCollision(collider* c1, collider* c2) {

	if ((c1->type == HURTBOX || c1->type == PROJECTILE_INVENCIBLE_HURTBOX) && (c2->type == HITBOX || c2->type == PROJECTILE_HITBOX)) {
		attack_recieving = c2->attack_type; 
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
		if (current_super_gauge != max_super_gauge)
			current_super_gauge += super_gauge_gain_strike;

		deleteAttackHitbox(c1->attack_type.type, c1);
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
			instanciateHitbox(st_l);
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
			instanciateHitbox(st_h);
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
			instanciateHitbox(cr_l);
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
			instanciateHitbox(cr_h);
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
			instanciateHitbox(jm_l);	
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
			instanciateHitbox(jm_h);
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

void Character::instanciateHitbox(basic_attack_deff attack) 	{
	SDL_Rect collider;
	int life;

	collider = { calculateDrawPosition(attack.pos_rel_char.x,attack.hitbox.w, true), calculateDrawPosition(attack.pos_rel_char.y,attack.hitbox.h, false),attack.hitbox.w, attack.hitbox.h };
	life = attack.active_time;
			
	hitboxes.push_back(App->collision->AddCollider(collider, HITBOX,life ,attack, this));
	instanciated_hitbox = true;
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
	logic_position.y = ground_position;
	hurtbox->type = HURTBOX;
	hurtbox->active = true;
	pushbox->active = true;
	death = false;
	hit = false;
	state_first_tick = false;
	current_super_gauge = 0;
	velocity.x = velocity.y = 0;//This should be done from the scene manager
	instanciated_hitbox = false;
	crouching_hurtbox = false;
	juggle_attacks_recieved.clear();
	deleteAllHitboxes();

	specificCharacterReset();


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
		if (c->attack_type.type == attack_doing){
			return c;
		}
	}
	return nullptr;
}
collider* Character::getAttackHitbox(CHAR_ATT_TYPE type) {
	for (std::list<collider*>::iterator it = hitboxes.begin(); it != hitboxes.end(); ++it) {
		collider* c = *it;
		if (c->attack_type.type == type) {
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
		if (c->attack_type.type == type && (c == hitbox || hitbox ==nullptr)) {
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
		hurtbox->rect.h = standing_hurtbox_size.y / 2;
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
	case STOPPED:
		break;
	case DEAD:
		App->audio->playSFX(s_death);
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
	else if (lookInBuffer(SPECIAL_1, cancelability_window) && !inputs[DOWN] && standingSpecial1Condition()) {
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
	case JM_S1:
	case JM_S2:
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
			else {
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

bool Character::juggleLimit(CHAR_ATT_TYPE type) {
	int counter = 0;
	for (auto it = juggle_attacks_recieved.begin(); it != juggle_attacks_recieved.end(); it++) {
		if (type == *it)
			counter++;
	}

	if (counter >= 3) // 3 is the juggle limit
		return true;
	else
		return false;
}

void Character::tauntFor(int _taunt_duration) {
	taunt_start = SDL_GetTicks();
	taunt_duration = _taunt_duration;
	updateState(TAUNT);
}

void Character::hurtboxSizeManagement() {
	switch (current_state) {
		case CROUCHING:
		case CROUCH_BLOCKING:
			setCrouchingHurtbox(true);
			break;
		case ATTACKING:
		{
			bool hurtbox_set = false;
			for (auto it = crouching_hurtbox_attacks.begin(); it != crouching_hurtbox_attacks.end(); it++){
				if (*it == attack_doing){
					setCrouchingHurtbox(true);
					hurtbox_set = true;
					break;
				}
			}
			if(!hurtbox_set)
				setCrouchingHurtbox(false);
		}
			 break;

		default:
			setCrouchingHurtbox(false);

	}
}
void Character::setAnimationPause(bool active) {
	current_animation->paused = active;
}
void Character::setState(CHAR_STATE state) {
	updateState(state);
}
void Character::deleteAllHitboxes() {
	// Compute what hitboxes need to be deleted
	std::list<collider*> hitboxes_to_delete;
	for (std::list<collider*>::iterator it = hitboxes.begin(); it != hitboxes.end(); ++it) {
		collider* c = *it;
		c->to_delete = true;
		hitboxes_to_delete.push_back(c);
	}
	// Remove the colliders
	for (std::list<collider*>::iterator it = hitboxes_to_delete.begin(); it != hitboxes_to_delete.end(); ++it) {
		collider* c = *it;
		hitboxes.remove(c);
	}

	hitboxes_to_delete.clear();
}

void Character::blitComboCounter(){

}