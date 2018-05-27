#include "controlsScene.h"
#include "Application.h"
#include "mdFonts.h"
#include "DebLog.h"
#include "mdGuiManager.h"
#include "mdSceneManager.h"
#include "mdEntities.h"
#include "mdRender.h"
#include <map>


controlsScene::controlsScene(bool active) : scene(MAIN_SCENE) {
	scene_active = active;
	name = "Controls Scene";
}


controlsScene::~controlsScene() {
}

bool controlsScene::start() {
	if (!loaded) {
		controls_texture = App->textures->load("gui/controls.png");
	}

	curr_player = 1;

	loadSceneUi();
	assignFocus();

	controller_rect.x = 0; 
	controller_rect.y = 0;
	controller_rect.w = 116;
	controller_rect.h = 80;

	keyboard_rect.x = 120;
	keyboard_rect.y = 0;
	keyboard_rect.w = 116;
	keyboard_rect.h = 80;

	a_button_rect.x = 0;
	a_button_rect.y = 84;
	a_button_rect.w = 40;
	a_button_rect.h = 40;

	b_button_rect.x = 40;
	b_button_rect.y = 84;
	b_button_rect.w = 40;
	b_button_rect.h = 40;

	x_button_rect.x = 80;
	x_button_rect.y = 84;
	x_button_rect.w = 40;
	x_button_rect.h = 40;

	y_button_rect.x = 120;
	y_button_rect.y = 84;
	y_button_rect.w = 40;
	y_button_rect.h = 40;

	lb_button_rect.x = 0;
	lb_button_rect.y = 140;
	lb_button_rect.w = 48;
	lb_button_rect.h = 40;

	rb_button_rect.x = 48;
	rb_button_rect.y = 140;
	rb_button_rect.w = 48;
	rb_button_rect.h = 40;

	lt_button_rect.x = 96;
	lt_button_rect.y = 140;
	lt_button_rect.w = 48;
	lt_button_rect.h = 40;
	 
	rt_button_rect.x = 144;
	rt_button_rect.y = 140;
	rt_button_rect.w = 48;
	rt_button_rect.h = 40;

	left_button_rect.x = 0;
	left_button_rect.y = 184;
	left_button_rect.w = 40;
	left_button_rect.h = 40;

	up_button_rect.x = 40;
	up_button_rect.y = 184;
	up_button_rect.w = 40;
	up_button_rect.h = 40;

	right_button_rect.x = 80;
	right_button_rect.y = 184;
	right_button_rect.w = 40;
	right_button_rect.h = 40;

	down_button_rect.x = 120;
	down_button_rect.y = 184;
	down_button_rect.w = 40;
	down_button_rect.h = 40;
	return true;
}

bool controlsScene::update(float dt) {
	App->gui->draw();

	App->render->drawSprite(4, controls_texture, 330, 105, &lb_button_rect, 2, false, 0, 0, 0, 0, false);

	SDL_Rect* current_scheme_rect = controller ? &controller_rect : &keyboard_rect;
	App->render->drawSprite(4, controls_texture, 800 + 471, 105, current_scheme_rect, 1, false, 0, 0, 0, 0, false);
	App->render->drawSprite(4, controls_texture, 800 + 597, 105, &rb_button_rect, 2, false, 0, 0, 0, 0, false);
	if (App->input->isButtonState(BUTTON_LEFTSHOULDER, KEY_DOWN)) {
		player1 = !player1;
		l_curr_player->changeContent(player1 ? "Player 1" : "Player 2", l_curr_player->color);
		curr_player = player1 ? 1 : 2;
	}
	if (App->input->isButtonState(BUTTON_RIGHTSHOULDER, KEY_DOWN)) 
		controller = !controller;

	return true;
}

bool controlsScene::onEvent(Buttons* button) {
	bool ret = true;

	switch (button->button_type) {
	default:
		break;
	case NO_BUTTON:
		LOG("Non valid button type");
		break;
	case DEFAULT_CONTROLS:
		if (controller)
			App->entities->controller_schemes[curr_player] = App->entities->controller_schemes[0];
		else 
			App->entities->keyboard_schemes[curr_player] = App->entities->keyboard_schemes[0];
		break;
	case SAVE_CONTROLS:
		App->entities->saveSchemes();
		break;
	case BACK:
		App->scene_manager->changeScene(App->scene_manager->settings_scene, this);
	}

	return ret;
}


