#ifndef _MAINSCENE_
#define _MAINSCENE_

#include "scene.h"

class mainScene : public scene
{
public:
	mainScene(bool active);
	~mainScene();

//	bool awake(const pugi::xml_node & md_config);
	bool start();
	bool update(float dt);

private:
	bool onEvent(Buttons* button);
	void loadSceneUi();
	void assignFocus();
	bool cleanUp();
	
private:
	//MAIN MENU UI
	Buttons* b_o_vs_o = nullptr;
	Buttons* b_exit = nullptr;
	Buttons* b_training = nullptr;
	Buttons* b_settings = nullptr;
	Buttons* b_credits = nullptr;

	Labels* l_o_vs_o = nullptr;
	Labels* l_traning = nullptr;
	Labels* l_exit = nullptr;
	Labels* l_settings = nullptr;
	Labels* l_credits = nullptr;
};
#endif
