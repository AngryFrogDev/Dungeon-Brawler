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
	// Load stuff
	controls_texture = App->textures->load("gui/controls.png");
}


controlsScene::~controlsScene() {
}

bool controlsScene::start() {
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

	button_rects[BUTTON_A].x = 0;
	button_rects[BUTTON_A].y = 84;
	button_rects[BUTTON_A].w = 40;
	button_rects[BUTTON_A].h = 40;

	button_rects[BUTTON_B].x = 40;
	button_rects[BUTTON_B].y = 84;
	button_rects[BUTTON_B].w = 40;
	button_rects[BUTTON_B].h = 40;

	button_rects[BUTTON_X].x = 80;
	button_rects[BUTTON_X].y = 84;
	button_rects[BUTTON_X].w = 40;
	button_rects[BUTTON_X].h = 40;

	button_rects[BUTTON_Y].x = 120;
	button_rects[BUTTON_Y].y = 84;
	button_rects[BUTTON_Y].w = 40;
	button_rects[BUTTON_Y].h = 40;

	button_rects[BUTTON_LEFTSHOULDER].x = 0;
	button_rects[BUTTON_LEFTSHOULDER].y = 140;
	button_rects[BUTTON_LEFTSHOULDER].w = 48;
	button_rects[BUTTON_LEFTSHOULDER].h = 40;

	button_rects[BUTTON_RIGHTSHOULDER].x = 48;
	button_rects[BUTTON_RIGHTSHOULDER].y = 140;
	button_rects[BUTTON_RIGHTSHOULDER].w = 48;
	button_rects[BUTTON_RIGHTSHOULDER].h = 40;

	button_rects[AXIS_TRIGGERLEFT].x = 96;
	button_rects[AXIS_TRIGGERLEFT].y = 140;
	button_rects[AXIS_TRIGGERLEFT].w = 48;
	button_rects[AXIS_TRIGGERLEFT].h = 40;
	 
	button_rects[AXIS_TRIGGERRIGHT].x = 144;
	button_rects[AXIS_TRIGGERRIGHT].y = 140;
	button_rects[AXIS_TRIGGERRIGHT].w = 48;
	button_rects[AXIS_TRIGGERRIGHT].h = 40;

	button_rects[BUTTON_DPAD_LEFT].x = 0;
	button_rects[BUTTON_DPAD_LEFT].y = 184;
	button_rects[BUTTON_DPAD_LEFT].w = 40;
	button_rects[BUTTON_DPAD_LEFT].h = 40;

	button_rects[BUTTON_DPAD_UP].x = 40;
	button_rects[BUTTON_DPAD_UP].y = 184;
	button_rects[BUTTON_DPAD_UP].w = 40;
	button_rects[BUTTON_DPAD_UP].h = 40;

	button_rects[BUTTON_DPAD_RIGHT].x = 80;
	button_rects[BUTTON_DPAD_RIGHT].y = 184;
	button_rects[BUTTON_DPAD_RIGHT].w = 40;
	button_rects[BUTTON_DPAD_RIGHT].h = 40;

	button_rects[BUTTON_DPAD_DOWN].x = 120;
	button_rects[BUTTON_DPAD_DOWN].y = 184;
	button_rects[BUTTON_DPAD_DOWN].w = 40;
	button_rects[BUTTON_DPAD_DOWN].h = 40;

	setUpScancodeList();
	return true;
}

