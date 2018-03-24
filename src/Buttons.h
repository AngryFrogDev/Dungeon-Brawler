#ifndef _BUTTONS_
#define _BUTTONS_

#include "Widgets.h"
#include "mdGuiManager.h"

#include "SDL/include/SDL.h"

//Review button types
enum button_types {
	NO_BUTTON = 0,
	NEW_GAME,
	SETTINGS,
	CREDITS,
	EXIT,
	MUSIC_VOL_UP,
	MUSIC_VOL_DOWN,
	SOUND_VOL_UP,
	SOUND_VOL_DOWN,
	BACK,
};

class Buttons : public Widgets {
public:
	Buttons();
	virtual ~Buttons();

	void draw();
	bool mouseOver(const SDL_Rect& button);
	void getSection(SDL_Rect idle_sec, SDL_Rect highl_sec, SDL_Rect clicked_sec, SDL_Rect disabled_rec);
	void setButtonType(button_types type);
	void setArea(uint w, uint h);

private:
	void changeVisualState(controller_events event);

public:
	button_types button_type;
	bool being_clicked = false;
	bool hovering = false;

private:
	SDL_Rect clic_rect;
	SDL_Rect idle_rect;
	SDL_Rect highl_rect;
	SDL_Rect disabled_rect;
	SDL_Rect* current_rect;

	bool enabled = true;

	uint click_sfx;
};
#endif
