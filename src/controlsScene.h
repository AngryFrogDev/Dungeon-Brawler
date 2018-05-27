#ifndef __CONTROLSSCENE__
#define __CONTROLSSCENE__

#include "scene.h"

class controlsScene : public scene {
public:

	controlsScene(bool active);
	virtual ~controlsScene();

	bool start();
	bool update(float dt);

private:

	bool onEvent(Buttons* button);
	void loadSceneUi();
	void assignFocus();

private:

	Buttons* save_controls = nullptr;
	Buttons* default_controls = nullptr;
	Buttons* back = nullptr;

	Labels* l_curr_player = nullptr;
	Labels* l_up_control = nullptr;
	Labels* l_down_control = nullptr;
	Labels* l_left_control = nullptr;
	Labels* l_right_control = nullptr;
	Labels* l_light_attack_control = nullptr;
	Labels* l_heavy_attack_control = nullptr;
	Labels* l_special_1_control = nullptr;
	Labels* l_special_2_control = nullptr;
	Labels* l_grab_control = nullptr;
	Labels* l_start_control = nullptr;

	bool player1 = true;
	bool controller = true;
};
#endif
