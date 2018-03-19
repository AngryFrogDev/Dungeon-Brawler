#include "Character.h"

class Warrior : public Character {
public:
	Warrior();
	~Warrior();
	void requestState();
	void updateState();		
	void update();

	void updateAnimationWithState(state state);
	void updateAnimationWithAttack(attack_type attack);

private:
	Animation idle, walk_forward, walk_back,crouch, light_attack, heavy_attack;

};