bool controlsScene::update(float dt) {
	App->gui->draw();

	SDL_Rect* current_scheme_rect = controller ? &controller_rect : &keyboard_rect;
	App->render->drawSprite(4, controls_texture, 800 + 471, 105, current_scheme_rect, 1, false, 0, 0, 0, 0, false);

	if (changing_buttons) {
		changeInput();
	}
	else {
		App->render->drawSprite(4, controls_texture, 330, 105, &button_rects[BUTTON_LEFTSHOULDER], 2, false, 0, 0, 0, 0, false);
		App->render->drawSprite(4, controls_texture, 800 + 597, 105, &button_rects[BUTTON_RIGHTSHOULDER], 2, false, 0, 0, 0, 0, false);
		if (App->input->isButtonState(BUTTON_LEFTSHOULDER, KEY_DOWN)) {
			player1 = !player1;
			l_curr_player->changeContent(player1 ? "Player 1" : "Player 2", l_curr_player->color);
			curr_player = player1 ? 1 : 2;
			if (!controller)
				setKeyboardLabels();
		}
		if (App->input->isButtonState(BUTTON_RIGHTSHOULDER, KEY_DOWN)) {
			controller = !controller;
			for (int i = 0; i < MAX_INPUTS - 1; ++i)
				keyboard_labels[i]->to_blit = !controller;
			if (!controller)
				setKeyboardLabels();
		}
	}
	if (controller)
		drawControls();

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
		break;
	case B_UP:
		if (!changing_buttons) {
			changing_buttons = true;
			input_to_change = CHARACTER_INPUTS::UP;
			if (controller)
				App->input->pruneControllerInputs();
			else {
				App->input->pruneKeyboardInputs();
				keyboard_labels[UP]->to_blit = false;
			}
		}
		break;
	case B_DOWN:
		if (!changing_buttons) {
			changing_buttons = true;
			input_to_change = CHARACTER_INPUTS::DOWN;
			if (controller)
				App->input->pruneControllerInputs();
			else {
				App->input->pruneKeyboardInputs();
				keyboard_labels[DOWN]->to_blit = false;
			}
		}
		break;
	case B_LEFT:
		if (!changing_buttons) {
			changing_buttons = true;
			input_to_change = CHARACTER_INPUTS::LEFT;
			if (controller)
				App->input->pruneControllerInputs();
			else {
				App->input->pruneKeyboardInputs();
				keyboard_labels[LEFT]->to_blit = false;
			}
		}
		break;
	case B_RIGHT:
		if (!changing_buttons) {
			changing_buttons = true;
			input_to_change = CHARACTER_INPUTS::RIGHT;
			if (controller)
				App->input->pruneControllerInputs();
			else {
				App->input->pruneKeyboardInputs();
				keyboard_labels[RIGHT]->to_blit = false;
			}
		}
		break;
	case B_LIGHT_ATTACK:
		if (!changing_buttons) {
			changing_buttons = true;
			input_to_change = CHARACTER_INPUTS::LIGHT_ATTACK;
			if (controller)
				App->input->pruneControllerInputs();
			else {
				App->input->pruneKeyboardInputs();
				keyboard_labels[LIGHT_ATTACK]->to_blit = false;
			}
		}
		break;

	case B_HEAVY_ATTACK:
		if (!changing_buttons) {
			changing_buttons = true;
			input_to_change = CHARACTER_INPUTS::HEAVY_ATTACK;
			if (controller)
				App->input->pruneControllerInputs();
			else {
				App->input->pruneKeyboardInputs();
				keyboard_labels[HEAVY_ATTACK]->to_blit = false;
			}
		}
		break;
	case B_SPECIAL_1:
		if (!changing_buttons) {
			changing_buttons = true;
			input_to_change = CHARACTER_INPUTS::SPECIAL_1;
			if (controller)
				App->input->pruneControllerInputs();
			else {
				App->input->pruneKeyboardInputs();
				keyboard_labels[SPECIAL_1]->to_blit = false;
			}
		}
		break;
	case B_SPECIAL_2:
		if (!changing_buttons) {
			changing_buttons = true;
			input_to_change = CHARACTER_INPUTS::SPECIAL_2;
			if (controller)
				App->input->pruneControllerInputs();
			else {
				App->input->pruneKeyboardInputs();
				keyboard_labels[SPECIAL_2]->to_blit = false;
			}
		}
		break;
	case B_GRAB:
		if (!changing_buttons) {
			changing_buttons = true;
			input_to_change = CHARACTER_INPUTS::GRAB;
			if (controller)
				App->input->pruneControllerInputs();
			else {
				App->input->pruneKeyboardInputs();
				keyboard_labels[GRAB]->to_blit = false;
			}
		}
		break;
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

	default_controls = (Buttons*)App->gui->createButton(DEFAULT_CONTROLS, MEDIUM, -1, { resolution_width / 2 + 20, 165 }, this);
	b_up_control = (Buttons*)App->gui->createButton(B_UP, MEDIUM, -1, { resolution_width / 2 + 20, 265 }, this);
	b_down_control = (Buttons*)App->gui->createButton(B_DOWN, MEDIUM, -1, { resolution_width / 2 + 20, 330 }, this);
	b_left_control = (Buttons*)App->gui->createButton(B_LEFT, MEDIUM, -1, { resolution_width / 2 + 20, 395 }, this);
	b_right_control = (Buttons*)App->gui->createButton(B_RIGHT, MEDIUM, -1, { resolution_width / 2 + 20, 460 }, this);
	b_light_attack_control = (Buttons*)App->gui->createButton(B_LIGHT_ATTACK, MEDIUM, -1, { resolution_width / 2 + 20, 525 }, this);
	b_heavy_attack_control = (Buttons*)App->gui->createButton(B_HEAVY_ATTACK, MEDIUM, -1, { resolution_width / 2 + 20, 590 }, this);
	b_special_1_control = (Buttons*)App->gui->createButton(B_SPECIAL_1, MEDIUM, -1, { resolution_width / 2 + 20, 655 }, this);
	b_special_2_control = (Buttons*)App->gui->createButton(B_SPECIAL_2, MEDIUM, -1, { resolution_width / 2 + 20, 720 }, this);
	b_grab_control = (Buttons*)App->gui->createButton(B_GRAB, MEDIUM, -1, { resolution_width / 2 + 20, 785 }, this);
	save_controls = (Buttons*)App->gui->createButton(SAVE_CONTROLS, MEDIUM, -1, { resolution_width / 2 - 200, 900 }, this);
	back = (Buttons*)App->gui->createButton(BACK, MEDIUM, -1, { resolution_width / 2 + 245, 900 }, this);


	l_default = (Labels*)App->gui->createLabel("Default", { 112, 62, 62 }, App->fonts->large_size, { resolution_width / 2 + modx + 25, 175 }, this);
	l_save_controls = (Labels*)App->gui->createLabel("Save It", { 112, 62, 62 }, App->fonts->large_size, { resolution_width / 2 - 200 + modx , 905 }, this);
	l_back = (Labels*)App->gui->createLabel("Back", { 112, 62, 62 }, App->fonts->large_size, { resolution_width / 2 + 245 + modx + 35, 905 }, this);
	l_up_control = (Labels*)App->gui->createLabel("Up", { 112, 62, 62 }, App->fonts->large_size, { resolution_width / 2 + 20 + 105, 265 + 7 }, this);
	l_down_control = (Labels*)App->gui->createLabel("Down", { 112, 62, 62 }, App->fonts->large_size, { resolution_width / 2 + 20 + 80, 330 + 7 }, this);
	l_left_control = (Labels*)App->gui->createLabel("Left", { 112, 62, 62 }, App->fonts->large_size, { resolution_width / 2 + 20 + 80, 395 + 7 }, this);
	l_right_control = (Labels*)App->gui->createLabel("Right", { 112, 62, 62 }, App->fonts->large_size, { resolution_width / 2 + 20 + 75, 460 + 7 }, this);
	l_light_attack_control = (Labels*)App->gui->createLabel("Light Att.", { 112, 62, 62 }, App->fonts->large_size, { resolution_width / 2 + 20 + 25, 525 + 7 }, this);
	l_heavy_attack_control = (Labels*)App->gui->createLabel("Heavy Att.", { 112, 62, 62 }, App->fonts->large_size, { resolution_width / 2 + 20 + 25, 590 + 7 }, this);
	l_special_1_control = (Labels*)App->gui->createLabel("Special 1", { 112, 62, 62 }, App->fonts->large_size, { resolution_width / 2 + 20 + 25, 655 + 7 }, this);
	l_special_2_control = (Labels*)App->gui->createLabel("Special 2", { 112, 62, 62 }, App->fonts->large_size, { resolution_width / 2 + 20 + 25, 720 + 7 }, this);
	l_grab_control = (Labels*)App->gui->createLabel("Taunt", { 112, 62, 62 }, App->fonts->large_size, { resolution_width / 2 + 20 + 80, 785 + 6 }, this);

	keyboard_labels[UP] = (Labels*)App->gui->createLabel("", { 255, 255, 255 }, App->fonts->large_size, { 1130 , 265 + 7 }, this);
	keyboard_labels[DOWN] = (Labels*)App->gui->createLabel("", { 255, 255, 255 }, App->fonts->large_size, { 1130, 330 + 7 }, this);
	keyboard_labels[LEFT] = (Labels*)App->gui->createLabel("", { 255, 255, 255 }, App->fonts->large_size, { 1130, 395 + 7 }, this);
	keyboard_labels[RIGHT] = (Labels*)App->gui->createLabel("", { 255, 255, 255 }, App->fonts->large_size, { 1130, 460 + 7 }, this);
	keyboard_labels[LIGHT_ATTACK] = (Labels*)App->gui->createLabel("", { 255, 255, 255 }, App->fonts->large_size, { 1130, 525 + 7 }, this);
	keyboard_labels[HEAVY_ATTACK] = (Labels*)App->gui->createLabel("", { 255, 255, 255 }, App->fonts->large_size, { 1130, 590 + 7 }, this);
	keyboard_labels[SPECIAL_1] = (Labels*)App->gui->createLabel("", { 255, 255, 255 }, App->fonts->large_size, { 1130, 655 + 7 }, this);
	keyboard_labels[SPECIAL_2] = (Labels*)App->gui->createLabel("", { 255, 255, 255 }, App->fonts->large_size, { 1130, 720 + 7 }, this);
	keyboard_labels[GRAB] = (Labels*)App->gui->createLabel("", { 255, 255, 255 }, App->fonts->large_size, { 1130, 785 + 6 }, this);

	for (int i = 0; i < MAX_INPUTS - 1; ++i)
		keyboard_labels[i]->to_blit = false;

	l_curr_player = (Labels*)App->gui->createLabel("Player 1", { 255, 255, 255  }, App->fonts->large_size, { resolution_width / 2 - resolution_width / 4 + modx, 90 + mody }, this);
}

