#include "Character.h"

class Player
{
public:
	Player();
	Player(int controller_id, character_type type); //Will use "assignCharacter" and "assignController" functions
	~Player();

	// Will exectute the "update()" of the "curr_character"
	void update();
	// Function to assign a controller to the player								 
	void assignController(int id);
	// Function so assign a certain character to the player -> character_type: enum with all the character types			     
	void assignCharacter(character_type type); 

private:

	// Number of the controller being used by the player
	int controller_id;
	// Instance of the current character being used -> Character: Base class for the characters							
	Character curr_character;					 
};

