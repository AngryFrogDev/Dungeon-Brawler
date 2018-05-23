#include "settingsScene.h"
#include "Application.h"
#include "mdFonts.h"
#include "DebLog.h"
#include "mdGuiManager.h"
#include "mdSceneManager.h"
#include "mdCollision.h"
#include "mdAudio.h"
#include "Character.h"
#include "mdMap.h"


settingsScene::settingsScene(bool active) : scene(MAIN_SCENE) {
	scene_active = active;
	name = "Settings Scene";
}


settingsScene::~settingsScene()
{
}

bool settingsScene::start() {
	loadSceneUi();
	assignFocus();
	s_crouching_special_2 = App->audio->loadSFX("SFX/crouching_special_2.wav"); // Provisional: Should be loaded from XML

	return true;
}

bool settingsScene::update(float dt) {
	App->gui->draw();

	return true;
}

bool settingsScene::onEvent(Buttons* button) {
	bool ret = true;

	switch (button->button_type)
	{
	default:
		break;
	case NO_BUTTON:
		LOG("Non valid button type");
		break;
	case MUSIC_VOL_DOWN:
		current_music_volume -= 8;
		if (current_music_volume < 0)
			current_music_volume = 0;
		App->audio->musicVolume(current_music_volume);
		break;
	case MUSIC_VOL_UP:
		current_music_volume += 8;
		if (current_music_volume > 128)
			current_music_volume = 128;
		App->audio->musicVolume(current_music_volume);
		break;
	case SFX_VOL_DOWN:
		current_sfx_volume -= 8;
		if (current_sfx_volume < 0)
			current_sfx_volume = 0;
		App->audio->sfxVolume(current_sfx_volume);
		App->audio->playSFX(s_crouching_special_2);
		break;
	case SFX_VOL_UP:
		current_sfx_volume += 8;
		if (current_sfx_volume > 128)
			current_sfx_volume = 128;
		App->audio->sfxVolume(current_sfx_volume);
		App->audio->playSFX(s_crouching_special_2);
		break;
	case COLLIDERS:
		App->collision->debug = !App->collision->debug;
		break;
	case PARALLAX:
		App->map->parallax = !App->map->parallax;
		break;
	case BACK:
		App->scene_manager->changeScene(App->scene_manager->main_scene, this);
	}

	return ret;
}


void settingsScene::loadSceneUi() {

	pugi::xml_document config_file;
	pugi::xml_node config;
	config = App->loadConfig("config.xml", config_file);
	int mody = 35;
	int modx = 40;

	// Provisional: Positions should be loaded from XML
	music_down = (Buttons*)App->gui->createButton(MUSIC_VOL_DOWN, LARGE, 0, { config.child("window").child("resolution").attribute("width").as_int() / 2 - 450, 250 }, this);
	music_up = (Buttons*)App->gui->createButton(MUSIC_VOL_UP, LARGE, 0, { config.child("window").child("resolution").attribute("width").as_int() / 2 + 450, 250 }, this);
	sfx_down = (Buttons*)App->gui->createButton(SFX_VOL_DOWN, LARGE, 0, { config.child("window").child("resolution").attribute("width").as_int() / 2 - 450, 400 }, this);
	sfx_up = (Buttons*)App->gui->createButton(SFX_VOL_UP, LARGE, 0, { config.child("window").child("resolution").attribute("width").as_int() / 2 + 450, 400 }, this);
	colliders = (Buttons*)App->gui->createButton(COLLIDERS, LARGE, 0, { config.child("window").child("resolution").attribute("width").as_int() / 2, 550 }, this);
	parallax = (Buttons*)App->gui->createButton(PARALLAX, LARGE, 0, { config.child("window").child("resolution").attribute("width").as_int() / 2, 700 }, this);
	back = (Buttons*)App->gui->createButton(BACK, LARGE, 0, { config.child("window").child("resolution").attribute("width").as_int() / 2, 850 }, this);
	// Provisional: Data should be loaded from XML
	l_music_down = (Labels*)App->gui->createLabel("Volume Down", { 255, 255, 255 }, App->fonts->large_size, { config.child("window").child("resolution").attribute("width").as_int() / 2 - 450 + modx, 250 + mody }, this);
	l_music_up = (Labels*)App->gui->createLabel("Volume Up", { 255, 255, 255 }, App->fonts->large_size, { config.child("window").child("resolution").attribute("width").as_int() / 2 + 450 + modx, 250 + mody}, this);
	l_sfx_down = (Labels*)App->gui->createLabel("SFX Down", { 255, 255, 255 }, App->fonts->large_size, { config.child("window").child("resolution").attribute("width").as_int() / 2 - 450 + modx, 400 + mody }, this);
	l_sfx_up = (Labels*)App->gui->createLabel("SFX Up", { 255, 255, 255 }, App->fonts->large_size, { config.child("window").child("resolution").attribute("width").as_int() / 2 + 450 + modx, 400 + mody }, this);
	l_colliders = (Labels*)App->gui->createLabel("Colliders", { 255, 255, 255 }, App->fonts->large_size, { config.child("window").child("resolution").attribute("width").as_int() / 2 + modx, 550 + mody }, this);
	l_parallax = (Labels*)App->gui->createLabel("Parallax", { 255, 255, 255 }, App->fonts->large_size, { config.child("window").child("resolution").attribute("width").as_int() / 2 + modx, 700 + mody }, this);
	l_back = (Labels*)App->gui->createLabel("Back", { 255, 255, 255 }, App->fonts->large_size, { config.child("window").child("resolution").attribute("width").as_int() / 2 + modx, 850 + mody }, this);

	b_music = (Bars*)App->gui->createBar(MUSIC_VOL_BAR, { config.child("window").child("resolution").attribute("width").as_int() / 2, 250 }, false, 1, this);
	b_sfx = (Bars*)App->gui->createBar(SFX_VOL_BAR, { config.child("window").child("resolution").attribute("width").as_int() / 2, 400 }, false, 1, this);
}

void settingsScene::assignFocus() {
	if (!App->gui->p1_focus_elements.empty())
		App->entities->players[0]->focus = *App->gui->p1_focus_elements.begin();
}

int settingsScene::getMusicVol() {
	return current_music_volume;
}

int settingsScene::getSfxVol() {
	return current_sfx_volume;
}