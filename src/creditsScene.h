#ifndef _CREDITSSSCENE_
#define _CREDITSSSCENE_

#include "scene.h"

class creditsScene : public scene
{
public:

	creditsScene(bool active);
	virtual ~creditsScene();

	bool start();
	bool update(float dt);

private:

	bool onEvent(Buttons* button);
	void loadSceneUi();
	void assignFocus();

private:

	Buttons* back = nullptr;

	Labels* l_back = nullptr;

	Labels* line1 = nullptr;
	Labels* line2 = nullptr;
	Labels* line3 = nullptr;
	Labels* line4 = nullptr;
	Labels* line5 = nullptr;
	Labels* line6 = nullptr;
	Labels* line7 = nullptr;
	Labels* line8 = nullptr;
};
#endif
