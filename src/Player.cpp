#include "Player.h"



Player::Player(){

}


Player::~Player(){

}

void Player::update()
{
	//curr_character.update();
}
void Player::assignController(int id) {

	controller_id = id;
}

void Player::assignCharacter(character_type type) {

	switch(type)
	{
		case WARRIOR:
		{
			//curr_character = Character(WARRIOR);
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
