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

	createCharacter(0,300, CHAR_TYPE::WARRIOR, false, 1 );
	createCharacter(1,1000, CHAR_TYPE::WARRIOR, true, 2 );
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

	automaticFlip();

	return ret;
}

bool mdEntities::cleanUp() {
	bool ret = true;
	destroyCharacters();
	return ret;
}
void mdEntities::createCharacter(int player,int x_pos, CHAR_TYPE type, bool fliped, int lane) {

	if (players[player] == nullptr)
		players[player] = new Player();

	players[player]->assignCharacter(x_pos,type,fliped, lane);
}

void mdEntities::destroyCharacters() {

	for (int i = 0; i < 4; i++) {
		delete players[i];
		players[i] = nullptr;
	}
}

void mdEntities::automaticFlip() {

	Player* lane1_players[2];
	bool lane1_flip[2];

	int lanes = 2;

	for (int curr_lane = 1; curr_lane < lanes +1; curr_lane++)
	{

		int counter = 0;
		for (int i = 0; i < 4; i++) {
			if (players[i] == nullptr)
				continue;


			//If it breaks here, make sure character.lane != 0
			if (players[i]->getLane() == curr_lane) {
				
				
				lane1_players[counter] = players[i];
				counter++;
			}
		}

		if (lane1_players[1] == nullptr) //to ensure that there are two characters in a lane
			continue;

		if (lane1_players[0]->getPos().x < lane1_players[1]->getPos().x) {
			lane1_flip[0] = false;
			lane1_flip[1] = true;
		}
		else {
			lane1_flip[0] = true;
			lane1_flip[1] = false;
		}

		for (int i = 0; i < 2; i++)
			lane1_players[i]->setFlip(lane1_flip[i]);
	}
}