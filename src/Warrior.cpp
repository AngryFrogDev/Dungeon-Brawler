#include "Warrior.h"



Warrior::Warrior(): Character() {
	type = WARRIOR;
	walk_speed = 4;

	//PROVISIONAL: Animations should be loaded from the xml
	idle.PushBack({ 0,0,195,158 });
	idle.PushBack({ 195,0,195,158 });
	idle.PushBack({ 195 * 2,0,195,158 });
	idle.PushBack({ 195 * 3,0,195,158 });

	idle.loop = true;
	idle.speed = 0.2;

	walk_forward.PushBack({ 0,158,195,158 });
	walk_forward.PushBack({ 195,158,195,158 });
	walk_forward.PushBack({ 195*2,158,195,158 });
	walk_forward.PushBack({ 195*3,158,195,158 });
	walk_forward.PushBack({ 195*4,158,195,158 });
	walk_forward.PushBack({ 195*5,158,195,158 });
	walk_forward.PushBack({ 195*6,158,195,158 });
	walk_forward.PushBack({ 195*7,158,195,158 });
	walk_forward.PushBack({ 195*8,158,195,158 });
	walk_forward.PushBack({ 195*9,158,195,158 });
	walk_forward.PushBack({ 195*10,158,195,158 });
	walk_forward.PushBack({ 195*11,158,195,158 });

	walk_forward.loop = true;
	walk_forward.speed = 0.2;

	walk_back.PushBack({ 195 * 11,158,195,158 });
	walk_back.PushBack({ 195 * 10,158,195,158 });
	walk_back.PushBack({ 195 * 9,158,195,158 });
	walk_back.PushBack({ 195 * 8,158,195,158 });
	walk_back.PushBack({ 195 * 7,158,195,158 });
	walk_back.PushBack({ 195 * 6,158,195,158 });
	walk_back.PushBack({ 195 * 5,158,195,158 });
	walk_back.PushBack({ 195 * 4,158,195,158 });
	walk_back.PushBack({ 195 * 3,158,195,158 });
	walk_back.PushBack({ 195 * 2,158,195,158 });
	walk_back.PushBack({ 195,158,195,158 });
	walk_back.PushBack({ 0,158,195,158 });

	walk_back.loop = true;
	walk_back.speed = 0.2;

	crouch.PushBack({ 0			,158 * 19,195,158 });
	crouch.PushBack({ 195		,158 * 19,195,158 });
	crouch.PushBack({ 195 * 2	,158 * 19,195,158 });

	crouch.loop = false;
	crouch.speed = 0.2;

	light_attack.PushBack({ 0,158 * 13,195, 158 });
	light_attack.PushBack({ 195,158 * 13,195, 158 });
	light_attack.PushBack({ 195 * 2,158 * 13,195, 158 });
	light_attack.PushBack({ 195 * 3,158 * 13,195, 158 });
	light_attack.PushBack({ 195 * 4,158 * 13,195, 158 });
	light_attack.PushBack({ 195 * 5,158 * 13,195, 158 });

	light_attack.loop = false;
	light_attack.speed = 0.2;


	//PROVISIONAL, should be read from xml
	grounded = true;


	jump_power = 25.0f;

}


Warrior::~Warrior() {

}

void Warrior::requestState() {

	// Movement
	if (inputs_pressed.down)
		wanted_state = CROUCHING;

	else if (inputs_pressed.left){ 
		if (fliped)
			wanted_state = WALKING_FORWARD;
		else
			wanted_state = WALKING_BACK;
	}
	else if (inputs_pressed.right){ 
		if (fliped)
			wanted_state = WALKING_BACK;
		else
			wanted_state = WALKING_FORWARD;
	}

	else if (inputs_pressed.up)
		wanted_state = JUMPING;
	else
		wanted_state = IDLE;

	// Attack
	if (inputs_pressed.light_attack) {
		wanted_state = ATTACKING;

		if (inputs_pressed.down)
			attack_doing = CR_L;
		else
			attack_doing = ST_L;
		//else if (airborne)
		//	attack_doing = JM_L;
	}
	if (inputs_pressed.heavt_attack) {
		wanted_state = ATTACKING;

		if (inputs_pressed.down)
			attack_doing = CR_H;
		else
			attack_doing = ST_H;
		//else if (airborne)
		//	attack_doing = JM_H;
	}
	//if (inputs_pressed.special_1) {
	//	wanted_state = ATTACKING;

	//	if (inputs_pressed.down)
	//		attack_doing = CR_S1;
	//	else
	//		attack_doing = ST_S1;
	//	//else if (airborne)
	//	//	attack_doing = JM_S1;
	//}
	//if (inputs_pressed.special_2) {
	//	wanted_state = ATTACKING;

	//	if (inputs_pressed.down)
	//		attack_doing = CR_S2;
	//	else
	//		attack_doing = ST_S2;
	//	//else if (airborne)
	//	//	attack_doing = JM_S2;
	//}

}
void Warrior::updateState() {

	// PROVISIONAL: Should add more conditions (cancelability, jumping...)
	if (current_state != JUMPING) {
		if (isNeutralState(current_state)) {
			current_state = wanted_state;
			updateAnimationWithState(current_state);
		}

		else if (current_animation->Finished()) {
			current_animation->Reset();
			if (inputs_pressed.down)
				current_state = CROUCHING;
			if (inputs_pressed.up)
				current_state = JUMPING;
			else
				current_state = IDLE;


			updateAnimationWithState(current_state);
		}
	}
	else {
		if (grounded)
			current_state = IDLE;
	}

}
void Warrior::update() {
	// PROVISIONAL: Only supporting movement for the movement ( attacks need to be more sofisticated with "doAttack" 

	switch (current_state) {
		case WALKING_BACK:
			if (fliped)
				position.x += walk_speed;
			else
				position.x -= walk_speed;
			break;
		case WALKING_FORWARD:
			if (fliped)
				position.x-= walk_speed;
			else
				position.x+= walk_speed;
			break;
		case JUMPING:
			if (grounded) {
				velocity.y -= jump_power;
				grounded = false;
			}
			applyGravity();		//Do not reverse order! Nasty things will happen
			setIfGrounded();
			break;
	}
			
}

void Warrior::updateAnimationWithState(state state) 	{

	switch (state) 		{
	
		case IDLE:
			current_animation = &idle;
			break;
		case WALKING_BACK:
			current_animation = &walk_back;
			break;
		case WALKING_FORWARD:
			current_animation = &walk_forward;
			break;
		case CROUCHING:
			current_animation = &crouch;
			break;
		case ATTACKING:
			updateAnimationWithAttack(attack_doing);
			break;
	}

}

void Warrior::updateAnimationWithAttack(attack_type type) {

	switch (attack_doing) {
		case ST_L:
			current_animation = &light_attack;
		//case CR_L:
		//	current_animation = &crouching_light_attack;
		// ...
	}
}

