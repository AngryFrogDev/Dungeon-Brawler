#include "mdEntities.h"
#include "mdAudio.h";
#include "mdMap.h"
#include "Application.h"
#include "Player.h"
#include <algorithm>

mdEntities::mdEntities(){
	name = "entities";
}


mdEntities::~mdEntities(){

}

bool mdEntities::awake(const pugi::xml_node & md_config) {
	bool ret = true;

	//Empty the array
	for (int i = 0; i < 2; i++)
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
	warrior_graphics2 = App->textures->load("Assets/warrior_2.png");
	mage_graphics = App->textures->load("Assets/mage.png");
	mage_graphics2 = App->textures->load("Assets/mage_2.png");
	mage_graphics3 = App->textures->load("Assets/mage_3.png");
	mage_graphics4 = App->textures->load("Assets/mage_4.png");
	rogue_graphics = App->textures->load("Assets/rogue.png");
	paladin_graphics = App->textures->load("Assets/paladin.png");

	//Allocate space for character sounds
	character_sounds = new Mix_Chunk*[CHAR_TYPE::CHAR_TOTAL * CHAR_SOUNDS::MAX_SOUNDS];
	for (int i = 0; i < CHAR_TYPE::CHAR_TOTAL * CHAR_SOUNDS::MAX_SOUNDS; ++i)
		character_sounds[i] = nullptr;

	// PROVISIONAL: This should be loaded from XML
	//Warrior
	character_sounds[CHAR_TYPE::WARRIOR * CHAR_SOUNDS::MAX_SOUNDS + CHAR_SOUNDS::S_JUMP] = App->audio->loadSFX("SFX/jump.wav");
	character_sounds[CHAR_TYPE::WARRIOR * CHAR_SOUNDS::MAX_SOUNDS + CHAR_SOUNDS::S_LIGHT_SWORD_BLOCK] = App->audio->loadSFX("SFX/light_sword_block.wav");
	character_sounds[CHAR_TYPE::WARRIOR * CHAR_SOUNDS::MAX_SOUNDS + CHAR_SOUNDS::S_HEAVY_SWORD_BLOCK] = App->audio->loadSFX("SFX/heavy_sword_block.wav");
	character_sounds[CHAR_TYPE::WARRIOR * CHAR_SOUNDS::MAX_SOUNDS + CHAR_SOUNDS::S_LIGHT_SWORD_WHIFF] = App->audio->loadSFX("SFX/light_sword_whiff.wav");
	character_sounds[CHAR_TYPE::WARRIOR * CHAR_SOUNDS::MAX_SOUNDS + CHAR_SOUNDS::S_HEAVY_SWORD_WHIFF] = App->audio->loadSFX("SFX/heavy_sword_whiff.wav");
	character_sounds[CHAR_TYPE::WARRIOR * CHAR_SOUNDS::MAX_SOUNDS + CHAR_SOUNDS::S_LIGHT_SWORD_IMPACT] = App->audio->loadSFX("SFX/light_sword_impact.wav");
	character_sounds[CHAR_TYPE::WARRIOR * CHAR_SOUNDS::MAX_SOUNDS + CHAR_SOUNDS::S_HEAVY_SWORD_IMPACT] = App->audio->loadSFX("SFX/heavy_sword_impact.wav");
	character_sounds[CHAR_TYPE::WARRIOR * CHAR_SOUNDS::MAX_SOUNDS + CHAR_SOUNDS::S_STANDING_SPECIAL_1] = App->audio->loadSFX("SFX/standing_special_1.wav");
	character_sounds[CHAR_TYPE::WARRIOR * CHAR_SOUNDS::MAX_SOUNDS + CHAR_SOUNDS::S_STANDING_SPECIAL_2] = App->audio->loadSFX("SFX/standing_special_2.wav");
	character_sounds[CHAR_TYPE::WARRIOR * CHAR_SOUNDS::MAX_SOUNDS + CHAR_SOUNDS::S_JUMPING_SPECIAL_1] = App->audio->loadSFX("SFX/jumping_special_1.wav");;
	character_sounds[CHAR_TYPE::WARRIOR * CHAR_SOUNDS::MAX_SOUNDS + CHAR_SOUNDS::S_CROUCHING_SPECIAL_1] = App->audio->loadSFX("SFX/crouching_special_1.wav");;
	character_sounds[CHAR_TYPE::WARRIOR * CHAR_SOUNDS::MAX_SOUNDS + CHAR_SOUNDS::S_CROUCHING_SPECIAL_2] = App->audio->loadSFX("SFX/crouching_special_2.wav");;
	character_sounds[CHAR_TYPE::WARRIOR * CHAR_SOUNDS::MAX_SOUNDS + CHAR_SOUNDS::S_DEATH] = App->audio->loadSFX("SFX/man_death.wav");
	character_sounds[CHAR_TYPE::WARRIOR * CHAR_SOUNDS::MAX_SOUNDS + CHAR_SOUNDS::S_SUPER] = App->audio->loadSFX("SFX/super.wav");

	traning = false;
	show = true;
	paused = false;

	camera_movement = 4; //Should be loaded from XML

	return ret;
}

