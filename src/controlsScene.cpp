#include "controlsScene.h"
#include "Application.h"
#include "mdFonts.h"
#include "DebLog.h"
#include "mdGuiManager.h"
#include "mdSceneManager.h"
#include "mdEntities.h"


controlsScene::controlsScene(bool active) : scene(MAIN_SCENE) {
	scene_active = active;
	name = "Controls Scene";
}


controlsScene::~controlsScene() {
}

bool controlsScene::start() {
	loadSceneUi();
	assignFocus();
	return true;
}

bool controlsScene::update(float dt) {
	App->gui->draw();

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
	//case SFX_VOL_DOWN:
	//	current_sfx_volume -= 8;
	//	if (current_sfx_volume < 0)
	//		current_sfx_volume = 0;
	//	App->audio->sfxVolume(current_sfx_volume);
	//	App->audio->playSFX(s_crouching_special_2);
	//	break;
	//case SFX_VOL_UP:
	//	current_sfx_volume += 8;
	//	if (current_sfx_volume > 128)
	//		current_sfx_volume = 128;
	//	App->audio->sfxVolume(current_sfx_volume);
	//	App->audio->playSFX(s_crouching_special_2);
	//	break;
	//case COLLIDERS:
	//	App->collision->debug = !App->collision->debug;
	//	break;
	//case PARALLAX:
	//	App->map->parallax = !App->map->parallax;
	//	break;
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
	default_controls = (Buttons*)App->gui->createButton(DEFAULT_CONTROLS, MEDIUM, 0, { config.child("window").child("resolution").attribute("width").as_int() / 2 - 350, 265 }, this);
	save_controls = (Buttons*)App->gui->createButton(SAVE_CONTROLS, MEDIUM, 0, { config.child("window").child("resolution").attribute("width").as_int() / 2 + 410, 265 }, this);
	back = (Buttons*)App->gui->createButton(BACK, MEDIUM, 0, { config.child("window").child("resolution").attribute("width").as_int() / 2 + 20, 850 }, this);
	/*sfx_down = (Buttons*)App->gui->createButton(SFX_VOL_DOWN, MEDIUM, 0, { config.child("window").child("resolution").attribute("width").as_int() / 2 - 350, 425 }, this);
	sfx_up = (Buttons*)App->gui->createButton(SFX_VOL_UP, MEDIUM, 0, { config.child("window").child("resolution").attribute("width").as_int() / 2 + 410, 425 }, this);
	colliders = (Buttons*)App->gui->createButton(COLLIDERS, MEDIUM, 0, { config.child("window").child("resolution").attribute("width").as_int() / 2 + 20, 550 }, this);
	parallax = (Buttons*)App->gui->createButton(PARALLAX, MEDIUM, 0, { config.child("window").child("resolution").attribute("width").as_int() / 2 + 20, 700 }, this);*/
	// Provisional: Data should be loaded from XML
	l_curr_player = (Labels*)App->gui->createLabel("Player 1", { 112, 62, 62 }, App->fonts->large_size, { config.child("window").child("resolution").attribute("width").as_int() / 2 - 387 + modx, 240 + mody }, this);
	/*l_music_up = (Labels*)App->gui->createLabel("Volume Up", { 112, 62, 62 }, App->fonts->large_size, { config.child("window").child("resolution").attribute("width").as_int() / 2 + 400 + modx, 240 + mody }, this);
	l_sfx_down = (Labels*)App->gui->createLabel("SFX Down", { 112, 62, 62 }, App->fonts->large_size, { config.child("window").child("resolution").attribute("width").as_int() / 2 - 350 + modx, 400 + mody }, this);
	l_sfx_up = (Labels*)App->gui->createLabel("SFX Up", { 112, 62, 62 }, App->fonts->large_size, { config.child("window").child("resolution").attribute("width").as_int() / 2 + 430 + modx, 400 + mody }, this);
	l_colliders = (Labels*)App->gui->createLabel("Colliders", { 112, 62, 62 }, App->fonts->large_size, { config.child("window").child("resolution").attribute("width").as_int() / 2 + modx + 10, 525 + mody }, this);
	l_parallax = (Labels*)App->gui->createLabel("Parallax", { 112, 62, 62 }, App->fonts->large_size, { config.child("window").child("resolution").attribute("width").as_int() / 2 + modx + 20, 675 + mody }, this);
	l_back = (Labels*)App->gui->createLabel("Back", { 112, 62, 62 }, App->fonts->large_size, { config.child("window").child("resolution").attribute("width").as_int() / 2 + modx + 60, 825 + mody }, this);*/
}

void controlsScene::assignFocus() {
	if (!App->gui->p1_focus_elements.empty())
		App->entities->players[0]->focus = *App->gui->p1_focus_elements.begin();
}