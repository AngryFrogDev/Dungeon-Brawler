#include "Player.h"
#include "Character.h"
#include "Warrior.h"
#include "Mage.h"
#include "Application.h"
#include "mdSceneManager.h"

Player::Player(){
	
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

void Player::createAndAssignCharacter(int x_pos, CHAR_TYPE type, bool fliped, int lane) {

	switch(type)
	{
		case WARRIOR:
		{
			curr_character = new Warrior(App->entities->warrior,x_pos, fliped, lane);
			break;
		}
		case MAGE:
		{
			curr_character = new Mage(App->entities->mage, x_pos, fliped, lane);
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
	curr_character->manageOponent();
			
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
void Player::setFlip(bool flip) {
	curr_character->setFlip(flip);
}