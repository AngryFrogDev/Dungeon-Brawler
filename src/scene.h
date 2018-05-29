#ifndef _SCENE_
#define _SCENE_

#include "Module.h"
#include "Player.h"
#include "Buttons.h"
#include "Labels.h"
#include "Bars.h"
#include "UiWindow.h"
#include "mdAudio.h"

#include "SDL_mixer\include\SDL_mixer.h"

enum scene_type {
	NO_SCENE = 0,
	COMBAT_SCENE,
	MAIN_SCENE,
	START_SCENE,
	SETTINGS_SCENE,
	CHAR_SEL_SCENE,
	STAGE_SEL_SCENE,
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
	virtual void setRects() { return; };
	virtual void assignFocus() { return; };
	virtual void checkSceneInput() { return; };
	virtual void popUpP1Window() { return; };
	virtual void popUpP2Window() { return; };
	virtual void closeP1Window() { return; };
	virtual void closeP2Window() { return; };
	virtual void resetSceneValues() { return; };
	virtual void startingTransition() { return; };
	virtual int getMusicVol() { return 0; };
	virtual int getSfxVol() { return 0; };

public:
	bool scene_active = false;
	bool rematching = false;
	scene_type type = NO_SCENE;
	const char* name = nullptr;
	Mix_Music* scene_music = nullptr;
	bool loaded = false;

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

