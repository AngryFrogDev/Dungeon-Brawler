#ifndef __MDSCENEMANAGER__
#define __MDSCENEMANAGER__
#include "Module.h"
#include <list>
#include "SDL\include\SDL.h"
#include "p2Point.h"
#include "Character.h"

struct CharacterInfo {
	int player;
	int x_pos;
	CHAR_TYPE type;
	bool flipped;
};

struct UIElement {
	SDL_Rect area;
};

struct Scene {
	Scene();
	std::list<CharacterInfo> characters; //to create scenes with 2 or 4 characters
	std::list<UIElement> ui_elements;
};



class mdSceneManager : public Module{
public:
	mdSceneManager();
	~mdSceneManager();

	bool awake(const pugi::xml_node& md_config) override;
	bool start() override;

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

};

#endif