void controlsScene::loadSceneUi() {

	pugi::xml_document config_file;
	pugi::xml_node config;
	config = App->loadConfig("config.xml", config_file);
	int mody = 35;
	int modx = 40;

	// Provisional: Positions should be loaded from XML

	int resolution_width, resolution_height = 0;
	resolution_width = config.child("window").child("resolution").attribute("width").as_int();
	resolution_height = config.child("window").child("resolution").attribute("height").as_int();

	default_controls = (Buttons*)App->gui->createButton(DEFAULT_CONTROLS, MEDIUM, 0, { resolution_width / 2 + 20, 165 }, this);
	b_up_control = (Buttons*)App->gui->createButton(B_UP, MEDIUM, 0, { resolution_width / 2 + 20, 265 }, this);
	b_down_control = (Buttons*)App->gui->createButton(B_DOWN, MEDIUM, 0, { resolution_width / 2 + 20, 330 }, this);
	b_left_control = (Buttons*)App->gui->createButton(B_LEFT, MEDIUM, 0, { resolution_width / 2 + 20, 395 }, this);
	b_right_control = (Buttons*)App->gui->createButton(B_RIGHT, MEDIUM, 0, { resolution_width / 2 + 20, 460 }, this);
	b_light_attack_control = (Buttons*)App->gui->createButton(B_LIGHT_ATTACK, MEDIUM, 0, { resolution_width / 2 + 20, 525 }, this);
	b_heavy_attack_control = (Buttons*)App->gui->createButton(B_HEAVY_ATTACK, MEDIUM, 0, { resolution_width / 2 + 20, 590 }, this);
	b_special_1_control = (Buttons*)App->gui->createButton(B_SPECIAL_1, MEDIUM, 0, { resolution_width / 2 + 20, 655 }, this);
	b_special_2_control = (Buttons*)App->gui->createButton(B_SPECIAL_2, MEDIUM, 0, { resolution_width / 2 + 20, 720 }, this);
	b_grab_control = (Buttons*)App->gui->createButton(B_GRAB, MEDIUM, 0, { resolution_width / 2 + 20, 785 }, this);
	save_controls = (Buttons*)App->gui->createButton(SAVE_CONTROLS, MEDIUM, 0, { resolution_width / 2 - 200, 900 }, this);
	back = (Buttons*)App->gui->createButton(BACK, MEDIUM, 0, { resolution_width / 2 + 245, 900 }, this);


	l_default = (Labels*)App->gui->createLabel("Default", { 112, 62, 62 }, App->fonts->large_size, { resolution_width / 2 + modx + 25, 175 }, this);
	l_save_controls = (Labels*)App->gui->createLabel("Save All", { 112, 62, 62 }, App->fonts->large_size, { resolution_width / 2 - 200 + modx , 905 }, this);
	l_back = (Labels*)App->gui->createLabel("Back", { 112, 62, 62 }, App->fonts->large_size, { resolution_width / 2 + 245 + modx + 35, 905 }, this);
	l_up_control = (Labels*)App->gui->createLabel("Up", { 112, 62, 62 }, App->fonts->large_size, { resolution_width / 2 + 20 + 105, 265 + 7 }, this);
	l_down_control = (Labels*)App->gui->createLabel("Down", { 112, 62, 62 }, App->fonts->large_size, { resolution_width / 2 + 20 + 80, 330 + 7 }, this);
	l_left_control = (Labels*)App->gui->createLabel("Left", { 112, 62, 62 }, App->fonts->large_size, { resolution_width / 2 + 20 + 80, 395 + 7 }, this);
	l_right_control = (Labels*)App->gui->createLabel("Right", { 112, 62, 62 }, App->fonts->large_size, { resolution_width / 2 + 20 + 75, 460 + 7 }, this);
	l_light_attack_control = (Labels*)App->gui->createLabel("Light Att.", { 112, 62, 62 }, App->fonts->large_size, { resolution_width / 2 + 20 + 25, 525 + 7 }, this);
	l_heavy_attack_control = (Labels*)App->gui->createLabel("Heavy Att.", { 112, 62, 62 }, App->fonts->large_size, { resolution_width / 2 + 20 + 25, 590 + 7 }, this);
	l_special_1_control = (Labels*)App->gui->createLabel("Special 1", { 112, 62, 62 }, App->fonts->large_size, { resolution_width / 2 + 20 + 25, 655 + 7 }, this);
	l_special_2_control = (Labels*)App->gui->createLabel("Special 2", { 112, 62, 62 }, App->fonts->large_size, { resolution_width / 2 + 20 + 25, 720 + 7 }, this);
	l_grab_control = (Labels*)App->gui->createLabel("Grab", { 112, 62, 62 }, App->fonts->large_size, { resolution_width / 2 + 20 + 80, 785 + 6 }, this);

	l_curr_player = (Labels*)App->gui->createLabel("Player 1", { 255, 255, 255  }, App->fonts->large_size, { resolution_width / 2 - resolution_width / 4 + modx, 90 + mody }, this);



}

