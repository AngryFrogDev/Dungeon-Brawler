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

	UiWindow * controls_window = nullptr;

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

	SDL_Texture* controls_texture = nullptr;
	SDL_Rect controller_rect;
	SDL_Rect keyboard_rect;
	SDL_Rect a_button_rect;
	SDL_Rect b_button_rect;
	SDL_Rect x_button_rect;
	SDL_Rect y_button_rect;
	SDL_Rect lb_button_rect;
	SDL_Rect rb_button_rect;
	SDL_Rect lt_button_rect;
	SDL_Rect rt_button_rect;
	SDL_Rect left_button_rect;
	SDL_Rect up_button_rect;
	SDL_Rect right_button_rect;
	SDL_Rect down_button_rect;


	bool player1 = true;
	bool controller = true;
};
#endif
