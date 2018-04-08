#ifndef _BARS_
#define _BARS_

#include "Widgets.h"
#include "mdGuiManager.h"
#include "SDL/include/SDL.h"
#include "p2Point.h"

class Bars : public Widgets {
public:
	Bars(bar_types type, std::pair<int, int> pos, bool _flipped, Module* callback);
	~Bars();
	bool preUpdate();
	void draw();
	void getSection(SDL_Rect rect, SDL_Rect gauge);
	void updateBarGauge(uint new_gauge);
	void loadGuiFromAtlas();

private:
	bar_types bar_type = NO_BAR;

	SDL_Rect gauge_rect;
	SDL_Rect bar_rect;

	int gauge_y_position;
	iPoint relative_pos = { 0,0 };
	bool flipped;

};


#endif // !_BARS_