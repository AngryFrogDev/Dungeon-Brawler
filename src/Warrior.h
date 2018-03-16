#include "Character.h"

class Warrior : public Character {
public:
	Warrior();
	~Warrior();
	void requestState();
	virtual void updateState();		
	virtual void update();
private:
	Animation idle;

};

