#include "Animation.h"
#include "Application.h"
#include "mdInput.h"
#include "p2Point.h"
#include "mdRender.h"
enum character_type {
	DEF_CHAR,
	WARRIOR,
	MAGE,
	ROGUE,
	PALADIN
};

enum attack_type {
	NO_ATT,
	CR_L,
	CR_H,
	CR_S1,
	CR_S2,
	
	ST_L,
	ST_H,
	ST_S1,
	ST_S2,

	JM_L,
	JM_H,
	JM_S1,
	JM_S2,
};
enum state {
	DEF_STATE,
	IDLE,
	WALKING_BACK,
	WALKING_FORWARD,
	CROUCHING,
	JUMPING,
	ATTACKING,

	BLOCKING,
	HIT,
	JUGGLE,
	KNOCKDOWN
};

enum input {
	DEF_INPUT,
	UP,
	DOWN,
	LEFT,
	RIGHT,
	LIGHT_ATTACK,
	HEAVY_ATTACK,
	SPECIAL_1,
	SPECIAL_2,
	GRAB
};

struct input_values {

	input keyb_down;
	input keyb_up;
	input keyb_left;
	input keyb_right;
	input keyb_a; //Light
	input keyb_s; //Heavy
};

struct active_inputs {
	bool up, down, left, right, light_attack, heavt_attack, special_1, special_2, grab = false;
};

struct attack_deff {
	Animation anim;
};

class Character
{
public:
	Character();
	~Character();
	// Recieve input
	void recieveInput();
	// Request "wanted_state" depending on input and "current_state"								 
	virtual void requestState(); 
	// Substitute wanted_state with current_state if possible					
	virtual void updateState();
	// Logic update depending on state (animation, position)							
	virtual void update();								 
	// Draw the entity
	void draw(SDL_Texture* graphic);

	// Execute an attack depending on an attack deffinition (collider, animation, recovery...) -> attack_deff: Struct that holds all the properties of an attack										
	void doAttack(attack_deff attack);					 
	//void onCollision(Collider* collider);
	
	// Get attack data from this entity depending on an attack_type -> attack_type: Enum with the basic attack types (light/heavy and crouching/standing/jumping
	attack_deff getAttackData(attack_type type);

	// PROVISIONAL: Returns if the state is neutral or not 
	bool isNeutralState(state state);
private:
	void activateInput(input requested_input);
protected:
	//Character type
	character_type type;

	//Position
	iPoint position;

	// Current lifepoints
	int life; 
	// Maximum lifepoints										
	int max_life;	

	 // Walk speed
	int walk_speed;	

	// If the character is oriented left or right (true for left, false for right)
	bool fliped;

	// Gravity used for the jump
	int gravity;	 
	// Initial degrees for jump arc								
	int degree_of_jumping;		

	// Entity collider
	//Collider* hurtbox;								 
	
	//Input assigned values
	input_values assigned_inputs;
	//Directions being pressed
	active_inputs inputs_pressed;
	

	//State requested in this frame
	state wanted_state;
	//Current state
	state current_state;
	//Current attack
	attack_type attack_doing;

	//Animation to draw from
	Animation* current_animation;

	//Struct that stores attack data loaded from xml
	attack_deff standing_light;

	


};