bool mdEntities::preUpdate() {
	bool ret = true;

	if (players[0] == nullptr) //It crashed if there were no playres
		return true;

	if (players[0]->getController() == nullptr) { // Shouldn't this be done less often?
		assignControllers();
	}

	for (int i = 0; i < 2; i++) { 
		if (players[i] != nullptr && players[i]->getCurrCharacter() != nullptr){
			switch (players[i]->getCurrCharacter()->getType()) {
				case WARRIOR:
					switch (players[i]->getCurrCharacter()->skin_id){
						case 0:
							players[i]->update(warrior_graphics); 
							break;
						case 1:
							players[i]->update(warrior_graphics2);
							break;
					}
					break;
				case MAGE:
					switch (players[i]->getCurrCharacter()->skin_id) {
					case 0:
						players[i]->update(mage_graphics);
						break;
					case 1:
						players[i]->update(mage_graphics2);
						break;
					case 2:
						players[i]->update(mage_graphics3);
						break;
					case 3:
						players[i]->update(mage_graphics4);
						break;
					}
					break;
				case ROGUE:
					switch (players[i]->getCurrCharacter()->skin_id) {
					case 0:
						players[i]->update(rogue_graphics);
						break;
					}
					break;
				case PALADIN:
					switch (players[i]->getCurrCharacter()->skin_id) {
					case 0:
						players[i]->update(paladin_graphics);
						break;
					}
					break;

			}

		}
		
	}

	return ret;
}

bool mdEntities::postUpdate()
{
	if (players[0] == nullptr) 
		return true;

	if(players[0]->getCurrCharacter() != nullptr && players[0]->getCurrCharacter() != nullptr && allowFlip()){ 
		automaticFlip();
	}

	return true;
}

bool mdEntities::cleanUp() {
	bool ret = true;
	destroyCharacters();
	return ret;
}
void mdEntities::createPlayer(int player) {

	if (players[player] == nullptr){
		players[player] = new Player();
		players[player]->assignControlScheme(controller_schemes.front());
		players[player]->assignKeyboardScheme(keyboard_schemes.front());
	}
}

