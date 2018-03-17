#include "mdEntities.h"



mdEntities::mdEntities(){

}


mdEntities::~mdEntities(){

}

bool mdEntities::awake(const pugi::xml_node & md_config) {
	bool ret = true;

	// Empty the array
	for (int i = 0; i < 4; i++) {
		players[i] = nullptr;
	}

	//PROVISIONAL: Should be loaded from an xml
	warrior_graphics = App->textures->load("Assets/warrior.png");

	createCharacter(1, WARRIOR);

	return ret;
}

bool mdEntities::preUpdate() {
	bool ret = true;

	for (int i = 0; i < 4; i++) {
		if (players[i] != nullptr)
			players[i]->update(warrior_graphics); //We should check the type of character of the player and pass the correct textures
	}

	return ret;
}

bool mdEntities::cleanUp() {
	bool ret = true;
	destroyCharacters();
	return ret;
}
void mdEntities::createCharacter(int player, character_type type) {

	if (players[player] != nullptr) {
		delete players[player];
		players[player] = nullptr;
	}

	switch (type) {
		case WARRIOR:
			players[player] = new Player(player, type);
			break;
		//case MAGE: 
		//	players[player] = new Mage();
		//	break;
		//case ROGUE:
		//	players[player] = new Rogue();
		//	break;
		//case PALADIN:
		//	players[player] = new Paladin();
		//	break;
	}
}

void mdEntities::destroyCharacters() {

	for (int i = 0; i < 4; i++) {
		delete players[i];
		players[i] = nullptr;
	}
}