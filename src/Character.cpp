#include "Character.h"



Character::Character() {

	//PROVISIONAL: This should be loaded from an xml
	assigned_inputs.keyb_down = DOWN;
	assigned_inputs.keyb_up = UP;
	assigned_inputs.keyb_left = LEFT;
	assigned_inputs.keyb_right = RIGHT;
	assigned_inputs.keyb_a = LIGHT_ATTACK;
	assigned_inputs.keyb_s = HEAVY_ATTACK;
}


Character::~Character() {

}

void Character::recieveInput() {
	
	if (App->input->getKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		activateInput(assigned_inputs.keyb_down);
	if (App->input->getKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		activateInput(assigned_inputs.keyb_up);
	if (App->input->getKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		activateInput(assigned_inputs.keyb_left);
	if (App->input->getKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		activateInput(assigned_inputs.keyb_right);
	if (App->input->getKey(SDL_SCANCODE_A) == KEY_REPEAT)
		activateInput(assigned_inputs.keyb_a);
	if (App->input->getKey(SDL_SCANCODE_S) == KEY_REPEAT)
		activateInput(assigned_inputs.keyb_s);

	
}

void Character::requestState() {

}

void Character::updateState() {

}

void Character::update() {

}

void Character::draw() {

}

void Character::doAttack(attack_deff attack) {

}

attack_deff Character::getAttackData(attack_type type) {

	switch (type) {

		case ST_L:
			return standing_light;
			break;
		/*case ST_H:
		{
			return standing_heavy;
		}
		case ST_S1:
		{
			return standing_special_1;
		}
	   ...*/

	}
}

void Character::activateInput(input requested_input) {

	switch (requested_input) {
		case UP:
			inputs_pressed.up = true;
			break;
		case DOWN:
			inputs_pressed.down = true;
			break;
		case LEFT:
			inputs_pressed.left = true;
			break;
		case RIGHT:
			inputs_pressed.right = true;
			break;
		case LIGHT_ATTACK:
			inputs_pressed.light_attack = true;
			break;
		case HEAVY_ATTACK:
			inputs_pressed.heavt_attack = true;
			break;
		case SPECIAL_1:
			inputs_pressed.special_1 = true;
			break;
		case SPECIAL_2:
			inputs_pressed.special_2 = true;
			break;
		case GRAB:
			inputs_pressed.grab = true;
			break;
	}
}