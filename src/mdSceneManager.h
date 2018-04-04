#ifndef __MDSCENEMANAGER__
#define __MDSCENEMANAGER__
#include "Module.h"
#include <list>
#include "SDL\include\SDL.h"
#include "p2Point.h"
#include "Character.h"
class Bars;
class Buttons;
enum ui_elem_type;
enum button_types;
enum bar_types;

enum scene_type {ONE_VS_ONE, TWO_VS_TWO, MAIN_MENU}; //To make a switch for specific code later

struct CharacterInfo {
	int player;
	int x_pos;
	CHAR_TYPE type;
	bool flipped;
};

struct WidgetInfo {
	iPoint pos;
	ui_elem_type type;
	button_types button_type; //in xml, leave empty if item is not a button
	bar_types bar_type; //in xml, leave empty if item is not a button

};

struct Scene {
	scene_type type; 
	std::list<CharacterInfo> characters; //to create scenes with 2 or 4 characters
	std::list<WidgetInfo> ui_elements;
};



class mdSceneManager : public Module{
public:
	mdSceneManager();
	~mdSceneManager();

	bool awake(const pugi::xml_node& md_config) override;
	bool start() override;
	bool update(float dt) override;

private:
	bool CreateCharacters();
	bool CreateWidgets();

public:
	std::list<Scene> scenes;
	Scene* current_scene = nullptr;

private:
	//HARDCODE

	Scene one_vs_one;
	Scene two_vs_two;
	CharacterInfo player1;
	CharacterInfo player2;

	CharacterInfo player3;
	CharacterInfo player4;

	WidgetInfo left_health_bar;
};

#endif