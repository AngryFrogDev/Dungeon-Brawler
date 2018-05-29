#include "stageSelScene.h"
#include "mdGuiManager.h"
#include "mdMap.h"
#include "mdSceneManager.h"


stageSelScene::stageSelScene(bool active) : scene(STAGE_SEL_SCENE){
	scene_active = active;
	name = "Stage Selection Scene";
}


stageSelScene::~stageSelScene(){}

bool stageSelScene::start()	{
	bool ret = true;
	if (!loaded)
	{
		stage_1 = App->textures->load("assets/stage_1_prev.png");
		stage_2 = App->textures->load("assets/stage_2_prev.png");
		stage_3 = App->textures->load("assets/stage_3_prev.png");
		scene_sfx = App->audio->loadSFX("SFX/announcer/location-confirmed.wav");
		scene_music = App->audio->loadMusic("SFX/scene music/Character_Stage_Selection.ogg");
	}
	if (App->audio->re_play_music)
		App->audio->playMusic(scene_music), App->audio->re_play_music;

	transition_timer.stop();

	loadSceneUi();
	assignFocus();

	return ret;
}

bool stageSelScene::update(float dt)	{
	bool ret = true;
	loadSceneTextures();
	App->gui->draw();

	if (transition_timer.isActive())
		startingTransition();

	return ret;
}

bool stageSelScene::onEvent(Buttons* button)	{
	bool ret = true;

	switch (button->button_type)
	{
	case SELECT_STAGE_1:
		current_stage = stage_1;
		selected_map = 1;
		break;
	case SELECT_STAGE_2:
		current_stage = stage_2;
		selected_map = 2;
		break;
	case SELECT_STAGE_3:
		current_stage = stage_3;
		selected_map = 3;
		break;
	}

	if (button->being_clicked)
	{
		App->audio->playSFX(scene_sfx);
		App->map->loadMap(selected_map);
		transition_timer.start();
	}

	return ret;
}

void stageSelScene::loadSceneUi()	{
	scene_title_label = (Labels*)App->gui->createLabel("STAGE SELECTION", { 255,255,255,255 }, App->fonts->extra_large_size, { 500, 100 }, this);
	stage_1_button = (Buttons*)App->gui->createButton(SELECT_STAGE_1, STAGE_SELECTION, 0, { 297, 797 }, this);
	stage_2_button = (Buttons*)App->gui->createButton(SELECT_STAGE_2, STAGE_SELECTION, 0, { 747, 797 }, this);
	stage_3_button = (Buttons*)App->gui->createButton(SELECT_STAGE_3, STAGE_SELECTION, 0, { 1197, 797 }, this);
}

void stageSelScene::loadSceneTextures()	{
	App->render->drawSprite(2, stage_1, 300, 800, 0, 2, false, 1.0f, 0, 0, 0, false);
	App->render->drawSprite(2, stage_2, 750, 800, 0, 2, false, 1.0f, 0, 0, 0, false);
	App->render->drawSprite(2, stage_3, 1200, 800, 0, 2, false, 1.0f, 0, 0, 0, false);
	App->render->drawSprite(2, current_stage, 550, 250, 0, 5, false, 0, 0, 0, 0, false);


}

void stageSelScene::assignFocus()	{
	if (!App->gui->p1_focus_elements.empty())
		App->entities->players[0]->focus = *App->gui->p1_focus_elements.begin();
}

void stageSelScene::startingTransition()	{
	if (transition_timer.readSec() >= 1)
		App->scene_manager->changeScene(App->scene_manager->combat_scene, this);
}
