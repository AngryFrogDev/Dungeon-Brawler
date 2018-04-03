#ifndef _WIDGETS_
#define _WIDGETS_

#include "Module.h"
#include "ProjDefs.h"

#include "SDL\include\SDL.h"

enum ui_elem_type {
	NO_ELEM = 0,
	BUTTON,
	LABEL,
	BAR,
};

//Review button types
enum button_types {
	NO_BUTTON = 0,
	NEW_GAME,
	SETTINGS,
	CREDITS,
	EXIT,
	TWO_PLAYERS,
	FOUR_PLAYERS,
	MUSIC_VOL_UP,
	MUSIC_VOL_DOWN,
	SFX_VOL_UP,
	SFX_VOL_DOWN,
	BACK,
};

enum bar_types {
	NO_BAR = 0,
	HEALTH_BAR,
	SUPER_BAR,
	SWAP_BAR,
};

class Widgets {
public:
	Widgets(ui_elem_type type, std::pair<int, int> pos, Module* callback);
	virtual ~Widgets();

	virtual bool preUpdate() { return true; };
	virtual bool update(float dt) { return true; };
	
	virtual void draw() { return; };

public:
	ui_elem_type type;
	SDL_Rect world_area;
	std::pair<int, int> position;
	bool to_delete = false;

protected:
	Module* callback = nullptr;
	pugi::xml_document ui_config_file;
	pugi::xml_node ui_config;
};
#endif
