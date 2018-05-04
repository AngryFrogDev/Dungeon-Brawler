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
	void popUpP1Window();
	void popUpP2Window();
	void closeP1Window();
	void closeP2Window();
	void popUpGeneralWindow();
	void closeGeneralWindow();

public:
	uint	current_time = 0;
	uint	max_time = 0;

private:
	bool to_resume = false;

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
	//P1
	UiWindow* p1_window = nullptr;
	Labels* p1_pause_label = nullptr;
	Labels* p1_resume_label = nullptr;
	Labels* p1_char_sel_label = nullptr;
	Labels* p1_stage_sel_label = nullptr;
	Labels* p1_settings_label = nullptr;
	Labels* p1_main_menu_label = nullptr;
	Labels* p1_rematch_label = nullptr;
	
	Buttons* p1_resume_button = nullptr;
	Buttons* p1_char_sel_button = nullptr;
	Buttons* p1_stage_sel_button = nullptr;
	Buttons* p1_settings_button = nullptr;
	Buttons* p1_main_menu_button = nullptr;
	Buttons* p1_rematch_button = nullptr;
	//P2
	UiWindow* p2_window = nullptr;
	Labels* p2_pause_label = nullptr;
	Labels* p2_resume_label = nullptr;
	Labels* p2_char_sel_label = nullptr;
	Labels* p2_stage_sel_label = nullptr;
	Labels* p2_settings_label = nullptr;
	Labels* p2_main_menu_label = nullptr;

	Buttons* p2_resume_button = nullptr;
	Buttons* p2_char_sel_button = nullptr;
	Buttons* p2_stage_sel_button = nullptr;
	Buttons* p2_settings_button = nullptr;
	Buttons* p2_main_menu_button = nullptr;
	Buttons* p2_rematch_button = nullptr;

	//Combat scene timer
	Timer	scene_timer;
	
	pugi::xml_node window_node;
};
#endif