void controlsScene::assignFocus() {
	if (!App->gui->p1_focus_elements.empty())
		App->entities->players[0]->focus = *App->gui->p1_focus_elements.begin();
}

void controlsScene::setUpScancodeList() {
	//Numbers
	scancode_names[SDL_SCANCODE_0] = "0";
	scancode_names[SDL_SCANCODE_1] = "1";
	scancode_names[SDL_SCANCODE_2] = "2";
	scancode_names[SDL_SCANCODE_3] = "3";
	scancode_names[SDL_SCANCODE_4] = "4";
	scancode_names[SDL_SCANCODE_5] = "5";
	scancode_names[SDL_SCANCODE_6] = "6";
	scancode_names[SDL_SCANCODE_7] = "7";
	scancode_names[SDL_SCANCODE_8] = "8";
	scancode_names[SDL_SCANCODE_9] = "9";

	//Letters
	scancode_names[SDL_SCANCODE_Q] = "Q";
	scancode_names[SDL_SCANCODE_W] = "W";
	scancode_names[SDL_SCANCODE_E] = "E";
	scancode_names[SDL_SCANCODE_R] = "R";
	scancode_names[SDL_SCANCODE_T] = "T";
	scancode_names[SDL_SCANCODE_Y] = "Y";
	scancode_names[SDL_SCANCODE_U] = "U";
	scancode_names[SDL_SCANCODE_I] = "I";
	scancode_names[SDL_SCANCODE_O] = "O";
	scancode_names[SDL_SCANCODE_P] = "P";

	scancode_names[SDL_SCANCODE_A] = "A";
	scancode_names[SDL_SCANCODE_S] = "S";
	scancode_names[SDL_SCANCODE_D] = "D";
	scancode_names[SDL_SCANCODE_F] = "F";
	scancode_names[SDL_SCANCODE_G] = "G";
	scancode_names[SDL_SCANCODE_H] = "H";
	scancode_names[SDL_SCANCODE_J] = "J";
	scancode_names[SDL_SCANCODE_K] = "K";
	scancode_names[SDL_SCANCODE_L] = "L";

	scancode_names[SDL_SCANCODE_Z] = "Z";
	scancode_names[SDL_SCANCODE_X] = "X";
	scancode_names[SDL_SCANCODE_C] = "C";
	scancode_names[SDL_SCANCODE_V] = "V";
	scancode_names[SDL_SCANCODE_B] = "B";
	scancode_names[SDL_SCANCODE_N] = "N";
	scancode_names[SDL_SCANCODE_M] = "M";
	scancode_names[SDL_SCANCODE_COMMA] = ",";
	scancode_names[SDL_SCANCODE_STOP] = ".";
	scancode_names[SDL_SCANCODE_SLASH] = "-";

	//Arrows
	scancode_names[SDL_SCANCODE_LEFT] = "LEFT";
	scancode_names[SDL_SCANCODE_RIGHT] = "RIGHT";
	scancode_names[SDL_SCANCODE_UP] = "UP";
	scancode_names[SDL_SCANCODE_DOWN] = "DOWN";

}
