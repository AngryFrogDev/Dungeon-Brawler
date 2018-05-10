#ifndef _SETTINGSSCENE_
#define _SETTINGSSCENE_

#include "scene.h"

class settingsScene : public scene
{
public:

	settingsScene(bool active);
	virtual ~settingsScene();

	bool start();
	bool update(float dt);

private:

	bool onEvent(Buttons* button);
	void loadSceneUi();
	void assignFocus();

private:

	Buttons* music_up = nullptr;
	Buttons* music_down = nullptr;
	Buttons* sfx_up = nullptr;
	Buttons* sfx_down = nullptr;
	Buttons* show_colliders = nullptr;
	Buttons* hide_colliders = nullptr;
	Buttons* parallax = nullptr;
	Buttons* back = nullptr;

	Labels* l_music_up = nullptr;
	Labels* l_music_down = nullptr;
	Labels* l_sfx_up = nullptr;
	Labels* l_sfx_down = nullptr;
	Labels* l_show_colliders = nullptr;
	Labels* l_hide_colliders = nullptr;
	Labels* l_parallax = nullptr;
	Labels* l_back = nullptr;

	Mix_Chunk* s_crouching_special_2;
	int current_music_volume = 128;
	int current_sfx_volume = 128;
};
#endif
