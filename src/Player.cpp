#include "Player.h"
#include "Character.h"
#include "Warrior.h"
#include "Application.h"

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
	bool player_inputs[MAX_INPUTS];
	if (controller != nullptr) {
		for (int i = 0; i < MAX_INPUTS; i++)
			player_inputs[i] = controller->isPressed(scheme.scheme[i]);
	}
	if (curr_character != nullptr) {
		curr_character->update(player_inputs);
		curr_character->draw(graphics);
	}
}
void Player::assignController(Controller* controller) {

	Player::controller = controller;
}

void Player::assignCharacter(CHAR_TYPE type, bool fliped) {

	switch(type)
	{
		case WARRIOR:
		{
			curr_character = new Warrior(fliped);
			break;
		}
		//case MAGE:
		//{
		//	curr_character = Character(MAGE);
		//	break;
		//}
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
}

void Player::assignControlScheme(const controller_scheme& new_scheme) {
	scheme = new_scheme;
}

Controller * Player::getController() const {
	return controller;
}
