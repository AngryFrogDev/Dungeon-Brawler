#ifndef _SCENE_
#define _SCENE_

#include "Module.h"
#include "Player.h"
#include "Buttons.h"
#include "Labels.h"
#include "Bars.h"
#include "UiWindow.h"

enum scene_type {
	NO_SCENE = 0,
	COMBAT_SCENE,
	MAIN_SCENE,
	START_SCENE,
	CHAR_SEL_SCENE,
	OBJECT_SEL
};

class scene
{
public:
	scene(scene_type _type);
	virtual ~scene();
//	virtual bool awake(const pugi::xml_node & md_config) { return true; };
	virtual bool start() { return true; };
	virtual bool update(float dt) { return true; };
	virtual bool cleanUp() { return true; };

	virtual bool onEvent(Buttons* button) { return true; };
	virtual void loadSceneUi() { return; };
	virtual void loadSceneTextures() { return; };
	virtual void assignFocus() { return; };
	virtual void checkSceneInput() { return; };

public:
	bool scene_active = false;
	scene_type type = NO_SCENE;
	Controller* player1_cont = nullptr;
	Controller* player2_cont = nullptr;

protected:
	//Nodes
	pugi::xml_document scene_config_doc;
	pugi::xml_node scene_config;
	pugi::xml_node labels_node;
	pugi::xml_node buttons_node;
	pugi::xml_node bars_node;
	pugi::xml_node textures_node;
};
#endif

