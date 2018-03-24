#include "mdEntities.h"
#include "Player.h"

mdEntities::mdEntities(){
	name = "entities";
}


mdEntities::~mdEntities(){

}

bool mdEntities::awake(const pugi::xml_node & md_config) {
	bool ret = true;

	// Empty the array
	for (int i = 0; i < 4; i++)
		players[i] = nullptr;
	
	pugi::xml_node schemes_node = md_config.child("schemes");
	for (pugi::xml_node_iterator it = schemes_node.children().begin(); it != schemes_node.children().end(); ++it) {
		controller_scheme new_scheme = controller_scheme(it->name());
		pugi::xml_attribute_iterator attribute = it->attributes().begin();
		for (int i = 1; i < MAX_INPUTS; ++i, ++attribute)
			new_scheme.scheme[i] = (CONTROLLER_BUTTON)attribute->as_int();
		schemes.push_back(new_scheme);
	}
	//PROVISIONAL: Should be loaded from an xml
	warrior_graphics = App->textures->load("Assets/warrior.png");
	std::list<Controller*> controllers = App->input->getController();
	if (!controllers.empty())
		players[0] = new Player(controllers.front());
	createCharacter(0, CHAR_TYPE::WARRIOR);
	players[0]->assignControlScheme(&schemes.front());

	return ret;
}

bool mdEntities::preUpdate() {
	bool ret = true;

	for (int i = 0; i < 4; i++) {
		if (players[i] != nullptr)
			players[i]->update(warrior_graphics); // PROVISIONAL: We should check the type of character of the player and pass the correct textures
	}

	return ret;
}

bool mdEntities::cleanUp() {
	bool ret = true;
	destroyCharacters();
	return ret;
}
void mdEntities::createCharacter(int player, CHAR_TYPE type) {

	if (players[player] == nullptr)
		players[player] = new Player();

	players[player]->assignCharacter(type);
}

void mdEntities::destroyCharacters() {

	for (int i = 0; i < 4; i++) {
		delete players[i];
		players[i] = nullptr;
	}
}