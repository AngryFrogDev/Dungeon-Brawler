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
	std::string name = md_config.child("settings").attribute("attack_type").as_string();
	attack_input = stringToKeystate(name);

	loadCharactersFromXML(App->loadConfig("characters_deff.xml", entities_config_doc));
	//PROVISIONAL: Should be loaded from an xml
	warrior_graphics = App->textures->load("Assets/warrior.png");
	warrior_graphics2 = App->textures->load("Assets/warrior_2_placeholder.png");

	traning = false;

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

	for (int i = 0; i < 4; i++) { // PROVISIONAL: We should check the type of character of the player and pass the correct textures
		if (players[i] != nullptr){
			if(i == 0) // PROVISIONAL: Super hardcode
				players[i]->update(warrior_graphics); 
			else
				players[i]->update(warrior_graphics2);
		}
		
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

KEY_STATE mdEntities::stringToKeystate(std::string string) {
	if (string == "key_repeat")
		return KEY_REPEAT;
	else if (string == "key_down")
		return KEY_DOWN;
	else
		return KEY_NULL;
}
CHAR_TYPE mdEntities::stringToCharType(std::string string) {
	if (string == "warrior")
		return WARRIOR;
	else if (string == "mage")
		return MAGE;
	else if (string == "rogue")
		return ROGUE;
	else if (string == "paladin")
		return PALADIN;

	return DEF_CHAR;
}
ITEMS mdEntities::stringToItem(std::string string) {
	if (string == "special_item_1")
		return SPECIAL_ITEM_1;
	else if (string == "special_item_2")
		return SPECIAL_ITEM_2;
	else if (string == "special_item_3")
		return SPECIAL_ITEM_3;

	return NO_ITEM;
}
void mdEntities::loadCharactersFromXML(const pugi::xml_node& md_config) {
	fillWarriorFromXML(md_config.child("warrior"));
}
void mdEntities::fillWarriorFromXML(const pugi::xml_node& md_config) {
	std::string tmp;
	tmp = md_config.attribute("type").as_string();
	warrior.type = stringToCharType(tmp);
	warrior.scale = md_config.attribute("scale").as_int();
	warrior.gravity = md_config.attribute("gravity").as_float();
	warrior.walk_speed = md_config.attribute("walk_speed").as_int();
	warrior.jump_power.x = md_config.attribute("jump_power_x").as_int();
	warrior.jump_power.y = md_config.attribute("jump_power_y").as_int();
	warrior.max_life = md_config.attribute("max_life").as_int();
	warrior.max_super_gauge = md_config.attribute("max_super_gauge").as_int();
	warrior.super_gauge_gain_hit = md_config.attribute("super_gauge_gain_hit").as_int();
	warrior.super_gauge_gain_block = md_config.attribute("super_gauge_gain_block").as_int();
	warrior.super_gauge_gain_strike = md_config.attribute("super_gauge_gain_strike").as_int();
	//warrior.left_x_limit = md_config.attribute("left_x_limit").as_int();
	//warrior.left_y_limit = md_config.attribute("left_y_limit").as_int();
	warrior.standing_hurtbox_size.x = md_config.attribute("standing_hurtbox_size_x").as_int();
	warrior.standing_hurtbox_size.y = md_config.attribute("standing_hurtbox_size_y").as_int();
	warrior.crouching_hurtbox_offset = md_config.attribute("crouching_hurtbox_offset").as_int();
	warrior.spin_speed = md_config.attribute("spin_speed").as_int();
	tmp = md_config.attribute("spin_object").as_string();
	warrior.spin_object = stringToItem(tmp);
	warrior.jm_s1_angle = md_config.attribute("jm_s1_angle").as_int();
	warrior.jm_s1_speed.x = md_config.attribute("jm_s1_speed_x").as_int();
	warrior.jm_s1_speed.y = md_config.attribute("jm_s1_speed_y").as_int();
	warrior.jm_s2_angle = md_config.attribute("jm_s2_angle").as_int();
	warrior.jm_s2_speed.x = md_config.attribute("jm_s2_speed_x").as_int();
	warrior.jm_s2_speed.y = md_config.attribute("jm_s2_speed_y").as_int();
	tmp = md_config.attribute("dive_kick_object").as_string();
	warrior.dive_kick_object = stringToItem(tmp);
	warrior.projectile_duration = md_config.attribute("projectile_duration").as_int();
	warrior.projectile_speed = md_config.attribute("projectile_speed").as_int();
	warrior.swordyuken_invencivility = md_config.attribute("swordyuken_invencivility").as_int();
	warrior.swordyuken_jump_power = md_config.attribute("swordyuken_jump_power").as_int();
}
