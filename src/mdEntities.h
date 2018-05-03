#ifndef __MDENTITIES__
#define __MDENTITIES__

#include "Module.h"
#include "Character.h"
#include "mdTextures.h"
#define MAX_KEYS 300


struct controller_scheme {
	CONTROLLER_BUTTON scheme[CHARACTER_INPUTS::MAX_INPUTS];
	std::string name;

	controller_scheme() : name("") {}
	controller_scheme(std::string name) : name(name){}
};
struct keyboard_scheme {
	SDL_Scancode scheme[CHARACTER_INPUTS::MAX_INPUTS];
	std::string name;

	keyboard_scheme() : name("") {}
	keyboard_scheme(std::string name) : name(name) {}
};
struct character_deff {
	CHAR_TYPE type;
	int scale;
	float gravity;
	int walk_speed;
	iPoint jump_power;
	int max_life;
	int max_super_gauge;
	int super_gauge_gain_hit;
	int super_gauge_gain_block;
	int super_gauge_gain_strike;
	//int left_x_limit;
	//int right_x_limit;
	iPoint standing_hurtbox_size;
	int crouching_hurtbox_offset;
	int invencibility_on_wakeup;
	int super_window;
	int cancelability_window;
	basic_attack_deff st_l, st_h, cr_l, cr_h, jm_l, jm_h, st_s1, st_s2, cr_s1, cr_s2, jm_s1, jm_s2, super;
	std::list<CHAR_ATT_TYPE> non_flip_attacks;

	// Warrior variables
	int spin_speed;
	int improved_spin_speed;
	int improved_spin_recovery;
	int jm_s1_angle;
	iPoint jm_s1_speed;
	int jm_s2_angle;
	iPoint jm_s2_speed;
	int dive_kick_max_height;
	int projectile_duration;
	int projectile_speed;
	int swordyuken_invencivility;
	int swordyuken_jump_power;

	// Mage variables
	int fireball_speed;
	int fireball_duration;
	iPoint fireball_emitter_offset;
	int air_fireball_angle;
	int air_fireball_max_height;
	iPoint air_fireball_backfire;
	iPoint air_fireball_speed;
	iPoint double_jump_power;
	int meteorits_life;
	int first_meteorit_height;
	iPoint meteorits_offset;
	int meteorits;
	int meteorits_rows;
	int meteorits_rows_offset;
	iPoint meteorits_speed;
};

class Player;

class mdEntities : public Module {

public:
	mdEntities();
	virtual ~mdEntities();

	bool awake(const pugi::xml_node& md_config);
	bool preUpdate();
	bool postUpdate() override;
	bool cleanUp();

	// Creates a character and assigns it to a player
	void createPlayer(int player);

	void destroyCharacters();
	void removeCharacters();//Destroy current character from each player

	void assignControls();
	void assignControllers();
	void assignPartners();

	bool moveCamera(bool movingLeft);

	character_deff warrior, mage;
	bool traning;
	bool paused;
	bool show;

	KEY_STATE stringToKeystate(std::string string);
	CHAR_TYPE stringToCharType(std::string string);
	ITEMS stringToItem(std::string string);
	BLOCK_TYPE stringToBlockType(std::string string);
	CHAR_ATT_TYPE stringToCharAttType(std::string string);

public:
	Player * players[4]; //I made it public to make the partner system work
	std::list<controller_scheme> controller_schemes;
	std::list<keyboard_scheme>	 keyboard_schemes;
	KEY_STATE attack_input; 
	SDL_Texture* mage_graphics; // So mage can blit arcane symbol

private:
	//if it returns false something wrong happened
	bool automaticFlip();
	bool allowFlip();
	void loadCharactersFromXML(const pugi::xml_node& md_config);
	void fillFromXML(const pugi::xml_node& md_config, character_deff& character);	
	void loadAttackListFromXML(const pugi::xml_node& md_config, std::list<CHAR_ATT_TYPE>& attack_list);
	void loadAttackDeffFromXML(const pugi::xml_node& md_config, basic_attack_deff& attack);
private:
	int camera_movement;
	//Graphics of the warrior
	SDL_Texture* warrior_graphics;
	SDL_Texture* warrior_graphics2;


	pugi::xml_document entities_config_doc;
};

#endif //__MDENTITIES__