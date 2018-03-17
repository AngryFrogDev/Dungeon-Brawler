#include "Character.h"

class Warrior : public Character {
public:
	Warrior();
	~Warrior();
	void requestState();
	void updateState();		
	void update();

	void updateAnimationWithState(state state);
private:
	Animation idle, walk,crouch, light_attack, heavy_attack;

};

