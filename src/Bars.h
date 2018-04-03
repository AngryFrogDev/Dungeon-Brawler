#ifndef _BARS_
#define _BARS_

#include "Widgets.h"
#include "mdGuiManager.h"
#include "SDL/include/SDL.h"

class Bars : public Widgets {
public:
	Bars(bar_types type, std::pair<int, int> pos, Module* callback);
	~Bars();
	bool preUpdate();
	void draw();
	void getSection(SDL_Rect rect, SDL_Rect gauge);
	void updateBarGauge(uint new_gauge);

private:
	bar_types bar_type = NO_BAR;

	SDL_Rect current_gauge_rect;
	SDL_Rect max_gauge_rect;
	SDL_Rect current_bar_rect;
	SDL_Rect max_bar_rect;

	SDL_Texture* bar_tex = nullptr;

	int gauge_y_position;

};


#endif // !_BARS_