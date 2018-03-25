#ifndef _BUTTONS_
#define _BUTTONS_

#include "Widgets.h"
#include "mdGuiManager.h"

#include "SDL/include/SDL.h"


class Buttons : public Widgets {
public:
	Buttons(button_types type, std::pair<int, int> pos, Module* callback);
	virtual ~Buttons();

	void draw();
	void getSection(SDL_Rect idle_sec, SDL_Rect highl_sec, SDL_Rect clicked_sec, SDL_Rect disabled_sec);
	void setButtonType(button_types type);
	void setArea(uint w, uint h);

private:
	void changeVisualState(controller_events event);

public:
	button_types button_type;
	bool being_clicked = false;
	bool hovering = false;

private:
	SDL_Rect click_rect;
	SDL_Rect idle_rect;
	SDL_Rect highl_rect;
	SDL_Rect disabled_rect;
	SDL_Rect* current_rect;

	bool enabled = true;

	uint click_sfx;
};
#endif
