#ifndef __MDSCENEMANAGER__
#define __MDSCENEMANAGER__
#include "Module.h"
#include <list>
#include "SDL\include\SDL.h"
#include "p2Point.h"
#include "Character.h"
#include "mdInput.h"
#include "Buttons.h"
#include "Bars.h"
#include "Labels.h"
#include "PerfTimer.h"

//PROVISIONAL: Nyeh, needed to do fade to black
#define MIN( a, b ) ( ((a) < (b)) ? (a) : (b) )


enum ui_elem_type;
enum button_types;
enum bar_types;

enum scene_type {ONE_VS_ONE, TWO_VS_TWO, MAIN_MENU, START_SCENE}; //To make a switch for specific code later

struct CharacterInfo {
	int player;
	int x_pos;
	CHAR_TYPE type;
	bool flipped;
};

struct Scene {
	scene_type type; 
	std::list<CharacterInfo> characters; //to create scenes with 2 or 4 characters
	std::list<Widgets*> scene_ui_elems;
	std::list<SDL_Rect> other_scene_elems; //If name is not clear, it could be changed
	pugi::xml_node scene_data;
};

class mdSceneManager : public Module{
public:
	mdSceneManager();
	~mdSceneManager();

	bool awake(const pugi::xml_node& md_config) override;
	bool start() override;
	bool update(float dt) override;

	bool changeScene(Scene* scene_to_load);
	bool onEvent(Buttons* button);

private:
	bool createCharacters();
	bool createWidgets();
	void loadSceneUI();
	void loadSceneCharacters();
	void updateTimer();
	void blitUiTextures();


public:
	std::list<Scene> scenes;
	Scene* current_scene = nullptr;
	Scene* scene_to_load = nullptr;
	bool	paused = false;
	uint	current_time = 90;
	uint	max_time = 90;
	
	Scene one_vs_one;
	Scene two_vs_two;
	Scene main_menu;
	Scene start_scene;

private:
	enum fade_step
	{
		NONE,
		FADE_TO_BLACK,
		FADE_FROM_BLACK
	} current_step = fade_step::NONE;

	SDL_Rect screen;
	float fadetime = 1.0f;
	Timer switch_timer;
	//HARDCODE
	///When reading this from an XML, names should not be necesary

	CharacterInfo player1;
	CharacterInfo player2;

	CharacterInfo player3;
	CharacterInfo player4;

	//START SCENE UI
	Labels* intro_label = nullptr;
	SDL_Texture* game_logo = nullptr;

	//MAIN MENU UI
	Buttons* b_o_vs_o = nullptr;
	Buttons* b_t_vs_t = nullptr;
	Buttons* b_exit = nullptr;
	Buttons* b_training = nullptr;

	Labels* l_o_vs_o = nullptr;
	Labels* l_t_vs_t = nullptr;
	Labels* l_exit = nullptr;

	//COMBAT UI
	Bars* health_bar1 = nullptr;
	Bars* health_bar2 = nullptr;
	Bars* health_bar3 = nullptr;
	Bars* health_bar4 = nullptr;
	Bars* super_bar1 = nullptr;
	Bars* super_bar2 = nullptr;
	Bars* super_bar3 = nullptr;
	Bars* super_bar4 = nullptr;
	Labels* timer = nullptr;

	SDL_Rect timer_rect;
	SDL_Rect character1_rect;
	SDL_Rect character2_rect;
	SDL_Rect character3_rect;
	SDL_Rect character4_rect;
	SDL_Rect character1_image;
	SDL_Rect character2_image;
	SDL_Rect character3_image;
	SDL_Rect character4_image;

	//WINDOWS && RELATED UI
	SDL_Rect window;
	Buttons* rematch = nullptr;
	Buttons* restart = nullptr;
	Buttons* resume = nullptr;
	Buttons* in_game_settings = nullptr;
	Buttons* char_sel = nullptr;
	Buttons* stage_sel = nullptr;
	Buttons* to_main_menu = nullptr;

	//Combat scene timer
	Timer	scene_timer;

	//Provisional bar targets
	int health_bar_target = 0;
	int super_bar_target = 0;

	//Nodes
	pugi::xml_document scene_config_doc;
	pugi::xml_node scene_config;
	pugi::xml_node labels_node;
	pugi::xml_node buttons_node;
	pugi::xml_node bars_node;
	pugi::xml_node textures_node;
};

#endif