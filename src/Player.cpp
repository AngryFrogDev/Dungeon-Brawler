#include "Player.h"
#include "Character.h"
#include "Warrior.h"
#include "Mage.h"
#include "Rogue.h"
#include "Paladin.h"
#include "Application.h"
#include "mdSceneManager.h"

Player::Player(){
	curr_character = nullptr;
}

Player::Player(Controller* controller) : controller(controller){
}

Player::~Player(){
	delete curr_character;
	curr_character = nullptr;
}

void Player::update()
{
	bool player_inputs[MAX_INPUTS] = { false };
	if (controller != nullptr) {
		for (int i = 0; i < MAX_INPUTS; i++){
				if(i < 4)
					player_inputs[i] = controller->isPressed((CONTROLLER_BUTTON)(*player_controller_scheme).scheme[i], KEY_REPEAT);
				else
					player_inputs[i] = controller->isPressed((CONTROLLER_BUTTON)(*player_controller_scheme).scheme[i], App->entities->attack_input);
		}
	}
	else {
		for (int i = 0; i < MAX_INPUTS; i++){
			if(i < 4)
				player_inputs[i] = App->input->getKey((SDL_Scancode)(*player_keyboard_scheme).scheme[i]) == KEY_REPEAT;
			else 
				player_inputs[i] = App->input->getKey((SDL_Scancode)(*player_keyboard_scheme).scheme[i]) == App->entities->attack_input;
		}
	}
	if (curr_character != nullptr) {
		if (!App->entities->paused)
			curr_character->update(player_inputs);
		if(App->entities->show)
			curr_character->draw(curr_character->getGraphics());
	}

}
void Player::assignController(Controller* controller) {

	Player::controller = controller;
}

void Player::createAndAssignCharacter(CHAR_TYPE type, ITEMS item, bool player, int skin) {
	int x_pos = 0;
	bool fliped = false;
	if (player) 
		x_pos = 400;
	else {
		fliped = true;
		x_pos = 1500;
	}


	switch(type)
	{
		case WARRIOR:
		{
			curr_character = new Warrior(App->entities->warrior,x_pos, fliped, skin);
			break;
		}
		case MAGE:
		{
			curr_character = new Mage(App->entities->mage, x_pos, fliped, skin);
			break;
		}
		case ROGUE:
		{
			curr_character = new Rogue(App->entities->rogue, x_pos, fliped, skin);
			break;
		}
		case PALADIN:
		{
			curr_character = new Paladin(App->entities->paladin, x_pos, fliped, skin);
			break;
		}
	}
	curr_character->giveItem(item);
		
}

void Player::assignControlScheme(generic_scheme* new_scheme) {
	player_controller_scheme = new_scheme;
}

void Player::assignKeyboardScheme(generic_scheme* new_scheme) {
	player_keyboard_scheme = new_scheme;
}


Controller * Player::getController() const {
	return controller;
}

iPoint Player::getPos() {
	return curr_character->getPos();
}
Character * Player::getCurrCharacter()
{
	return this->curr_character;
}

void Player::removeCharacters()	{
	if(curr_character){
	curr_character->cleanUp();
	delete curr_character;
	curr_character = nullptr;
	}
}

void Player::setFlip(bool flip) {
	curr_character->setFlip(flip);
}

bool Player::getInput(CHARACTER_INPUTS input, KEY_STATE state) {
	bool ret = false;

	if (controller != nullptr) 
		ret = controller->isPressed((CONTROLLER_BUTTON)(*player_controller_scheme).scheme[(int)input], state);
	else 
		ret = App->input->getKey((SDL_Scancode)(*player_keyboard_scheme).scheme[(int)input]) == state;

	return ret;

}