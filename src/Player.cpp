#include "Player.h"
#include "Character.h"
#include "Warrior.h"
#include "Mage.h"
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

void Player::update(SDL_Texture* graphics)
{
	bool player_inputs[MAX_INPUTS] = { false };
	if (controller != nullptr) {
		for (int i = 0; i < MAX_INPUTS; i++){
				if(i < 4)
					player_inputs[i] = controller->isPressed(player_controller_scheme.scheme[i], KEY_REPEAT);
				else
					player_inputs[i] = controller->isPressed(player_controller_scheme.scheme[i], App->entities->attack_input);
		}
	}
	else {
		for (int i = 0; i < MAX_INPUTS; i++){
			if(i < 4)
				player_inputs[i] = App->input->getKey(player_keyboard_scheme.scheme[i]) == KEY_REPEAT;
			else 
				player_inputs[i] = App->input->getKey(player_keyboard_scheme.scheme[i]) == App->entities->attack_input;
		}
	}

	if (curr_character != nullptr) {
		if(!App->entities->paused)
			curr_character->update(player_inputs);
		if(App->entities->show)
			curr_character->draw(graphics);
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
		//case ROGUE:
		//{
		//	curr_character = Character(ROGUE);
		//	break;
		//}
		//case PALADIN:
		//{
		//	curr_character = Character(PALADIN);
		//	break;
		//}
	}
	curr_character->giveItem(item);
		
}

void Player::assignControlScheme(const controller_scheme& new_scheme) {
	player_controller_scheme = new_scheme;
}

void Player::assignKeyboardScheme(const keyboard_scheme& new_scheme) {
	player_keyboard_scheme = new_scheme;
}


Controller * Player::getController() const {
	return controller;
}

int Player::getLane() {
	return curr_character->lane;
}
iPoint Player::getPos() {
	return curr_character->getPos();
}
Character * Player::getCurrCharacter()
{
	return this->curr_character;
}

void Player::removeCharacters()	{
	for (int i = 0; i < 2; i++) {
		delete App->entities->players[i]->getCurrCharacter();
		App->entities->players[i]->curr_character = nullptr;
	}
}

void Player::setFlip(bool flip) {
	curr_character->setFlip(flip);
}

bool Player::getInput(CHARACTER_INPUTS input, KEY_STATE state) {
	bool ret = false;

	if (controller != nullptr) 
		ret = controller->isPressed(player_controller_scheme.scheme[(int)input], state);
	else 
		ret = App->input->getKey(player_keyboard_scheme.scheme[(int)input]) == state;

	return ret;

}