#ifndef _WIDGETS_
#define _WIDGETS_

#include "Module.h"
#include "ProjDefs.h"

#include "SDL\include\SDL.h"

class scene;


enum ui_elem_type {
	NO_ELEM = 0,
	BUTTON,
	LABEL,
	BAR,
	WINDOW,
};

//Review button types
enum button_types {
	NO_BUTTON = 0,
	ONE_V_ONE,
	TRAINING,
	SETTINGS,
	CREDITS,
	GAME_EXIT,
	MUSIC_VOL_UP,
	MUSIC_VOL_DOWN,
	SFX_VOL_UP,
	SFX_VOL_DOWN,
	MATCH_END_REMATCH,
	IN_GAME_SETTINGS,
	IN_GAME_CHAR_SEL,
	IN_GAME_STAGE_SEL,
	IN_GAME_MAIN_MENU,
	MATCH_END_CHAR_SEL,
	MATCH_END_STAGE_SEL,
	MATCH_END_SETTINGS,
	MATCH_END_MAIN_MENU,
	SELECT_WARRIOR,
	SELECT_MAGE,
	SELECT_ROGUE,
	SELECT_PALADIN,
	SELECT_ITEM1,
	SELECT_ITEM2,
	SELECT_STAGE_1,
	SELECT_STAGE_2, 
	SELECT_STAGE_3,
	SHOW_COLLIDERS,
	HIDE_COLLIDERS,
	BACK,
	PARALLAX,
};

enum button_size {
	NO_SIZE = 0,
	SMALL,
	MEDIUM,
	LARGE,
	CHARACTER_SELECTION,
	OBJECT_SELECTION,
	STAGE_SELECTION,
};

enum bar_types {
	NO_BAR = 0,
	HEALTH_BAR,
	SUPER_BAR,
	MUSIC_VOL_BAR,
	SFX_VOL_BAR,
};

enum window_type {
	NO_WINDOW = 0,
	PAUSE,
	MATCH_END, 
	OBJ_SELECTION,
};

class Widgets {
public:
	Widgets(ui_elem_type type, std::pair<int, int> pos, scene* callback);
	virtual ~Widgets();

	virtual bool preUpdate() { return true; };
	virtual bool update(float dt) { return true; };
	
	virtual void draw() { return; };
	virtual	void loadGuiFromAtlas() { return; };


public:
	ui_elem_type type;
	SDL_Rect world_area;
	std::pair<int, int> position;
	bool to_delete = false;
	int focus_id = 0;
	bool stop_focus = false;

protected:
	scene* callback = nullptr;

	//Nodes
	pugi::xml_document config_doc;
	pugi::xml_node config;
	pugi::xml_node data;
};
#endif
