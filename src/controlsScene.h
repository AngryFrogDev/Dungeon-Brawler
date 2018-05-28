#ifndef __CONTROLSSCENE__
#define __CONTROLSSCENE__

#include "scene.h"
#include <map>

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
	void setUpScancodeList();
	void drawControls();
	void changeInput();
	void setKeyboardLabels();

private:

	Buttons* save_controls = nullptr;
	Buttons* default_controls = nullptr;
	Buttons* back = nullptr;

	Buttons* b_up = nullptr;

	Labels* l_curr_player = nullptr;
	Labels* l_default = nullptr;
	Labels* l_back = nullptr;
	Labels* l_save_controls = nullptr;

	Labels* l_up_control = nullptr;
	Labels* l_down_control = nullptr;
	Labels* l_left_control = nullptr;
	Labels* l_right_control = nullptr;
	Labels* l_light_attack_control = nullptr;
	Labels* l_heavy_attack_control = nullptr;
	Labels* l_special_1_control = nullptr;
	Labels* l_special_2_control = nullptr;
	Labels* l_grab_control = nullptr;

	Labels* keyboard_labels[MAX_INPUTS - 1];

	Buttons* b_up_control = nullptr;
	Buttons* b_down_control = nullptr;
	Buttons* b_left_control = nullptr;
	Buttons* b_right_control = nullptr;
	Buttons* b_light_attack_control = nullptr;
	Buttons* b_heavy_attack_control = nullptr;
	Buttons* b_special_1_control = nullptr;
	Buttons* b_special_2_control = nullptr;
	Buttons* b_grab_control = nullptr;

	std::map<SDL_Scancode, std::string> scancode_names;

	SDL_Texture* controls_texture = nullptr;
	SDL_Rect controller_rect;
	SDL_Rect keyboard_rect;
	SDL_Rect button_rects[CONTROLLER_BUTTON::BUTTON_MAX];

	int curr_player;

	bool player1 = true;
	bool controller = true;

	bool changing_buttons = false;
	CHARACTER_INPUTS input_to_change = NULL_INPUT;
};
#endif
