#include "Character.h"
#include "ProjDefs.h"
#include "DebLog.h"



Character::Character() {

	velocity.y = 0;
	velocity.x = 0;

	//PROVISIONAL: This should be loaded from an xml
	assigned_inputs.keyb_down = DOWN;
	assigned_inputs.keyb_up = UP;
	assigned_inputs.keyb_left = LEFT;
	assigned_inputs.keyb_right = RIGHT;
	assigned_inputs.keyb_a = LIGHT_ATTACK;
	assigned_inputs.keyb_s = HEAVY_ATTACK;

	current_state = IDLE;
	fliped = false;
	//PROVISIONAL: This should be a parameter in the constructor
	position.x = 300;
	position.y = bottom_lane;
}


Character::~Character() {

}

void Character::recieveInput() {
	

	if (App->input->getKey(SDL_SCANCODE_DOWN) == KEY_DOWN)
		modifyInput(assigned_inputs.keyb_down, true);
	if (App->input->getKey(SDL_SCANCODE_UP) == KEY_DOWN)
		modifyInput(assigned_inputs.keyb_up, true);
	if (App->input->getKey(SDL_SCANCODE_LEFT) == KEY_DOWN)
		modifyInput(assigned_inputs.keyb_left, true);
	if (App->input->getKey(SDL_SCANCODE_RIGHT) == KEY_DOWN)
		modifyInput(assigned_inputs.keyb_right, true);
	if (App->input->getKey(SDL_SCANCODE_A) == KEY_DOWN)
		modifyInput(assigned_inputs.keyb_a, true);
	if (App->input->getKey(SDL_SCANCODE_S) == KEY_DOWN)
		modifyInput(assigned_inputs.keyb_s, true);

	if (App->input->getKey(SDL_SCANCODE_DOWN) == KEY_UP)
		modifyInput(assigned_inputs.keyb_down, false);
	if (App->input->getKey(SDL_SCANCODE_UP) == KEY_UP)
		modifyInput(assigned_inputs.keyb_up, false);
	if (App->input->getKey(SDL_SCANCODE_LEFT) == KEY_UP)
		modifyInput(assigned_inputs.keyb_left, false);
	if (App->input->getKey(SDL_SCANCODE_RIGHT) == KEY_UP)
		modifyInput(assigned_inputs.keyb_right, false);
	if (App->input->getKey(SDL_SCANCODE_A) == KEY_UP)
		modifyInput(assigned_inputs.keyb_a, false);
	if (App->input->getKey(SDL_SCANCODE_S) == KEY_UP)
		modifyInput(assigned_inputs.keyb_s, false);
	
}

void Character::requestState() {

}

void Character::updateState() {

}

void Character::update() {

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
		grounded = true;
}

void Character::draw(SDL_Texture* graphic) {
	App->render->blit(graphic, position.x, position.y, &current_animation->GetCurrentFrame(),3);
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

void Character::modifyInput(input requested_input, bool active) {

	switch (requested_input) {
		case UP:
			inputs_pressed.up = active;
			break;
		case DOWN:
			inputs_pressed.down = active;
			break;
		case LEFT:
			inputs_pressed.left = active;
			break;
		case RIGHT:
			inputs_pressed.right = active;
			break;
		case LIGHT_ATTACK:
			inputs_pressed.light_attack = active;
			break;
		case HEAVY_ATTACK:
			inputs_pressed.heavt_attack = active;
			break;
		case SPECIAL_1:
			inputs_pressed.special_1 = active;
			break;
		case SPECIAL_2:
			inputs_pressed.special_2 = active;
			break;
		case GRAB:
			inputs_pressed.grab = active;
			break;
	}
}

bool Character::isNeutralState(state state) 	{

	return state == CROUCHING || state == WALKING_BACK || state == WALKING_FORWARD || state == IDLE;
}
