#include "Player.h"

Player::Player(){

}

Player::Player(int controller_id, character_type type) {

	assignController(controller_id);
	assignCharacter(type);
}

Player::~Player(){
	delete curr_character;
	curr_character = nullptr;
}

void Player::update(SDL_Texture* graphics)
{
	curr_character->recieveInput();
	curr_character->requestState();
	curr_character->updateState();
	curr_character->update();
	curr_character->draw(graphics);
}
void Player::assignController(int id) {

	controller_id = id;
}

void Player::assignCharacter(character_type type) {

	switch(type)
	{
		case WARRIOR:
		{
			curr_character = new Warrior();
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
