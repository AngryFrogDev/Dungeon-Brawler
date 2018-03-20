#include "Module.h"
#include "Player.h"
#include "mdTextures.h"

class mdEntities : public Module {

public:
	mdEntities();
	virtual ~mdEntities();

	bool awake(const pugi::xml_node& md_config);
	bool preUpdate();
	bool cleanUp();

	// Creates a character and assigns it to a player
	void createCharacter(int player, character_type type);
	// Destroys all the players
	void destroyCharacters();

private:

	// Array that stores 4 players
	Player* players[4];
	//Graphics of the warrior
	SDL_Texture* warrior_graphics;


};