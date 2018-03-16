

enum character_type {
	NONE,
	WARRIOR,
	MAGE,
	ROGUE,
	PALADIN
};
enum attack_type {
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

class Character
{
public:
	Character();
	~Character();
	virtual void update();
	virtual void draw();
	//void doAttack(Animation anim, attack_deff attack);   attack_deff: Struct that holds all the properties of an attack
	//void onCollision(Collider* collider)
	//AttackDeff getAttackData(attack_type type);		   attack_type: Enum with the basic attack types (light/heavy and crouching/standing/jumping

private:
	int life;											// Current lifepoints
	int max_life;										// Maximum lifepoints

	int walk_speed;										// Walk speed

	int gravity;										// Gravity used for the jump
	int degree_of_jumping;								// Initial degrees for jump arc
	//Collider* hurtbox;

};

