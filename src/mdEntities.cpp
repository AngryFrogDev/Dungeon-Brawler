#include "mdEntities.h"
#include "mdMap.h"
#include "Player.h"
#include <algorithm>

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
	show = true;
	paused = false;

	camera_movement = 5; //Should be loaded from XML

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

bool mdEntities::moveCamera(bool movingLeft) {
	bool ret = true;
	if (movingLeft) {
		if (App->render->camera.x == 0) ret = false;
		else {
			int target_x = std::max(App->render->camera.x - camera_movement, 0);
			for (int i = 0; i < 4 && ret; ++i) {
				if (players[i] != nullptr && players[i]->getCurrCharacter() != nullptr) {
					Character* temp = players[i]->getCurrCharacter();
					int future_limit = target_x + App->render->camera.w - temp->getLateralLimit();
					if (temp->getPos().x > future_limit) ret = false;  //If the character will be out of limits, the camera doesn't move
				}
			}

			if (ret)
				App->render->camera.x = target_x;
		}
	}
	else {
		int map_limit = App->map->data.camera_x_limit;
		if (App->render->camera.x == map_limit) ret = false;
		else {
			int target_x = std::min(App->render->camera.x + camera_movement + App->render->camera.w, map_limit) - App->render->camera.w;
			for (int i = 0; i < 4 && ret; ++i) {
				if (players[i] != nullptr && players[i]->getCurrCharacter() != nullptr) {
					Character* temp = players[i]->getCurrCharacter();
					int future_limit = target_x + temp->getLateralLimit();
					if (temp->getPos().x < future_limit) ret = false;  //If the character will be out of limits, the camera doesn't move
				}
			}

			if (ret)
				App->render->camera.x = target_x;
		}
	}

	return ret;

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
		if (character->getCurrentState() == ATTACKING && (character->getAttackDoing() == JM_S1 || character->getAttackDoing() == JM_S2 || character->getAttackDoing() == CR_S1))
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
BLOCK_TYPE mdEntities::stringToBlockType(std::string string) {
	if (string == "mid")
		return MID;
	else if (string == "low")
		return LOW;
	else if (string == "overhead")
		return OVERHEAD;

	return NO_BLOCK_TYPE;
}
CHAR_ATT_TYPE mdEntities::stringToCharAttType(std::string string) {
	if (string == "st_l")
		return ST_L;
	else if (string == "st_h")
		return ST_H;
	else if (string == "cr_l")
		return CR_L;
	else if (string == "cr_h")
		return CR_H;
	else if (string == "jm_l")
		return JM_L;
	else if (string == "jm_h")
		return JM_H;
	else if (string == "st_s1")
		return ST_S1;
	else if (string == "st_s2")
		return ST_S2;
	else if (string == "cr_s1")
		return CR_S1;
	else if (string == "cr_s2")
		return CR_S2;
	else if (string == "jm_s1")
		return JM_S1;
	else if (string == "jm_s2")
		return JM_S2;
	else if (string == "super")
		return SUPER;

		return NO_ATT;
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
	warrior.invencibility_on_wakeup = md_config.attribute("invencibility_on_wakeup").as_int();
	warrior.super_window = md_config.attribute("super_window").as_int();
	warrior.cancelability_window = md_config.attribute("cancelability_window").as_int();
	warrior.spin_speed = md_config.attribute("spin_speed").as_int();
	tmp = md_config.attribute("spin_object").as_string();
	warrior.spin_object = stringToItem(tmp);
	warrior.jm_s1_angle = md_config.attribute("jm_s1_angle").as_int();
	warrior.jm_s1_speed.x = md_config.attribute("jm_s1_speed_x").as_int();
	warrior.jm_s1_speed.y = md_config.attribute("jm_s1_speed_y").as_int();
	warrior.jm_s2_angle = md_config.attribute("jm_s2_angle").as_int();
	warrior.jm_s2_speed.x = md_config.attribute("jm_s2_speed_x").as_int();
	warrior.jm_s2_speed.y = md_config.attribute("jm_s2_speed_y").as_int();
	warrior.dive_kick_max_height = md_config.attribute("dive_kick_max_height").as_int();
	tmp = md_config.attribute("dive_kick_object").as_string();
	warrior.dive_kick_object = stringToItem(tmp);
	warrior.projectile_duration = md_config.attribute("projectile_duration").as_int();
	warrior.projectile_speed = md_config.attribute("projectile_speed").as_int();
	warrior.projectile_scale = md_config.attribute("projectile_scale").as_int();
	warrior.swordyuken_invencivility = md_config.attribute("swordyuken_invencivility").as_int();
	warrior.swordyuken_jump_power = md_config.attribute("swordyuken_jump_power").as_int();
	pugi::xml_node attack_data = md_config.child("attack_data");
	loadAttackDeffFromXML(attack_data.child("st_l"), warrior.st_l);
	loadAttackDeffFromXML(attack_data.child("st_h"), warrior.st_h);
	loadAttackDeffFromXML(attack_data.child("cr_l"), warrior.cr_l);
	loadAttackDeffFromXML(attack_data.child("cr_h"), warrior.cr_h);
	loadAttackDeffFromXML(attack_data.child("jm_l"), warrior.jm_l);
	loadAttackDeffFromXML(attack_data.child("jm_h"), warrior.jm_h);
	loadAttackDeffFromXML(attack_data.child("st_s1"), warrior.st_s1);
	loadAttackDeffFromXML(attack_data.child("st_s2"), warrior.st_s2);
	loadAttackDeffFromXML(attack_data.child("cr_s1"), warrior.cr_s1);
	loadAttackDeffFromXML(attack_data.child("cr_s2"), warrior.cr_s2);
	loadAttackDeffFromXML(attack_data.child("jm_s1"), warrior.jm_s1);
	loadAttackDeffFromXML(attack_data.child("jm_s2"), warrior.jm_s2);
	loadAttackDeffFromXML(attack_data.child("super"), warrior.super);
}
void mdEntities::loadAttackDeffFromXML(const pugi::xml_node& md_config, basic_attack_deff& attack) {
	std::string tmp;
	attack.pos_rel_char.x = md_config.attribute("pos_rel_char_x").as_int();
	attack.pos_rel_char.y = md_config.attribute("pos_rel_char_y").as_int();
	attack.hitbox.w = md_config.attribute("hitbox_w").as_int();
	attack.hitbox.h = md_config.attribute("hitbox_h").as_int();
	attack.active_time = md_config.attribute("active_time").as_int();
	attack.hitstun = md_config.attribute("hitstun").as_int();
	attack.blockstun = md_config.attribute("blockstun").as_int();
	attack.pushhit = md_config.attribute("pushhit").as_int();
	attack.pushblock = md_config.attribute("pushblock").as_int();
	attack.damage = md_config.attribute("damage").as_int();
	attack.knockdown = md_config.attribute("knockdown").as_bool();
	attack.juggle_speed.x = md_config.attribute("juggle_speed_x").as_int();
	attack.juggle_speed.y = md_config.attribute("juggle_speed_y").as_int();
	tmp = md_config.attribute("block_type").as_string();
	attack.block_type = stringToBlockType(tmp);
	tmp = md_config.attribute("type").as_string();
	attack.type = stringToCharAttType(tmp);
	attack.recovery = md_config.attribute("recovery").as_int();
	attack.animation_speed = md_config.attribute("animation_speed").as_float();
}