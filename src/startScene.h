#ifndef _STARTSCENE_
#define _STARTSCENE_

#include "scene.h"

class startScene : public scene
{
public:
	startScene(bool active);
	~startScene();

//	bool awake(const pugi::xml_node & md_config);
	bool start();
	bool update(float dt);

private:
	void loadSceneUi();
	void loadSceneTextures();
	void checkSceneInput();

private:
	//START SCENE UI
	Labels* intro_label = nullptr;
	SDL_Texture* game_logo = nullptr;
};
#endif