void controlsScene::assignFocus() {
	if (!App->gui->p1_focus_elements.empty())
		App->entities->players[0]->focus = *App->gui->p1_focus_elements.begin();
	if (!App->gui->p2_focus_elements.empty())
		App->entities->players[1]->focus = *App->gui->p2_focus_elements.begin();
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

void controlsScene::drawControls() {
	int curr_y = 265;
	for (int i = 0; i < CHARACTER_INPUTS::MAX_INPUTS - 1; ++i, curr_y += 65) {
		if (changing_buttons && i == input_to_change)
			continue;
		CONTROLLER_BUTTON curr_input = (CONTROLLER_BUTTON)App->entities->controller_schemes[curr_player].scheme[i];
		App->render->drawSprite(4, controls_texture, 1130, curr_y, &button_rects[curr_input], 1.5, false, 0, 0, 0, 0, false);
	}
}

void controlsScene::changeInput() {
	if (controller) {
		CONTROLLER_BUTTON new_input = App->input->getLastButtonPressed();
		if (new_input != BUTTON_INVALID) {
			for (int i = 0; i < MAX_INPUTS - 1; ++i)
				if (App->entities->controller_schemes[curr_player].scheme[i] == new_input) {
					App->entities->controller_schemes[curr_player].scheme[i] = App->entities->controller_schemes[curr_player].scheme[input_to_change];
					break;
				}
			App->entities->controller_schemes[curr_player].scheme[input_to_change] = new_input;
			App->input->pruneControllerInputs();
			changing_buttons = false;
		}
	}
	else {
		SDL_Scancode new_input = App->input->getLastKeyPressed();
		if (scancode_names.find(new_input) != scancode_names.end()) {
			for (int i = 0; i < MAX_INPUTS - 1; ++i)
				if (App->entities->keyboard_schemes[curr_player].scheme[i] == new_input) {
					App->entities->keyboard_schemes[curr_player].scheme[i] = App->entities->keyboard_schemes[curr_player].scheme[input_to_change];
					break;
				}
			App->entities->keyboard_schemes[curr_player].scheme[input_to_change] = new_input;
			keyboard_labels[input_to_change]->to_blit = true;
			setKeyboardLabels();
			App->input->pruneKeyboardInputs();
			changing_buttons = false;
		}
	}
}

void controlsScene::setKeyboardLabels() {
	for (int i = 0; i < MAX_INPUTS - 1; ++i) {
		std::string name = scancode_names[(SDL_Scancode)App->entities->keyboard_schemes[curr_player].scheme[i]];
		keyboard_labels[i]->changeContent(name.c_str(), keyboard_labels[i]->color);
	}
}