void mdEntities::destroyCharacters() {

	for (int i = 0; i < 2; i++) {
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

bool mdEntities::moveCamera(bool movingLeft) {
	bool ret = true;
	if (movingLeft) {
		if (App->render->camera.x == 0) ret = false;
		else {
			int target_x = std::max(App->render->camera.x - camera_movement, 0);
			for (int i = 0; i < 2 && ret; ++i) {
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
		if (App->render->camera.x + App->render->camera.w == map_limit) ret = false;
		else {
			int target_x = std::min(App->render->camera.x + camera_movement + App->render->camera.w, map_limit) - App->render->camera.w;
			for (int i = 0; i < 2 && ret; ++i) {
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

	bool flip[2];
	if (players[0]->getPos().x < players[1]->getPos().x) {
		flip[0] = false;
		flip[1] = true;
	}
	else {
		flip[0] = true;
		flip[1] = false;
	}

	for (int i = 0; i < 2; i++) 
		players[i]->setFlip(flip[i]);
	

	return ret;
}

bool mdEntities::allowFlip() 	{

	// PROVISIONAL: Every character should have specified with which states flip should not be executed
	bool do_flip = true;
	for (int i = 0; i < 2; i++) {
		if (players[i] == nullptr)
			continue;
		Character* character = players[i]->getCurrCharacter();
		if (character)
		{
			if (character->notAllowFlip())
				do_flip = false;
		}

	}

	return do_flip;
}

void mdEntities::assignControllers() {
	std::list<Controller*> controllers = App->input->getController();
	int counter = 0;
	for (auto it = controllers.begin(); it != controllers.end() || counter > 4; it++) {
		if (players[counter] == nullptr)
			break;

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
	fillFromXML(md_config.child("warrior"), warrior);
	fillFromXML(md_config.child("mage"), mage);
	fillFromXML(md_config.child("rogue"), rogue);
	fillFromXML(md_config.child("paladin"), paladin);
}
void mdEntities::fillFromXML(const pugi::xml_node& md_config, character_deff& character) {
	std::string tmp;
	tmp = md_config.attribute("type").as_string("");
	character.type = stringToCharType(tmp);
	character.scale = md_config.attribute("scale").as_int(0);
	character.gravity = md_config.attribute("gravity").as_float(0);
	character.walk_speed = md_config.attribute("walk_speed").as_int(0);
	character.jump_power.x = md_config.attribute("jump_power_x").as_int(0);
	character.jump_power.y = md_config.attribute("jump_power_y").as_int(0);
	character.max_life = md_config.attribute("max_life").as_int(0);
	character.max_super_gauge = md_config.attribute("max_super_gauge").as_int(0);
	character.super_gauge_gain_hit = md_config.attribute("super_gauge_gain_hit").as_int(0);
	character.super_gauge_gain_block = md_config.attribute("super_gauge_gain_block").as_int(0);
	character.super_gauge_gain_strike = md_config.attribute("super_gauge_gain_strike").as_int(0);
	character.standing_hurtbox_size.x = md_config.attribute("standing_hurtbox_size_x").as_int(0);
	character.standing_hurtbox_size.y = md_config.attribute("standing_hurtbox_size_y").as_int(0);
	character.crouching_hurtbox_offset = md_config.attribute("crouching_hurtbox_offset").as_int(0);
	character.invencibility_on_wakeup = md_config.attribute("invencibility_on_wakeup").as_int(0);
	character.super_window = md_config.attribute("super_window").as_int(0);
	character.cancelability_window = md_config.attribute("cancelability_window").as_int(0);
	loadAttackListFromXML(md_config.child("non_flip_attacks"), character.non_flip_attacks);

	// Warrior
	character.spin_speed = md_config.attribute("spin_speed").as_int(0);
	character.improved_spin_speed = md_config.attribute("improved_spin_speed").as_int(0);
	character.improved_spin_recovery = md_config.attribute("improved_spin_recovery").as_int(0);
	character.jm_s1_angle = md_config.attribute("jm_s1_angle").as_int(0);
	character.jm_s1_speed.x = md_config.attribute("jm_s1_speed_x").as_int(0);
	character.jm_s1_speed.y = md_config.attribute("jm_s1_speed_y").as_int(0);
	character.jm_s2_angle = md_config.attribute("jm_s2_angle").as_int(0);
	character.jm_s2_speed.x = md_config.attribute("jm_s2_speed_x").as_int(0);
	character.jm_s2_speed.y = md_config.attribute("jm_s2_speed_y").as_int(0);
	character.dive_kick_object_mult = md_config.attribute("dive_kick_object_mult").as_float(0.0);
	character.dive_kick_max_height = md_config.attribute("dive_kick_max_height").as_int(0);
	tmp = md_config.attribute("dive_kick_object").as_string("");
	character.projectile_speed = md_config.attribute("projectile_speed").as_int(0);
	character.projectile_duration = md_config.attribute("projectile_duration").as_int(0);
	character.swordyuken_invencivility = md_config.attribute("swordyuken_invencivility").as_int(0);
	character.swordyuken_jump_power = md_config.attribute("swordyuken_jump_power").as_int(0);

	// Mage
	character.fireball_speed = md_config.attribute("fireball_speed").as_int(0);
	character.fireball_duration = md_config.attribute("fireball_duration").as_int(0);
	character.fireball_emitter_offset.x = md_config.attribute("fireball_emitter_offset_x").as_int(0);
	character.fireball_emitter_offset.y = md_config.attribute("fireball_emitter_offset_y").as_int(0);
	character.air_fireball_angle = md_config.attribute("air_fireball_angle").as_int(0);
	character.air_fireball_max_height = md_config.attribute("air_fireball_max_height").as_int(0);
	character.air_fireball_backfire.x = md_config.attribute("air_fireball_backfire_x").as_int(0);
	character.air_fireball_backfire.y = md_config.attribute("air_fireball_backfire_y").as_int(0);
	character.air_fireball_speed.x = md_config.attribute("air_fireball_speed_x").as_int(0);
	character.air_fireball_speed.y = md_config.attribute("air_fireball_speed_y").as_int(0);
	character.double_jump_power.x = md_config.attribute("double_jump_power_x").as_int(0);
	character.double_jump_power.y = md_config.attribute("double_jump_power_y").as_int(0);
	character.meteorits = md_config.attribute("meteorits").as_int(0);
	character.meteorits_life = md_config.attribute("meteorits_life").as_int(0);
	character.first_meteorit_height = md_config.attribute("first_meteorit_height").as_int(0);
	character.meteorits_offset.x = md_config.attribute("meteorits_offset_x").as_int(0);
	character.meteorits_offset.y = md_config.attribute("meteorits_offset_y").as_int(0);
	character.meteorits_speed.x = md_config.attribute("meteorits_speed_x").as_int(0);
	character.meteorits_speed.y = md_config.attribute("meteorits_speed_y").as_int(0);
	character.meteorits_rows = md_config.attribute("meteorits_rows").as_int(0);
	character.meteorits_rows_offset = md_config.attribute("meteorits_rows_offset").as_int(0);

	// Pladin
	character.parry_reaction_invencivility = md_config.attribute("parry_reaction_invencivility").as_int(0);
	character.parry_healing = md_config.attribute("parry_healing").as_int(0);
	character.parry_duration = md_config.attribute("parry_duration").as_int(0);
	character.miss_parry_recovery = md_config.attribute("miss_parry_recovery").as_int(0);
	character.st_s2_speed = md_config.attribute("st_s2_speed").as_int(0);
	character.st_s2_invencivility = md_config.attribute("st_s2_invencivility").as_int(0);
	character.hammer_duration = md_config.attribute("hammer_duration").as_int(0);
	character.hammer_speed.x = md_config.attribute("hammer_speed_x").as_int(0);
	character.hammer_speed.y = md_config.attribute("hammer_speed_y").as_int(0);
	character.slam_speed.x = md_config.attribute("slam_speed_x").as_int(0);
	character.slam_speed.y = md_config.attribute("slam_speed_y").as_int(0);
	character.slam_max_height = md_config.attribute("slam_max_height").as_int(0);
	character.air_hammer_speed = md_config.attribute("air_hammer_speed").as_int(0);
	character.air_hammer_duration = md_config.attribute("air_hammer_duration").as_int(0);
	


	pugi::xml_node attack_data = md_config.child("attack_data");
	loadAttackDeffFromXML(attack_data.child("st_l"), character.st_l);
	loadAttackDeffFromXML(attack_data.child("st_h"), character.st_h);
	loadAttackDeffFromXML(attack_data.child("cr_l"), character.cr_l);
	loadAttackDeffFromXML(attack_data.child("cr_h"), character.cr_h);
	loadAttackDeffFromXML(attack_data.child("jm_l"), character.jm_l);
	loadAttackDeffFromXML(attack_data.child("jm_h"), character.jm_h);
	loadAttackDeffFromXML(attack_data.child("st_s1"), character.st_s1);
	loadAttackDeffFromXML(attack_data.child("st_s2"), character.st_s2);
	loadAttackDeffFromXML(attack_data.child("cr_s1"), character.cr_s1);
	loadAttackDeffFromXML(attack_data.child("cr_s2"), character.cr_s2);
	loadAttackDeffFromXML(attack_data.child("jm_s1"), character.jm_s1);
	loadAttackDeffFromXML(attack_data.child("jm_s2"), character.jm_s2);
	loadAttackDeffFromXML(attack_data.child("super"), character.super);
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
void mdEntities::loadAttackListFromXML(const pugi::xml_node& md_config, std::list<CHAR_ATT_TYPE>& attack_list) {
	pugi::xml_node iterator = md_config.first_child();

	while (iterator != nullptr) {
		attack_list.push_back(stringToCharAttType(iterator.attribute("value").as_string()));
 		iterator = iterator.next_sibling();
	}
}