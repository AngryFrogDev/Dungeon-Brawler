#include "controlsScene.h"
#include "Application.h"
#include "mdFonts.h"
#include "DebLog.h"
#include "mdGuiManager.h"
#include "mdSceneManager.h"
#include "mdEntities.h"
#include "mdRender.h"


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

	App->render->drawSprite(4, controls_texture, 330, 155, &lb_button_rect, 2, false, 0, 0, 0, 0, false);
	App->render->drawSprite(4, controls_texture, 640, 155, &rb_button_rect, 2, false, 0, 0, 0, 0, false);
	if (App->input->isButtonState(SDL_CONTROLLER_BUTTON_RIGHTSHOULDER, KEY_DOWN) ||
		App->input->isButtonState(SDL_CONTROLLER_BUTTON_LEFTSHOULDER, KEY_DOWN)) {
		player1 = !player1;
		l_curr_player->changeContent(player1 ? "Player 1" : "Player 2", { 255, 255, 255 });
	}
	return true;
}

bool controlsScene::onEvent(Buttons* button) {
	bool ret = true;

	int player = 0;

	switch (button->button_type) {
	default:
		break;
	case NO_BUTTON:
		LOG("Non valid button type");
		break;
	case DEFAULT_CONTROLS:
		player = player1 ? 1 : 2;
		if (controller)
			App->entities->controller_schemes[player] = App->entities->controller_schemes[0];
		else 
			App->entities->keyboard_schemes[player] = App->entities->keyboard_schemes[0];
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

	default_controls = (Buttons*)App->gui->createButton(DEFAULT_CONTROLS, MEDIUM, 0, { resolution_width / 2 - 350, 265 }, this);
	save_controls = (Buttons*)App->gui->createButton(SAVE_CONTROLS, MEDIUM, 0, { resolution_width / 2 + 410, 265 }, this);
	back = (Buttons*)App->gui->createButton(BACK, MEDIUM, 0, { resolution_width / 2 + 20, 850 }, this);

	l_curr_player = (Labels*)App->gui->createLabel("Player 1", { 255, 255, 255  }, App->fonts->large_size, { resolution_width / 2 - resolution_width / 4 + modx, 140 + mody }, this);

}

void controlsScene::assignFocus() {
	if (!App->gui->p1_focus_elements.empty())
		App->entities->players[0]->focus = *App->gui->p1_focus_elements.begin();
}