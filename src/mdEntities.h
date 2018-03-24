#ifndef __MDENTITIES__
#define __MDENTITIES__

#include "Module.h"
#include "Character.h"
#include "mdTextures.h"

struct controller_scheme {
	CONTROLLER_BUTTON scheme[CHARACTER_INPUTS::MAX_INPUTS];
	std::string name;

	controller_scheme() : name("") {}
	controller_scheme(std::string name) : name(name){}
};

class Player;

class mdEntities : public Module {

public:
	mdEntities();
	virtual ~mdEntities();

	bool awake(const pugi::xml_node& md_config);
	bool preUpdate();
	bool cleanUp();

	// Creates a character and assigns it to a player
	void createCharacter(int player, CHAR_TYPE type);
	// Destroys all the players
	void destroyCharacters();

public:
	std::list<controller_scheme> schemes;

private:

	// Array that stores 4 players
	Player* players[4];
	//Graphics of the warrior
	SDL_Texture* warrior_graphics;


};

#endif //__MDENTITIES__