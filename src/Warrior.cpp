#include "Warrior.h"



Warrior::Warrior(): Character() {
	type = WARRIOR;

	//PROVISIONAL: Animations should be loaded from the xml
	idle.PushBack({ 0,0,195,158 });
	idle.PushBack({ 195,0,195,158 });
	idle.PushBack({ 195 * 2,0,195,158 });
	idle.PushBack({ 195 * 3,0,195,158 });

	idle.loop = true;
	idle.speed = 0.2;

	walk.PushBack({ 0,158,195,158 });
	walk.PushBack({ 195,158,195,158 });
	walk.PushBack({ 195*2,158,195,158 });
	walk.PushBack({ 195*3,158,195,158 });
	walk.PushBack({ 195*4,158,195,158 });
	walk.PushBack({ 195*5,158,195,158 });
	walk.PushBack({ 195*6,158,195,158 });
	walk.PushBack({ 195*7,158,195,158 });
	walk.PushBack({ 195*8,158,195,158 });
	walk.PushBack({ 195*9,158,195,158 });
	walk.PushBack({ 195*10,158,195,158 });
	walk.PushBack({ 195*11,158,195,158 });

	walk.loop = true;
	walk.speed = 0.2;
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
	if (isNeutralState(current_state)) {
		current_state = wanted_state;
		updateAnimationWithState(current_state);
	}
	else if (current_animation->Finished()) {
		current_animation->Reset();
		if (inputs_pressed.down)
			current_state = CROUCHING;
		else
			current_state = IDLE;

		updateAnimationWithState(current_state);
	}
}
void Warrior::update() {
	// PROVISIONAL: Only supporting movement for the movement (shold use movement speed, attacks need to be more sofisticated with "doAttack" 

	switch (current_state) {
		case WALKING_BACK:
			if (fliped)
				position.x++;
			else
				position.x--;
			break;
		case WALKING_FORWARD:
			if (fliped)
				position.x--;
			else
				position.x++;
			break;
	}
			
}

void Warrior::updateAnimationWithState(state state) 	{

	switch (state) 		{
	
		case IDLE:
			current_animation = &idle;
			break;
		case WALKING_BACK:
			current_animation = &walk;
			break;
		case WALKING_FORWARD:
			current_animation = &walk;
			break;
		case CROUCHING:
			current_animation = &crouch;
	}

}