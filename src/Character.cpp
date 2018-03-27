#include "Character.h"
#include "ProjDefs.h"
#include "DebLog.h"
#include "Player.h"
#include "mdCollision.h"

Character::Character() {

	SDL_SCANCODE_C;
}


Character::~Character() {

}

void Character::update(const bool(&inputs)[MAX_INPUTS]) {

	// Calculate draw position out of logic position
	draw_position.x = logic_position.x - 97 * 3; // "97" is character drawing width/2 and "3" is scale
	draw_position.y = logic_position.y - 79 * 3; // "79" is character drawing height/2 and "3" is scale
	// Hurtbox allways next to the player
	iPoint hurtbox_drawing_position;
	hurtbox_drawing_position.x = logic_position.x - hurtbox->rect.w / 2;
	hurtbox_drawing_position.y = logic_position.y - hurtbox->rect.h / 2;

	hurtbox->SetPos(hurtbox_drawing_position.x , hurtbox_drawing_position.y);

	switch (current_state) {
	case IDLE:
		updateAnimation(idle);
		if (hit) { 
			current_state = CHAR_STATE::HIT;
			hit = false;
		}
		else if (inputs[SWITCH])
			current_state = CHAR_STATE::SWAPPING;
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
		else if (inputs[SPECIAL_1]) {
			current_state = CHAR_STATE::ATTACKING;
			attack_doing = CHAR_ATT_TYPE::ST_S1;
		}
		else if (inputs[SPECIAL_2]) {
			current_state = CHAR_STATE::ATTACKING;
			attack_doing = CHAR_ATT_TYPE::ST_S2;
		}
		break;

	case WALKING_BACK:
		updateAnimation(walk_back);
		if (hit)
			current_state = CHAR_STATE::BLOCKING;
		else if (!fliped && !inputs[LEFT] || fliped && !inputs[RIGHT])
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
		else if (inputs[SPECIAL_1]) {
			current_state = CHAR_STATE::ATTACKING;
			attack_doing = CHAR_ATT_TYPE::ST_S1;
		}
		else if (inputs[SPECIAL_2]) {
			current_state = CHAR_STATE::ATTACKING;
			attack_doing = CHAR_ATT_TYPE::ST_S2;
		}

		if (fliped)
			logic_position.x += walk_speed;
		else
			logic_position.x -= walk_speed;
			
		break;

	case WALKING_FORWARD:
		updateAnimation(walk_forward);
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
		else if (inputs[SPECIAL_1]) {
			current_state = CHAR_STATE::ATTACKING;
			attack_doing = CHAR_ATT_TYPE::ST_S1;
		}
		else if (inputs[SPECIAL_2]) {
			current_state = CHAR_STATE::ATTACKING;
			attack_doing = CHAR_ATT_TYPE::ST_S2;
		}

		if (fliped)
			logic_position.x -= walk_speed;
		else
			logic_position.x += walk_speed;
		break;

	case CROUCHING:
		updateAnimation(crouch);
		if (hit && inputs[LEFT] && !fliped || hit && inputs[RIGHT] && fliped)
			current_state = CHAR_STATE::BLOCKING;
		else if (hit)
			current_state = CHAR_STATE::HIT;
		else if (!inputs[DOWN])
			current_state = CHAR_STATE::IDLE;
		else if (inputs[SWITCH])
			current_state = CHAR_STATE::SWAPPING;
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
		updateAnimation(jump);
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

		applyGravity();		//Do not reverse order! Nasty things will happen
		setIfGrounded();
		break;

	case ATTACKING:
		doAttack();
		break;

	case BLOCKING:
		if (!hit)
			current_state = CHAR_STATE::WALKING_BACK;
		else if (!fliped && !inputs[LEFT] || fliped && !inputs[RIGHT])
			current_state = CHAR_STATE::IDLE;
		break;

	case HIT:
		updateAnimation(standing_hit);
		if(SDL_GetTicks()- moment_hit > attack_recieving.hitstun) { 
			current_state = CHAR_STATE::IDLE;
		}
		break;

	case JUGGLE:
		//TODO: Define juggle
		current_state = CHAR_STATE::IDLE;
		break;

	case KNOCKDOWN:
		//TODO: Define knockdown
		current_state = CHAR_STATE::IDLE;
		break;
	}



}

