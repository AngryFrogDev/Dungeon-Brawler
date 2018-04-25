#ifndef _COMBATSCENE_
#define _COMBATSCENE_

#include "scene.h"
#include "ProjDefs.h"
#include "PerfTimer.h"

class combatScene : public scene
{
public:
	combatScene(bool active);
	virtual ~combatScene();
	
//	bool awake(const pugi::xml_node & md_config);
	bool start();
	bool update(float dt);

private:
	bool onEvent(Buttons* button);
	void loadSceneUi();

	void updateTimer();
	void checkPlayers();
	void loadSceneTextures();

	void assignFocus();
	void checkSceneInput();
	void popUpWindow();
	void closeWindow();

public:
	uint	current_time = 0;
	uint	max_time = 0;

private:
//	CharacterInfo player1;
//	CharacterInfo player2;

	//COMBAT UI
	Bars* health_bar1 = nullptr;
	Bars* health_bar2 = nullptr;
	Bars* super_bar1 = nullptr;
	Bars* super_bar2 = nullptr;
	Labels* timer = nullptr;

	SDL_Rect timer_rect;
	SDL_Rect character1_rect;
	SDL_Rect character2_rect;
	SDL_Rect character1_image;
	SDL_Rect character2_image;


	//WINDOWS && RELATED UI
	UiWindow* window = nullptr;
	Buttons* rematch = nullptr;
	Buttons* restart = nullptr;
	Buttons* resume = nullptr;
	Buttons* in_game_settings = nullptr;
	Buttons* char_sel = nullptr;
	Buttons* stage_sel = nullptr;
	Buttons* to_main_menu = nullptr;
	Labels* pause_label = nullptr;
	Labels* end_label = nullptr;
	Labels* rematch_l = nullptr;
	Labels* to_main_menu_l = nullptr;

	//Combat scene timer
	Timer	scene_timer;

	pugi::xml_node window_node;
};
#endif

