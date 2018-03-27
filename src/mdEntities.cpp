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
	//Load controller schemes
	pugi::xml_node controller_schemes_node = md_config.child("schemes").child("controller_schemes");
	for (pugi::xml_node_iterator it = controller_schemes_node.children().begin(); it != controller_schemes_node.children().end(); ++it) {
		controller_scheme new_scheme = controller_scheme(it->name());
		pugi::xml_attribute_iterator attribute = it->attributes().begin();
		for (int i = 0; i < MAX_INPUTS; ++i, ++attribute)
			new_scheme.scheme[i] = (CONTROLLER_BUTTON)attribute->as_int();
		controller_schemes.push_back(new_scheme);
	}

	//Load keyboard schemes
	pugi::xml_node keyboard_schemes_node = md_config.child("schemes").child("keyboard_schemes");
	for (pugi::xml_node_iterator it = keyboard_schemes_node.children().begin(); it != keyboard_schemes_node.children().end(); ++it) {
		keyboard_scheme new_scheme = keyboard_scheme(it->name());
		pugi::xml_attribute_iterator attribute = it->attributes().begin();
		for (int i = 0; i < MAX_INPUTS; ++i, ++attribute)
			new_scheme.scheme[i] = (SDL_Scancode)attribute->as_int();
		keyboard_schemes.push_back(new_scheme);
	}

	//PROVISIONAL: Should be loaded from an xml
	warrior_graphics = App->textures->load("Assets/warrior.png");

	createCharacter(0, CHAR_TYPE::WARRIOR, false);
	createCharacter(1, CHAR_TYPE::WARRIOR, true);
	players[0]->assignControlScheme(controller_schemes.front());
	players[1]->assignKeyboardScheme(keyboard_schemes.front());

	return ret;
}

bool mdEntities::preUpdate() {
	bool ret = true;


	if (players[0]->getController() == nullptr) {
		std::list<Controller*> controllers = App->input->getController();
		if (!controllers.empty())
			players[0]->assignController(controllers.front());
	}

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
void mdEntities::createCharacter(int player, CHAR_TYPE type, bool fliped) {

	if (players[player] == nullptr)
		players[player] = new Player();

	players[player]->assignCharacter(type,fliped);
}

void mdEntities::destroyCharacters() {

	for (int i = 0; i < 4; i++) {
		delete players[i];
		players[i] = nullptr;
	}
}