void Character::applyGravity() {

	velocity.y += gravity;
	
	if (velocity.y > 0) {
		if (logic_position.y < bottom_lane)
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
	if (logic_position.y >= bottom_lane)
	{ 
		grounded = true;
		velocity.y = 0;
		velocity.x = 0;
	}
}

void Character::draw(SDL_Texture* graphic)  const{
	App->render->blit(graphic, draw_position.x, draw_position.y, &current_animation->GetCurrentFrame(),3, fliped);
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
			if(hitbox != nullptr) // Just for safety
			hitbox->to_delete = true;
		}	
		else if (current_animation->GetState() == ACTIVE && !instanciated_hitbox) {
			instanciateHitbox(JM_L);
		}	
		applyGravity();		
		setIfGrounded();
		// Set the hitbox to follow the player
		if (hitbox != nullptr) 			{
			hitbox->SetPos(draw_position.x + jm_l.pos_rel_char.x, draw_position.y + jm_l.pos_rel_char.y);
		}


		break;
	case JM_H:
		updateAnimation(jumping_heavy);
		if (grounded) {
			current_state = IDLE; //Maybe should be "RECOVERY"
			instanciated_hitbox = false;
			if (hitbox != nullptr) // Just for safety
				hitbox->to_delete = true;
		}
		else if (current_animation->GetState() == ACTIVE && !instanciated_hitbox) {
			instanciateHitbox(JM_H);
		}
		applyGravity();
		setIfGrounded();
		// Set the hitbox to follow the player
		if (hitbox != nullptr) {
			hitbox->SetPos(draw_position.x + jm_h.pos_rel_char.x, draw_position.y + jm_h.pos_rel_char.y);
		}
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
			collider = { draw_position.x + st_l.pos_rel_char.x, draw_position.y + st_l.pos_rel_char.y,st_l.hitbox.w, st_l.hitbox.h };
			life = st_l.active_time;
			break;
		case ST_H:
			collider = { draw_position.x + st_h.pos_rel_char.x, draw_position.y + st_h.pos_rel_char.y,st_h.hitbox.w, st_h.hitbox.h };
			life = st_h.active_time;
			break;
		case CR_L:
			collider = { draw_position.x + cr_l.pos_rel_char.x, draw_position.y + cr_l.pos_rel_char.y,cr_l.hitbox.w, cr_l.hitbox.h };
			life = cr_l.active_time;
			break;
		case CR_H:
			collider = { draw_position.x + cr_h.pos_rel_char.x, draw_position.y + cr_h.pos_rel_char.y,cr_h.hitbox.w, cr_h.hitbox.h };
			life = cr_h.active_time;
			break;
		case JM_L:
			collider = { draw_position.x + jm_l.pos_rel_char.x, draw_position.y + jm_l.pos_rel_char.y,jm_l.hitbox.w, jm_l.hitbox.h };
			life = jm_l.active_time;
			break;
		case JM_H:
			collider = { draw_position.x + jm_h.pos_rel_char.x, draw_position.y + jm_h.pos_rel_char.y,jm_h.hitbox.w, jm_h.hitbox.h };
			life = jm_h.active_time;
			break;
	}
	hitbox = App->collision->AddCollider(collider, HITBOX,life ,App->entities, this);
	instanciated_hitbox = true;
}

void Character::onCollision(collider* c1, collider* c2) {

	if (c1->type == HURTBOX && c2->type == HITBOX) 	{
		attack_recieving = c2->character->getCurrentAttackData();
		c2->to_delete = true;
		hit = true;
		moment_hit = SDL_GetTicks();
	}
}
basic_attack_deff Character::getCurrentAttackData() {
	switch (attack_doing) {
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
	}
}