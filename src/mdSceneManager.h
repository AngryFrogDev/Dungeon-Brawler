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

struct CharacterInfo {
	int player;
	int x_pos;
	CHAR_TYPE type;
	bool flipped;
};

struct UIElementInfo {
	iPoint pos;
	ui_elem_type widget_type;

};

struct Scene {
	std::list<CharacterInfo> characters; //to create scenes with 2 or 4 characters
	std::list<UIElementInfo> ui_elements;
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

public:
	std::list<Scene> scenes;
	Scene* current_scene = nullptr;

private:
	//HARDCODE

	Scene Test_Scene;
	CharacterInfo player1;
	CharacterInfo player2;

	Bars* test_bar = nullptr;
	Buttons* button;
};

#endif