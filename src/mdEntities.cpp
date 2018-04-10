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

	//createPlayer(0,100, CHAR_TYPE::WARRIOR, false, 1 );
	//createPlayer(1,1000, CHAR_TYPE::WARRIOR, true, 1); //play with the lane (last argument) for 2v2
	//createPlayer(2, 1200, CHAR_TYPE::WARRIOR, true, 1);
	//createPlayer(3, 300, CHAR_TYPE::WARRIOR, true, 2);

	// PROVISIONAL: Very dangerous hardcode to set the partners: 

	//players[0]->getCurrCharacter()->partner = players[1];
	//players[1]->getCurrCharacter()->partner = players[0];
	//players[2]->getCurrCharacter()->partner = players[3];
	//players[3]->getCurrCharacter()->partner = players[2];

	//players[0]->assignControlScheme(controller_schemes.front());
	//players[1]->assignKeyboardScheme(keyboard_schemes.front());


	return ret;
}

bool mdEntities::preUpdate() {
	bool ret = true;

	if (players[0] == nullptr) //It crashed if there were no playres
		return true;

	if (players[0]->getController() == nullptr) { // Shouldn't this be done less often?
		assignControllers();
	}

	for (int i = 0; i < 4; i++) {
		if (players[i] != nullptr)
			players[i]->update(warrior_graphics); // PROVISIONAL: We should check the type of character of the player and pass the correct textures
	}



	return ret;
}

bool mdEntities::postUpdate()
{
	for (int i = 0; i < 4; i++) {
		if (players[i] == nullptr)
			continue;
		if (players[i]->getCurrCharacter()->readyToSwap == true) //no flip if characters are swapping
			return true;
	}

	if(allowFlip()) // PROVISIONAL: Kind of useless now, but it can be useful in the future
		automaticFlip();

	return true;
}

bool mdEntities::cleanUp() {
	bool ret = true;
	destroyCharacters();
	return ret;
}
void mdEntities::createPlayer(int player,int x_pos, CHAR_TYPE type, bool fliped, int lane) {

	if (players[player] == nullptr)
		players[player] = new Player();

	players[player]->createAndAssignCharacter(x_pos,type,fliped, lane);
}

void mdEntities::destroyCharacters() {

	for (int i = 0; i < 4; i++) {
		delete players[i];
		players[i] = nullptr;
	}
}

void mdEntities::assignControls()
{
	//PROVISIONAL: HARDCODE
	if (players[0] != nullptr){
		players[0]->assignControlScheme(controller_schemes.front());
		players[0]->assignKeyboardScheme(keyboard_schemes.front());
	}

	if (players[1] != nullptr){
		players[1]->assignControlScheme(controller_schemes.front());
		players[1]->assignKeyboardScheme(keyboard_schemes.front());
	}
}

void mdEntities::assignPartners()
{
	// PROVISIONAL: Very dangerous hardcode to set the partners: 
	players[0]->getCurrCharacter()->partner = players[1];
	players[1]->getCurrCharacter()->partner = players[0];
	players[2]->getCurrCharacter()->partner = players[3];
	players[3]->getCurrCharacter()->partner = players[2];
}

bool mdEntities::automaticFlip() {
	bool ret = true;

	Player* players_on_curr_lane[2];
	players_on_curr_lane[0] = nullptr;
	players_on_curr_lane[1] = nullptr;

	//Whatch the fuck out with this method

	bool lane1_flip[2];


	int lanes = 2;

	for (int curr_lane = 1; curr_lane < lanes +1; curr_lane++)
	{

		int counter = 0;
		for (int i = 0; i < 4; i++) {
			if (players[i] == nullptr)
				continue;
			
			int lane_of_player = players[i]->getLane();

			if (lane_of_player == 0)
				return false;

			if (lane_of_player == curr_lane) {
				players_on_curr_lane[counter] = players[i];
				counter++;
			}
		}

		if (players_on_curr_lane[1] == nullptr) { 
			return false;
		}

		if (players_on_curr_lane[0]->getPos().x < players_on_curr_lane[1]->getPos().x) {
			lane1_flip[0] = false;
			lane1_flip[1] = true;
		}
		else {
			lane1_flip[0] = true;
			lane1_flip[1] = false;
		}

		for (int i = 0; i < 2; i++) {
			players_on_curr_lane[i]->setFlip(lane1_flip[i]);
			players_on_curr_lane[i] = nullptr;
		}
	}

	return ret;
}

bool mdEntities::allowFlip() 	{

	// PROVISIONAL: Every character should have specified with which states flip should not be executed
	bool do_flip = true;
	for (int i = 0; i < 4; i++) {
		if (players[i] == nullptr)
			continue;
		Character* character = players[i]->getCurrCharacter();
		if (character->getCurrentState() == ATTACKING && (character->getAttackDoing() == JM_S1 || character->getAttackDoing() == JM_S2))
			do_flip = false;
	}

	return do_flip;
}

void mdEntities::assignControllers() {
	std::list<Controller*> controllers = App->input->getController();
	int counter = 0;
	for (auto it = controllers.begin(); it != controllers.end() || counter > 4; it++) {
		players[counter]->assignController(*it);
		counter++;
	}
}