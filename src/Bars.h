#ifndef _BARS_
#define _BARS_

#include "Widgets.h"
#include "mdGuiManager.h"
#include "SDL/include/SDL.h"

class Bars : public Widgets {
public:
	Bars(bar_types type, std::pair<int, int> pos, Module* callback);
	~Bars();
	void draw();
	void setType(bar_types type);
	void getSection(SDL_Rect rect);

public:
	bar_types bar_type;

private:
	uint current_gauge;
	uint max_gauge;
	SDL_Texture* bar_tex;
	SDL_Rect max_bar_rect;
	SDL_Rect* current_bar_rect;

};


#endif // !_BARS_