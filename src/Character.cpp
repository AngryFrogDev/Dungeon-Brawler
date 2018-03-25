#include "Character.h"
#include "ProjDefs.h"
#include "DebLog.h"
#include "Player.h"

Character::Character() {

	velocity.y = 0;
	velocity.x = 0;

	current_state = CHAR_STATE::IDLE;
	fliped = false;
	//PROVISIONAL: This should be a parameter in the constructor
	position.x = 300;
	position.y = bottom_lane;
}


Character::~Character() {

}

void Character::update(const bool(&inputs)[MAX_INPUTS]) {
	switch (current_state) {
	case IDLE:
		updateAnimation(idle);
		if (hit)
			current_state = CHAR_STATE::HIT;
		else if (inputs[SWITCH])
			current_state = CHAR_STATE::SWAPPING;
		else if (inputs[RIGHT] && !fliped || inputs[LEFT] && fliped)
			current_state = CHAR_STATE::WALKING_FORWARD;
		else if (inputs[LEFT] && !fliped || inputs[RIGHT] && fliped)
			current_state = CHAR_STATE::WALKING_BACK;
		else if (inputs[UP]) {
			velocity.y -= jump_power;
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
			velocity.y -= jump_power;
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
			position.x += walk_speed;
		else
			position.x -= walk_speed;
			
		break;

	case WALKING_FORWARD:
		updateAnimation(walk_forward);
		if (hit)
			current_state = CHAR_STATE::HIT;
		else if (!fliped && !inputs[RIGHT] || fliped && !inputs[LEFT])
			current_state = CHAR_STATE::IDLE;
		else if (inputs[UP]) {
			velocity.y -= jump_power;
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
			position.x -= walk_speed;
		else
			position.x += walk_speed;
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
			velocity.y -= jump_power;
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
		//TODO: Define hit
		current_state = CHAR_STATE::IDLE;
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
		if (position.y < bottom_lane)
			position.y += velocity.y;
	}
	else {
		position.y += velocity.y;
	}
}

void Character::setIfGrounded() {
	//will be updated
	LOG("%d",position.y);
	if (position.y >= bottom_lane)
	{ 
		grounded = true;
		velocity.y = 0;
	}
}

void Character::draw(SDL_Texture* graphic)  const{
	App->render->blit(graphic, position.x, position.y, &current_animation->GetCurrentFrame(),3);
}

void Character::doAttack() {
	switch (attack_doing) {
	case ST_L:
		updateAnimation(light_attack);
		if (current_animation->Finished())
			current_state = IDLE;
		break;
	case ST_H:
 		updateAnimation(heavy_attack);
		if (current_animation->Finished())
			current_state = IDLE;
		break;
	case CR_L:
		updateAnimation(crouching_light);
		if (current_animation->Finished())
			current_state = CROUCHING;
		break;
	case CR_H: 
		updateAnimation(crouching_heavy);
		if (current_animation->Finished())
			current_state = CROUCHING;
		break;
	case JM_L:
	case JM_H:
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
