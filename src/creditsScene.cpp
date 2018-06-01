#include "creditsScene.h"
#include "Application.h"
#include "mdFonts.h"
#include "DebLog.h"
#include "mdGuiManager.h"
#include "mdSceneManager.h"

creditsScene::creditsScene(bool active) : scene(MAIN_SCENE) {
	scene_active = active;
	name = "Credits Scene";
}


creditsScene::~creditsScene()
{
}

bool creditsScene::start() {
	loadSceneUi();
	assignFocus();
	if (!loaded)
		scene_music = App->audio->loadMusic("SFX/scene music/Main_Menu.ogg");
	if (App->audio->re_play_music)
		App->audio->playMusic(scene_music), App->audio->re_play_music = false;

	return true;
}

bool creditsScene::update(float dt) {
	App->gui->draw();

	return true;
}

bool creditsScene::onEvent(Buttons* button) {
	bool ret = true;

	switch (button->button_type)
	{
	default:
		break;
	case NO_BUTTON:
		LOG("Non valid button type");
		break;
	case BACK:
		App->scene_manager->changeScene(App->scene_manager->main_scene, this);
	}

	return ret;
}


void creditsScene::loadSceneUi() {

	pugi::xml_document config_file;
	pugi::xml_node config;
	config = App->loadConfig("config.xml", config_file);

	back = (Buttons*)App->gui->createButton(BACK, MEDIUM, -1, { 1550, 1000 }, this);
	
	l_back = (Labels*)App->gui->createLabel("Back", { 112, 62, 62 }, App->fonts->large_size, { 1560, 1010 }, this);

	line1 = (Labels*)App->gui->createLabel("Thank you for playing Dungeon Brawler!", { 255, 255, 255 }, App->fonts->large_size, { 525, 100 }, this);
	line2 = (Labels*)App->gui->createLabel("Lead - Lucas García", { 255, 255, 255 }, App->fonts->large_size, { 750, 300 }, this);
	line3 = (Labels*)App->gui->createLabel("Art & Audio - Cristian Palos", { 255, 255, 255 }, App->fonts->large_size, { 650, 400 }, this);
	line4 = (Labels*)App->gui->createLabel("UI - Clara Ratera", { 255, 255, 255 }, App->fonts->large_size, { 755, 500 }, this);
	line5 = (Labels*)App->gui->createLabel("Code - Pol Ferrando", { 255, 255, 255 }, App->fonts->large_size, { 750, 600 }, this);
	line6 = (Labels*)App->gui->createLabel("Code - Sergi Parra", { 255, 255, 255 }, App->fonts->large_size, { 755, 700 }, this);
	line7 = (Labels*)App->gui->createLabel("QA - Raul Morente", { 255, 255, 255 }, App->fonts->large_size, { 755, 800 }, this);
	line8 = (Labels*)App->gui->createLabel("Design - Genís Bayó", { 255, 255, 255 }, App->fonts->large_size, { 750, 900 }, this);
}

void creditsScene::assignFocus() {
	if (!App->gui->p1_focus_elements.empty())
		App->entities->players[0]->focus = *App->gui->p1_focus_elements.begin();
}