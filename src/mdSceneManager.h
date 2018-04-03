#ifndef __MDSCENEMANAGER__
#define __MDSCENEMANAGER__
#include "Module.h"
#include <list>
#include "SDL\include\SDL.h"
#include "p2Point.h"

struct CharacterInfo {

	iPoint postion;
};

struct UIElement {
	SDL_Rect area;
};

struct Scene {
	std::list<CharacterInfo> characters;
	std::list<UIElement> ui_elements;
};



class mdSceneManager : public Module{
public:
	mdSceneManager();
	~mdSceneManager();

	bool awake(const pugi::xml_node& md_config) override;
	bool start() override;

public:
	std::list<Scene> scenes;
	Scene* current_scene;

};

#endif