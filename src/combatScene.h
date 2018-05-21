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
	
	bool start();
	bool update(float dt);

private:
	bool onEvent(Buttons* button);
	void loadSceneUi();

	void updateSceneTimer();
	void loadSceneTextures();
	void setRects();

	void assignFocus();
	void checkSceneInput();
	void popUpP1Window();
	void popUpP2Window();
	void closeP1Window();
	void closeP2Window();
	void popUpGeneralWindow();
	void closeGeneralWindow();
	void resetSceneValues();
	void checkTimers();
	void manageRounds();

public:
	uint	current_time = 0;
	uint	max_time = 0;
	uint	rounds_left = 0;
	uint	max_rounds = 0;

private:
	//PRE-COMBAT
	SDL_Texture* announcer_textures = nullptr;
	SDL_Rect round1_rect;
	SDL_Rect round2_rect;
	SDL_Rect round3_rect;
	SDL_Rect fight_rect;
	SDL_Rect ko_rect;
	SDL_Rect perfect_rect;
	SDL_Rect player1_wins_announcer_rect;
	SDL_Rect player2_wins_announcer_rect;
	SDL_Rect draw_announcer_rect;
	SDL_Rect* current_round = nullptr;
	SDL_Rect* round_end = nullptr;
	SDL_Rect* combat_end = nullptr;

	//COMBAT UI
	Bars* health_bar1 = nullptr;
	Bars* health_bar2 = nullptr;
	Bars* super_bar1 = nullptr;
	Bars* super_bar2 = nullptr;
	Labels* timer = nullptr;

	SDL_Rect still_round_indicator;
	SDL_Rect won_round_indicator;
	SDL_Rect timer_rect;
	SDL_Rect character1_rect;
	SDL_Rect character2_rect;
	SDL_Rect character1_image;
	SDL_Rect character2_image;
	SDL_Rect warrior_rect;
	SDL_Rect mage_rect;
	SDL_Rect rogue_rect;
	SDL_Rect paladin_rect;

	//WINDOWS && RELATED UI
	//P1
	UiWindow* p1_window = nullptr;
	Labels* p1_pause_label = nullptr;
	Labels* p1_char_sel_label = nullptr;
	Labels* p1_stage_sel_label = nullptr;
	Labels* p1_settings_label = nullptr;
	Labels* p1_main_menu_label = nullptr;
	Labels* p1_rematch_label = nullptr;
	
	Buttons* p1_char_sel_button = nullptr;
	Buttons* p1_stage_sel_button = nullptr;
	Buttons* p1_settings_button = nullptr;
	Buttons* p1_main_menu_button = nullptr;
	Buttons* p1_rematch_button = nullptr;
	//P2
	UiWindow* p2_window = nullptr;
	Labels* p2_pause_label = nullptr;
	Labels* p2_char_sel_label = nullptr;
	Labels* p2_stage_sel_label = nullptr;
	Labels* p2_settings_label = nullptr;
	Labels* p2_main_menu_label = nullptr;

	Buttons* p2_char_sel_button = nullptr;
	Buttons* p2_stage_sel_button = nullptr;
	Buttons* p2_settings_button = nullptr;
	Buttons* p2_main_menu_button = nullptr;
	Buttons* p2_rematch_button = nullptr;
	//GENERAL
	UiWindow* general_window = nullptr;
	Labels* match_end_label = nullptr;
	Labels* rematch_label = nullptr;
	Labels* char_sel_label = nullptr;
	Labels* stage_sel_label = nullptr;
	Labels* settings_label = nullptr;
	Labels* main_menu_label = nullptr;

	Buttons* rematch_button = nullptr;
	Buttons* char_sel_button = nullptr;
	Buttons* stage_sel_button = nullptr;
	Buttons* settings_button = nullptr;
	Buttons* main_menu_button = nullptr;

	//Combat scene timer
	Timer	combat_start_timer;
	Timer	scene_timer;
	Timer	taunt_timer; 
	Timer	round_timer;
		
	//Player info
	CHAR_TYPE char1, char2;
	ITEMS char1_item, char2_item;
	int char1_hp = 0, char2_hp = 0;
	int max_char1_hp = 0, max_char2_hp = 0;
	uint p1_rounds_won = 0;
	uint p2_rounds_won = 0;
	bool extra_round = false;
	bool entities_stopped = false; // Dirty bool to fix bug

	bool next_round = false;
	pugi::xml_node window_node;
};
#endif

