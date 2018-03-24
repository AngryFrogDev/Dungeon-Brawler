#ifndef __PLAYER__
#define __PLAYER__

#include "mdInput.h"
#include "mdEntities.h"

class Character;

class Player {
public:
	Player();
	Player(Controller* controller); 

	~Player();

	// Will exectute the updates of the curr_character
	void update(SDL_Texture* graphics);
	// Function to assign a controller to the player								 
	void assignController(Controller* controller);
	// Function so assign a certain character to the player -> character_type: enum with all the character types			     
	void assignCharacter(CHAR_TYPE type); 
	
	void assignControlScheme(controller_scheme* new_scheme);

private:
	// Number of the controller being used by the player
	Controller* controller;
	// Instance of the current character being used -> Character: Base class for the characters							
	Character* curr_character;
	controller_scheme* scheme;
};

#endif //__PLAYER__