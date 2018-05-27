#ifndef __PLAYER__
#define __PLAYER__

#include "mdInput.h"
#include "mdEntities.h"

class Character;
class Widgets;
enum CHARACTER_INPUTS;

class Player {
public:
	Player();
	Player(Controller* controller); 

	~Player();

	// Will exectute the updates of the curr_character
	void update(SDL_Texture* graphics);
	// Function to assign a controller to the player								 
	void assignController(Controller* controller);
	// Function so assign a certain character to the player		     
	void createAndAssignCharacter(CHAR_TYPE type, ITEMS item, bool player, int skin); 
	
	void assignControlScheme(controller_scheme* new_scheme);

	void assignKeyboardScheme(keyboard_scheme* new_scheme);

	iPoint getPos();
	bool getInput(CHARACTER_INPUTS input, KEY_STATE state);

	Character* getCurrCharacter();
	void removeCharacters();//Destroy current character from each player

	void setFlip(bool flip);

	Controller* getController() const;

public:
	Widgets* focus = nullptr;

private:
	// Number of the controller being used by the player
	Controller* controller;
	// Instance of the current character being used -> Character: Base class for the characters							
	Character* curr_character;
	controller_scheme* player_controller_scheme;
	keyboard_scheme*   player_keyboard_scheme;



	
	
};

#endif //__PLAYER__
