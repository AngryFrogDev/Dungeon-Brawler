#include "Character.h"
#include "ProjDefs.h"
#include "DebLog.h"
#include "Player.h"
#include "mdCollision.h"

Character::Character() {

}


Character::~Character() {

}

void Character::update(const bool(&inputs)[MAX_INPUTS]) {

	//PROVISIONAL: Crazy provisional
	if (current_life <= 0) {
		current_state = CHAR_STATE::DEAD;
	}
	if (App->input->getKey(SDL_SCANCODE_F2) == KEY_DOWN) {
		current_life = max_life;
		current_state = CHAR_STATE::IDLE;
	}

	switch (current_state) {
	case IDLE:
		updateAnimation(idle);
		if (hit) { 
			if (!attack_recieving.knockdown)
				current_state = CHAR_STATE::HIT;
			else 
				current_state = CHAR_STATE::JUGGLE;
		}
		else if (inputs[SWITCH]) {
			current_state = CHAR_STATE::SWAPPING;
			swapRequested = true;						//Important!
		}
		else if (inputs[RIGHT] && !fliped || inputs[LEFT] && fliped)
			current_state = CHAR_STATE::WALKING_FORWARD;
		else if (inputs[LEFT] && !fliped || inputs[RIGHT] && fliped)
			current_state = CHAR_STATE::WALKING_BACK;
		else if (inputs[UP]) {
			velocity.y -= jump_power.y;
			grounded = false;
			current_state = CHAR_STATE::JUMPING;
		}
		else if (inputs[DOWN])
			current_state = CHAR_STATE::CROUCHING;
		else if (inputs[LIGHT_ATTACK]) {
			current_state = CHAR_STATE::ATTACKING;
			attack_doing = CHAR_ATT_TYPE::ST_L;
		}
		else if (inputs[HEAVY_ATTACK]) {
			current_state = CHAR_STATE::ATTACKING;
			attack_doing = CHAR_ATT_TYPE::ST_H;
		}
		else if (inputs[SPECIAL_1] && !projectile) {
			current_state = CHAR_STATE::ATTACKING;
			attack_doing = CHAR_ATT_TYPE::ST_S1;
		}
		else if (inputs[SPECIAL_2]) {
			current_state = CHAR_STATE::ATTACKING;
			attack_doing = CHAR_ATT_TYPE::ST_S2;
		}
		break;

	case WALKING_BACK:
		// Input independent actions
		updateAnimation(walk_back);
		if (fliped)
			logic_position.x += walk_speed;
		else
			logic_position.x -= walk_speed;
		if (hit){
			if (attack_recieving.block_type != BLOCK_TYPE::LOW)
				current_state = CHAR_STATE::STAND_BLOCKING;
			else {
				if (attack_recieving.knockdown)
					current_state = CHAR_STATE::JUGGLE;
				else
					current_state = CHAR_STATE::HIT; // STANDING HIT?
			}
		}
		// Input dependent actions

		if (!fliped && !inputs[LEFT] || fliped && !inputs[RIGHT])
			current_state = CHAR_STATE::IDLE;
		else if (inputs[UP]) {
			velocity.y -= jump_power.y;
			if (!fliped)
				velocity.x -= jump_power.x;
			else
				velocity.x += jump_power.x;
			grounded = false;
			current_state = CHAR_STATE::JUMPING;
		}
		else if (inputs[DOWN])
			current_state = CHAR_STATE::CROUCHING;
		else if (inputs[LIGHT_ATTACK]) {
			current_state = CHAR_STATE::ATTACKING;
			attack_doing = CHAR_ATT_TYPE::ST_L;
		}
		else if (inputs[HEAVY_ATTACK]) {
			current_state = CHAR_STATE::ATTACKING;
			attack_doing = CHAR_ATT_TYPE::ST_H;
		}
		else if (inputs[SPECIAL_1] && !projectile) {
			current_state = CHAR_STATE::ATTACKING;
			attack_doing = CHAR_ATT_TYPE::ST_S1;
		}
		else if (inputs[SPECIAL_2]) {
			current_state = CHAR_STATE::ATTACKING;
			attack_doing = CHAR_ATT_TYPE::ST_S2;
		}		

//		if (fliped && position.x >= 600) { // The position limit should be a variable
//			position.x += walk_speed;
//			App->render->camera.x -= walk_speed; // Camera should NOT be managed here
//		}
//		else if (position.x >= 600) { // The position limit should be a variable
//			position.x -= walk_speed;
//			App->render->camera.x += walk_speed; // Camera should NOT be managed here
//		}
		break;

	case WALKING_FORWARD:
		// Input independent actions
		updateAnimation(walk_forward);
		if (fliped)
			logic_position.x -= walk_speed;
		else
			logic_position.x += walk_speed;
		// Input dependent actions
		if (hit)
			current_state = CHAR_STATE::HIT;
		else if (!fliped && !inputs[RIGHT] || fliped && !inputs[LEFT])
			current_state = CHAR_STATE::IDLE;
		else if (inputs[UP]) {
			velocity.y -= jump_power.y;
			if (fliped)
				velocity.x -= jump_power.x;
			else
				velocity.x += jump_power.x;
			grounded = false;
			current_state = CHAR_STATE::JUMPING;
		}
		else if (inputs[DOWN])
			current_state = CHAR_STATE::CROUCHING;
		else if (inputs[LIGHT_ATTACK]) {
			current_state = CHAR_STATE::ATTACKING;
			attack_doing = CHAR_ATT_TYPE::ST_L;
		}
		else if (inputs[HEAVY_ATTACK]) {
			current_state = CHAR_STATE::ATTACKING;
			attack_doing = CHAR_ATT_TYPE::ST_H;
		}
		else if (inputs[SPECIAL_1] && !projectile) {
			current_state = CHAR_STATE::ATTACKING;
			attack_doing = CHAR_ATT_TYPE::ST_S1;
		}
		else if (inputs[SPECIAL_2]) {
			current_state = CHAR_STATE::ATTACKING;
			attack_doing = CHAR_ATT_TYPE::ST_S2;
		}

//		if (fliped && position.x <= 2500) { // The position limit should be a variable
//			position.x -= walk_speed;
//			App->render->camera.x += walk_speed; // Camera should NOT be managed here
//		}
//		else if (position.x <= 2500) { // The position limit should be a variable)
//			position.x += walk_speed;
//			App->render->camera.x -= walk_speed; // Camera should NOT be managed here
//		}
		break;

	case CROUCHING:
		// Input independent anctions
		updateAnimation(crouch);
		if (!crouching_hurtbox) {
			hurtbox->rect.h /= 2;
			crouching_hurtbox = true;
		}
		if (hit && inputs[LEFT] && !fliped|| hit && inputs[RIGHT] && fliped) {
			if(attack_recieving.block_type != BLOCK_TYPE::OVERHEAD)
				current_state = CHAR_STATE::CROUCH_BLOCKING;
			else{
				if (attack_recieving.knockdown)
					current_state = CHAR_STATE::JUGGLE;
				else
					current_state = CHAR_STATE::HIT; // CROUCHING HIT?
			}
		}	
		else if (hit) {
			if (attack_recieving.knockdown)
				current_state = CHAR_STATE::JUGGLE;
			else
				current_state = CHAR_STATE::HIT; // CROUCHING HIT?
		}

		// Input dependent actions
		if (!inputs[DOWN]) {
			current_state = CHAR_STATE::IDLE;
		}
		else if (inputs[SWITCH]) {
			swapRequested = true;						//Important!
			hurtbox->rect.h = standing_hurtbox_size.y;
			crouching_hurtbox = false;
		}
		else if (inputs[UP]) {
			velocity.y -= jump_power.y;
			grounded = false;
			current_state = CHAR_STATE::JUMPING;
		}
		else if (inputs[LIGHT_ATTACK]) {
			current_state = CHAR_STATE::ATTACKING;
			attack_doing = CHAR_ATT_TYPE::CR_L;
		}
		else if (inputs[HEAVY_ATTACK]) {
			current_state = CHAR_STATE::ATTACKING;
			attack_doing = CHAR_ATT_TYPE::CR_H;
		}
		else if (inputs[SPECIAL_1]) {
			current_state = CHAR_STATE::ATTACKING;
			attack_doing = CHAR_ATT_TYPE::CR_S1;
		}
		else if (inputs[SPECIAL_2]) {
			current_state = CHAR_STATE::ATTACKING;
			attack_doing = CHAR_ATT_TYPE::CR_S2;
		}
		break;

	case JUMPING:
		// Input independent actions
		updateAnimation(jump);
		// Input dependent actions
		if (grounded)
			current_state = CHAR_STATE::IDLE;
		else if (hit)
			current_state = CHAR_STATE::JUGGLE;
		else if (inputs[LIGHT_ATTACK]) {
			current_state = CHAR_STATE::ATTACKING;
			attack_doing = CHAR_ATT_TYPE::JM_L;
		}
		else if (inputs[HEAVY_ATTACK]) {
			current_state = CHAR_STATE::ATTACKING;
			attack_doing = CHAR_ATT_TYPE::JM_H;
		}
		else if (inputs[SPECIAL_1]) {
			current_state = CHAR_STATE::ATTACKING;
			attack_doing = CHAR_ATT_TYPE::JM_S1;
		}
		else if (inputs[SPECIAL_2]) {
			current_state = CHAR_STATE::ATTACKING;
			attack_doing = CHAR_ATT_TYPE::JM_S2;
		}
		break;

	case ATTACKING:
		doAttack();
		if (hit) 			{
			if (attack_recieving.knockdown)
				current_state = CHAR_STATE::JUGGLE;
			else
				current_state = CHAR_STATE::HIT; // CROUCHING HIT?
		}
		break;

	case STAND_BLOCKING:
		//Input independent actions
		// Continuous
		if (!fliped)
			logic_position.x -= attack_recieving.pushblock;
		else
			logic_position.x += attack_recieving.pushblock;
		updateAnimation(standing_block);
		// One tick
		if (hit) {
			hit = false;
		}
		else if (SDL_GetTicks() - moment_hit > attack_recieving.blockstun)
			current_state = CHAR_STATE::IDLE;
		break;

	case CROUCH_BLOCKING:
		//Input independent actions
		// Continuous
		if (!fliped)
			logic_position.x -= attack_recieving.pushblock;
		else
			logic_position.x += attack_recieving.pushblock;

		updateAnimation(crouching_block);
		// One tick
		if (hit) {
			hit = false;
		}
		else if (SDL_GetTicks() - moment_hit > attack_recieving.blockstun)
			current_state = CHAR_STATE::CROUCHING;
		break;

	case HIT:
		//Input independent actions
		// Continuous
		if (!fliped)
			logic_position.x -= attack_recieving.pushhit;
		else
			logic_position.x += attack_recieving.pushhit;
		updateAnimation(standing_hit);
		// One tick
		if (hit) { 
			if (attack_recieving.knockdown)
				current_state = CHAR_STATE::JUGGLE;
			else
				hit = false;
				current_life -= attack_recieving.damage;
		}
		else if(SDL_GetTicks()- moment_hit > attack_recieving.hitstun) 
			current_state = CHAR_STATE::IDLE;
	

		break;

	case JUGGLE:
		// TODO: Put a juggle animation
		updateAnimation(standing_hit); 
		if (hit) {
			if (!fliped)
				velocity.x -= attack_recieving.juggle_speed.x;
			else
				velocity.x += attack_recieving.juggle_speed.x;

			velocity.y -= attack_recieving.juggle_speed.y;
			
			current_life -= attack_recieving.damage;
			hit = false;
			grounded = false;
		}
		if (grounded)
			current_state = CHAR_STATE::KNOCKDOWN; // Should be Knockdown, but it is not implemented yet
		break;

	case KNOCKDOWN:
		//Input independent actions
		updateAnimation(knockdown);
		hurtbox->active = false;
		if (current_animation->Finished()){
			makeInvencibleFor(invencibility_on_wakeup);
			current_state = IDLE;
			hurtbox->active = true;
		}
		break;
	case SWAPPING:
		//TODO: Define swapping
		manageSwap();
		break;
	case DEAD:
		updateAnimation(dead);
		App->render->drawQuad({ 0,0,100,100 }, 255, 255, 255, 255);
		break;
	}

	manageGroundPosition();

	// State independent actions
	updateInvecibility();

	// Calculate draw position out of logic position
	draw_position.x = calculateDrawPosition(0, draw_size.x* scale, true);
	draw_position.y = calculateDrawPosition(0, draw_size.y * scale, false);

	// Hurtbox allways next to the player
	int offset;
	if (crouching_hurtbox)
		offset = crouching_hurtbox_offset;
	else
		offset = 0;

	hurtbox->SetPos(calculateDrawPosition(0, hurtbox->rect.w, true), calculateDrawPosition(offset, hurtbox->rect.h, false));

	if (!grounded) 		{
		applyGravity();
		setIfGrounded();
	}


}

