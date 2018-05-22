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
	case SHOW_COLLIDERS:
		App->collision->debug = true;
		break;
	case PARALLAX:
		App->map->parallax = !App->map->parallax;
		break;
	case HIDE_COLLIDERS:
		App->collision->debug = false;
		break;
	case BACK:
		App->scene_manager->changeScene(App->scene_manager->main_scene, this);
	}

	return ret;
}


void settingsScene::loadSceneUi() {

	// Provisional: Positions should be loaded from XML
	music_up = (Buttons*)App->gui->createButton(MUSIC_VOL_UP, LARGE, 0, { 0, 0 }, this);
	music_down = (Buttons*)App->gui->createButton(MUSIC_VOL_DOWN, LARGE, 0, { 0, 100 }, this);
	sfx_up = (Buttons*)App->gui->createButton(SFX_VOL_UP, LARGE, 0, { 0, 200 }, this);
	sfx_down = (Buttons*)App->gui->createButton(SFX_VOL_DOWN, LARGE, 0, { 0, 300 }, this);
	show_colliders = (Buttons*)App->gui->createButton(SHOW_COLLIDERS, LARGE, 0, { 0, 400 }, this);
	hide_colliders = (Buttons*)App->gui->createButton(HIDE_COLLIDERS, LARGE, 0, { 0, 500 }, this);
	parallax = (Buttons*)App->gui->createButton(PARALLAX, LARGE, 0, { 0, 600 }, this);
	back = (Buttons*)App->gui->createButton(BACK, LARGE, 0, { 0, 700 }, this);
	// Provisional: Data should be loaded from XML
	l_music_up = (Labels*)App->gui->createLabel("Volume Up", { 255, 255, 255 }, App->fonts->large_size, { 0, 0 }, this);
	l_music_down = (Labels*)App->gui->createLabel("Volume Down", { 255, 255, 255 }, App->fonts->large_size, { 0, 100 }, this);
	l_sfx_up = (Labels*)App->gui->createLabel("SFX Up", { 255, 255, 255 }, App->fonts->large_size, { 0, 200 }, this);
	l_sfx_down = (Labels*)App->gui->createLabel("SFX Down", { 255, 255, 255 }, App->fonts->large_size, { 0, 300 }, this);
	l_show_colliders = (Labels*)App->gui->createLabel("Show Colliders", { 255, 255, 255 }, App->fonts->large_size, { 0, 400 }, this);
	l_hide_colliders = (Labels*)App->gui->createLabel("Hide Colliders", { 255, 255, 255 }, App->fonts->large_size, { 0, 500 }, this);
	l_parallax = (Labels*)App->gui->createLabel("Parallax", { 255, 255, 255 }, App->fonts->large_size, { 0, 600 }, this);
	l_back = (Labels*)App->gui->createLabel("Back", { 255, 255, 255 }, App->fonts->large_size, { 0, 700 }, this);

	b_music = (Bars*)App->gui->createBar(MUSIC_VOL_BAR, { 200, 200 }, false, 1, this);
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