#include "Character.h"

class Player
{
public:
	Player();
	//Player(int controller_id, character char); //Will use "assignCharacter" and "assignController" functions
	~Player();

	void update();								 // Will exectute the "update()" of the "curr_character"
	void assignController(int id);			     // Function to assign a controller to the player
	void assignCharacter(character_type type); // Function so assign a certain character to the player -> character_type: enum with all the character types

private:

	int controller_id;							 // Number of the controller being used by the player
	Character curr_character;					 // Instance of the current character being used -> Character: Base class for the characters
};

