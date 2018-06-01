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

	// Load stuff
	scene_music = App->audio->loadMusic("SFX/scene music/Main_Menu.ogg");
	s_crouching_special_2 = App->audio->loadSFX("SFX/crouching_special_2.wav"); // Provisional: Should be loaded from XML
	background = App->textures->load("assets/main_background.png");
}


settingsScene::~settingsScene()
{
}

bool settingsScene::start() {
	loadSceneUi();
	assignFocus();

	if (App->audio->re_play_music)
		App->audio->playMusic(scene_music), App->audio->re_play_music = false;



	return true;
}

bool settingsScene::update(float dt) {
	App->gui->draw();
	App->render->drawSprite(1, background, -850, 0, (const SDL_Rect*)0, 4, false, 0);

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
		if (!App->collision->debug)
			l_colliders->setText("Colliders Off", { 112, 62, 62 }, App->fonts->medium_size);
		else
			l_colliders->setText("Colliders On", { 112, 62, 62 }, App->fonts->medium_size);
		break;
	case PARALLAX:
		App->map->parallax = !App->map->parallax;
		if (!App->map->parallax)
			l_parallax->setText("Parallax Off", { 112, 62, 62 }, App->fonts->medium_size);
		else
			l_parallax->setText("Parallax On", { 112, 62, 62 }, App->fonts->medium_size);
		break;
	case SAVE_CONTROLS:
		App->scene_manager->changeScene(App->scene_manager->controls_scene, this);
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
	music_down = (Buttons*)App->gui->createButton(MUSIC_VOL_DOWN, MEDIUM, -1, { config.child("window").child("resolution").attribute("width").as_int() / 2 - 350, 165 }, this);
	music_up = (Buttons*)App->gui->createButton(MUSIC_VOL_UP, MEDIUM, -1, { config.child("window").child("resolution").attribute("width").as_int() / 2 + 410, 165 }, this);
	sfx_down = (Buttons*)App->gui->createButton(SFX_VOL_DOWN, MEDIUM, -1, { config.child("window").child("resolution").attribute("width").as_int() / 2 - 350, 325 }, this);
	sfx_up = (Buttons*)App->gui->createButton(SFX_VOL_UP, MEDIUM, -1, { config.child("window").child("resolution").attribute("width").as_int() / 2 + 410, 325 }, this);
	colliders = (Buttons*)App->gui->createButton(COLLIDERS, MEDIUM, -1, { config.child("window").child("resolution").attribute("width").as_int() / 2+20, 450 }, this);
	parallax = (Buttons*)App->gui->createButton(PARALLAX, MEDIUM, -1, { config.child("window").child("resolution").attribute("width").as_int() / 2+20, 600 }, this);
	controls = (Buttons*)App->gui->createButton(SAVE_CONTROLS, MEDIUM, -1, { config.child("window").child("resolution").attribute("width").as_int() / 2 + 20, 750 }, this);
	back = (Buttons*)App->gui->createButton(BACK, MEDIUM, -1, { config.child("window").child("resolution").attribute("width").as_int() / 2+20, 900 }, this);
	// Provisional: Data should be loaded from XML
	l_music_down = (Labels*)App->gui->createLabel("Volume Down", { 112, 62, 62 }, App->fonts->large_size, { config.child("window").child("resolution").attribute("width").as_int() / 2 - 387 + modx, 140 + mody }, this);
	l_music_up = (Labels*)App->gui->createLabel("Volume Up", { 112, 62, 62 }, App->fonts->large_size, { config.child("window").child("resolution").attribute("width").as_int() / 2 + 400 + modx, 140 + mody}, this);
	l_sfx_down = (Labels*)App->gui->createLabel("SFX Down", { 112, 62, 62 }, App->fonts->large_size, { config.child("window").child("resolution").attribute("width").as_int() / 2 - 350 + modx, 300 + mody }, this);
	l_sfx_up = (Labels*)App->gui->createLabel("SFX Up", { 112, 62, 62 }, App->fonts->large_size, { config.child("window").child("resolution").attribute("width").as_int() / 2 + 430 + modx, 300 + mody }, this);
	l_colliders = (Labels*)App->gui->createLabel("Colliders Off", { 112, 62, 62 }, App->fonts->medium_size, { config.child("window").child("resolution").attribute("width").as_int() / 2 + modx, 425 + mody + 5 }, this);
	l_parallax = (Labels*)App->gui->createLabel("Parallax On", { 112, 62, 62 }, App->fonts->medium_size, { config.child("window").child("resolution").attribute("width").as_int() / 2 + modx + 5, 575 + mody + 5}, this);
	l_controls = (Labels*)App->gui->createLabel("Controls", { 112, 62, 62 }, App->fonts->large_size, { config.child("window").child("resolution").attribute("width").as_int() / 2 + modx + 20, 725 + mody }, this);
	l_back = (Labels*)App->gui->createLabel("Back", { 112, 62, 62 }, App->fonts->large_size, { config.child("window").child("resolution").attribute("width").as_int() / 2 + modx + 60, 875 + mody }, this);

	b_music = (Bars*)App->gui->createBar(MUSIC_VOL_BAR, { config.child("window").child("resolution").attribute("width").as_int() / 2, 130 }, false, 1, this);
	b_sfx = (Bars*)App->gui->createBar(SFX_VOL_BAR, { config.child("window").child("resolution").attribute("width").as_int() / 2, 290 }, false, 1, this);
}

void settingsScene::assignFocus() {
	if (!App->gui->p1_focus_elements.empty())
		App->entities->players[0]->focus = *App->gui->p1_focus_elements.begin();
	if (!App->gui->p2_focus_elements.empty())
		App->entities->players[1]->focus = *App->gui->p2_focus_elements.begin();
}

int settingsScene::getMusicVol() {
	return current_music_volume;
}

int settingsScene::getSfxVol() {
	return current_sfx_volume;
}

bool settingsScene::cleanUp() {
	LOG("Unloading background");

	App->textures->unload(background);

	return true;
}