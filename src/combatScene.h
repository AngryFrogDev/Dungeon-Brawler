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
	SDL_Rect time_up_rect;
	SDL_Rect* current_round = nullptr;
	SDL_Rect* round_end = nullptr;
	SDL_Rect* combat_end = nullptr;

	//COMBAT UI
	SDL_Texture* timer_tex = nullptr;
	Bars* health_bar1 = nullptr;
	Bars* health_bar2 = nullptr;
	Bars* super_bar1 = nullptr;
	Bars* super_bar2 = nullptr;
	Labels* p1_combat_label = nullptr;
	Labels* p2_combat_label = nullptr;

	SDL_Rect still_round_indicator;
	SDL_Rect won_round_indicator;
	//Timer numbers rects, i stands for inverse, n for normal
	SDL_Rect timer_rect;
	SDL_Rect left_number;
	SDL_Rect right_number;
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
	Labels* p1_main_menu_label = nullptr;
	Labels* p1_rematch_label = nullptr;
	
	Buttons* p1_char_sel_button = nullptr;
	Buttons* p1_stage_sel_button = nullptr;
	Buttons* p1_main_menu_button = nullptr;
	Buttons* p1_rematch_button = nullptr;
	//P2
	UiWindow* p2_window = nullptr;
	Labels* p2_pause_label = nullptr;
	Labels* p2_char_sel_label = nullptr;
	Labels* p2_stage_sel_label = nullptr;
	Labels* p2_main_menu_label = nullptr;

	Buttons* p2_char_sel_button = nullptr;
	Buttons* p2_stage_sel_button = nullptr;
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

	//SFX
	Mix_Chunk* round_sfx1 = nullptr;
	Mix_Chunk* round_sfx2 = nullptr;
	Mix_Chunk* round_sfx3 = nullptr;
	Mix_Chunk* round_sfx4 = nullptr;
	Mix_Chunk* last_round_sfx = nullptr;
	Mix_Chunk* fight_sfx = nullptr;
	Mix_Chunk* player_dead_sfx1 = nullptr;
	Mix_Chunk* player_dead_sfx2 = nullptr;
	Mix_Chunk* ko_sfx = nullptr;
	Mix_Chunk* perfect_sfx = nullptr;
	Mix_Chunk* time_up_sfx = nullptr;
	Mix_Chunk* player1_wins_sfx = nullptr;
	Mix_Chunk* player2_wins_sfx = nullptr;
	Mix_Chunk* victorious_player = nullptr;
	Mix_Chunk* danger = nullptr;
	uint random_sfx = 0;

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

	//Extra info
	bool extra_round = false;
	bool entities_stopped = false; // Dirty bool to fix bug
	bool can_load_textures = true;
	bool next_round = false;
	bool sfx_played = false;
	bool countdown = false;
	int sec_count = 0;


	pugi::xml_node window_node;
};
#endif

