#ifndef _BARS_
#define _BARS_

#include "Widgets.h"
#include "mdGuiManager.h"
#include "SDL/include/SDL.h"
#include "p2Point.h"
#include "Animation.h"

class Bars : public Widgets {
public:
	Bars(bar_types type, std::pair<int, int> pos, bool _flipped, int target_player, Module* callback);
	~Bars();
	bool preUpdate();
	void draw();
	void getSection(SDL_Rect rect, SDL_Rect gauge);
	void updateBarGauge();
	void calculateBarGauge();
	void loadGuiFromAtlas();
	void setAnimation();


private:
	bar_types bar_type = NO_BAR;

	SDL_Rect gauge_rect;
	SDL_Rect bar_rect;
	SDL_Rect current_gauge_rect;
	SDL_Rect full_gauge; 
	Animation* curr_anim = nullptr;
	Animation super_anim; 

	int current_gauge = 0;
	int max_gauge = 0;
	int last_gauge = 0;
	int aux_bar_pos = 0;
	int target_player;
	iPoint relative_pos = { 0,0 };
	bool flipped;

};


#endif // !_BARS_