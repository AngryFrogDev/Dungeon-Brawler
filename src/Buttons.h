#ifndef _BUTTONS_
#define _BUTTONS_

#include "Widgets.h"
#include "mdGuiManager.h"

#include "SDL/include/SDL.h"


class Buttons : public Widgets {
public:
	Buttons(button_types type, std::pair<int, int> pos, Module* callback);
	virtual ~Buttons();

	bool preUpdate();

	void draw();
	void getSection(SDL_Rect idle_sec, SDL_Rect highl_sec, SDL_Rect clicked_sec, SDL_Rect disabled_sec);
	
private:
	void changeVisualState(controller_events event);
	void loadButtonsFromAtlas();

public:
	bool being_clicked = false;
	bool hovering = false;
	button_types button_type = NO_BUTTON;

private:
	SDL_Rect click_rect;
	SDL_Rect idle_rect;
	SDL_Rect highl_rect;
	SDL_Rect disabled_rect;
	SDL_Rect* current_rect = nullptr;
	
	bool enabled = true;

	uint click_sfx;
};
#endif