void Character::onCollision(collider* c1, collider* c2) {

	if (c1->type == HURTBOX && c2->type == HITBOX) {
		attack_recieving = c2->character->getAttackData(c2->attack_type); 
		c2->to_delete = true;
		if (c2->character->hitbox == c2) { // PROVISIONAL: CRAZY PROVISIONAL
			c2->character->hitbox = nullptr;
		}
		hit = true;
		moment_hit = SDL_GetTicks();
	}
	else if (c1->type == HURTBOX && c2->type == HURTBOX) {
		if (!fliped)
			logic_position.x -= walk_speed;
		else
			logic_position.x += walk_speed;
	}
}
void Character::applyGravity() {

	velocity.y += gravity;
	
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
	//will be updated
	LOG("%d",logic_position.y);
	if (logic_position.y >= ground_position)
	{ 
		grounded = true;
		logic_position.y = ground_position;
		velocity.y = 0;
		velocity.x = 0;
	}
}

void Character::draw(SDL_Texture* graphic)  const{
	App->render->blit(3,graphic, draw_position.x, draw_position.y, &current_animation->GetCurrentFrame(),scale, fliped);
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
			current_state = CHAR_STATE::JUMPING;
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

void Character::doAttack() {
	switch (attack_doing) {
	case ST_L:
		updateAnimation(light_attack);
		if (current_animation->Finished()) 			{
			current_state = IDLE;
			instanciated_hitbox = false;
		}
		else if (current_animation->GetState() == ACTIVE && !instanciated_hitbox) {
			instanciateHitbox(ST_L);
		}
		break;
	case ST_H:
 		updateAnimation(heavy_attack);
		if (current_animation->Finished()) {
			current_state = IDLE;
			instanciated_hitbox = false;
		}
		else if (current_animation->GetState() == ACTIVE && !instanciated_hitbox) {
			instanciateHitbox(ST_H);
		}
		break;
	case CR_L:
		updateAnimation(crouching_light);
		if (current_animation->Finished()) {
			current_state = CROUCHING;
			instanciated_hitbox = false;
		}
		else if (current_animation->GetState() == ACTIVE && !instanciated_hitbox) {
			instanciateHitbox(CR_L);
		}
		break;
	case CR_H: 
		updateAnimation(crouching_heavy);
		if (current_animation->Finished()) {
			current_state = CROUCHING;
			instanciated_hitbox = false;
		}
		else if (current_animation->GetState() == ACTIVE && !instanciated_hitbox) {
			instanciateHitbox(CR_H);
		}
		break;
	case JM_L:
		updateAnimation(jumping_light);
		if (grounded) {
			current_state = IDLE; //Maybe should be "RECOVERY"
			instanciated_hitbox = false;
			if(hitbox != nullptr){ // Just for safety
				hitbox->to_delete = true;
				hitbox = nullptr;
			}
		}	
		else if (current_animation->GetState() == ACTIVE && !instanciated_hitbox) {
			instanciateHitbox(JM_L);
		}	
		// Set the hitbox to follow the player
		if (hitbox != nullptr) 			{
			hitbox->SetPos(calculateDrawPosition(jm_l.pos_rel_char.x, jm_l.hitbox.w, true), calculateDrawPosition(jm_l.pos_rel_char.y, jm_l.hitbox.h, false));
		}
		break;
	case JM_H:
		updateAnimation(jumping_heavy);
		if (grounded) {
			current_state = IDLE; //Maybe should be "RECOVERY"
			instanciated_hitbox = false;
			if (hitbox != nullptr){  // Just for safety
				hitbox->to_delete = true;
				hitbox = nullptr;
			}
		}
		else if (current_animation->GetState() == ACTIVE && !instanciated_hitbox) {
			instanciateHitbox(JM_H);
		}	
		// Set the hitbox to follow the player
		if (hitbox != nullptr) {
			hitbox->SetPos(calculateDrawPosition(jm_h.pos_rel_char.x, jm_h.hitbox.w, true), calculateDrawPosition(jm_h.pos_rel_char.y, jm_h.hitbox.h, false));
		}
		break;
	case ST_S1:
		updateAnimation(standing_special1);
		standingSpecial1(); 
		break;
	case ST_S2:
		updateAnimation(standing_special2);
		standingSpecial2();
		break;
	case JM_S1:
	case JM_S2:
		current_state = JUMPING;
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
	}
	hitbox = App->collision->AddCollider(collider, HITBOX,life ,type, App->entities, this);
	instanciated_hitbox = true;
}


void Character::manageGroundPosition() {
	if (lane == 1)
		ground_position = bottom_lane;
	if (lane == 2)
		ground_position = upper_lane;
}



basic_attack_deff Character::getAttackData(CHAR_ATT_TYPE attack_type) {
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
		case NO_ATT:
			LOG("FATAL ERROR");
			//return void;
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

iPoint Character::getPos() 	{
	return logic_position;
}
void Character::setFlip(bool flip) {
	fliped = flip;
}
void Character::setProjectile(bool _projectile) {
	projectile = _projectile;
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