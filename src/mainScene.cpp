#include "Application.h"
#include "mdMap.h"
#include "mdFonts.h"
#include "mainScene.h"
#include "DebLog.h"
#include "mdGuiManager.h"
#include "mdSceneManager.h"


mainScene::mainScene(bool active) : scene(MAIN_SCENE)	{
	scene_active = active;
	name = "Main Scene";

	//Preparing nodes to ease XML reading
	labels_node = scene_config.child("main_scene").child("labels");
	buttons_node = scene_config.child("main_scene").child("buttons");
}

mainScene::~mainScene()	{}

bool mainScene::start()	{
	if (!loaded)
		scene_music = App->audio->loadMusic("SFX/scene music/Main_Menu.ogg");
	if (App->audio->re_play_music)
		App->audio->playMusic(scene_music), App->audio->re_play_music = false;

	loadSceneUi();
	assignFocus();
	
	return true;
}

bool mainScene::update(float dt)	{
	App->gui->draw();

	return true;
}

bool mainScene::onEvent(Buttons* button)	{
	bool ret = true;

	switch (button->button_type)
	{
	default:
		break;
	case NO_BUTTON:
		LOG("Non valid button type");
		break;
	case ONE_V_ONE:
		App->entities->traning = false;
		App->scene_manager->changeScene(App->scene_manager->char_sel_scene, this);
		App->map->unloadMap();
		break;
	case TRAINING:
		App->entities->traning = true;
		App->scene_manager->changeScene(App->scene_manager->char_sel_scene, this);
		App->map->unloadMap();
		break;
	case CREDITS:
		App->scene_manager->changeScene(App->scene_manager->credits_scene, this);
		break;
	case GAME_EXIT:
		ret = false;
		break;
	case SETTINGS:
		App->scene_manager->changeScene(App->scene_manager->settings_scene, this);
		break;
	case STAGE_SEL_SCENE:
		App->scene_manager->changeScene(App->scene_manager->stage_sel_scene, this);
		App->map->unloadMap();
		break;
	}

	return ret;
}

void mainScene::loadSceneUi()	{
	//BUTTONS
	b_o_vs_o = (Buttons*)App->gui->createButton(ONE_V_ONE, LARGE, -1, { buttons_node.child("o_vs_o").child("pos").attribute("x").as_int(), buttons_node.child("o_vs_o").child("pos").attribute("y").as_int() }, this);
	b_training = (Buttons*)App->gui->createButton(TRAINING, LARGE, -1, { buttons_node.child("training").child("pos").attribute("x").as_int(), buttons_node.child("training").child("pos").attribute("y").as_int() }, this);
	b_settings = (Buttons*)App->gui->createButton(SETTINGS, LARGE, -1, { buttons_node.child("settings").child("pos").attribute("x").as_int(), buttons_node.child("settings").child("pos").attribute("y").as_int() }, this);
	b_credits = (Buttons*)App->gui->createButton(CREDITS, LARGE, -1, { buttons_node.child("credits").child("pos").attribute("x").as_int(), buttons_node.child("credits").child("pos").attribute("y").as_int() }, this);
	b_exit = (Buttons*)App->gui->createButton(GAME_EXIT, LARGE, -1, { buttons_node.child("exit").child("pos").attribute("x").as_int(), buttons_node.child("exit").child("pos").attribute("y").as_int() }, this);
	
	//LABELS
	l_o_vs_o = (Labels*)App->gui->createLabel(labels_node.child("o_vs_o").child("content").attribute("value").as_string(), { (Uint8)labels_node.child("o_vs_o").child("color").attribute("r").as_int(),(Uint8)labels_node.child("o_vs_o").child("color").attribute("g").as_int(),(Uint8)labels_node.child("o_vs_o").child("color").attribute("b").as_int(),(Uint8)labels_node.child("o_vs_o").child("color").attribute("a").as_int() },
	App->fonts->large_size, { labels_node.child("o_vs_o").child("pos").attribute("x").as_int(), labels_node.child("o_vs_o").child("pos").attribute("y").as_int() }, this);
	
	l_traning = (Labels*)App->gui->createLabel(labels_node.child("training").child("content").attribute("value").as_string(), { (Uint8)labels_node.child("training").child("color").attribute("r").as_int(),(Uint8)labels_node.child("training").child("color").attribute("g").as_int(),(Uint8)labels_node.child("training").child("color").attribute("b").as_int(),(Uint8)labels_node.child("training").child("color").attribute("a").as_int() },
	App->fonts->large_size, { labels_node.child("training").child("pos").attribute("x").as_int(), labels_node.child("training").child("pos").attribute("y").as_int() }, this);

	l_settings = (Labels*)App->gui->createLabel(labels_node.child("settings").child("content").attribute("value").as_string(), { (Uint8)labels_node.child("settings").child("color").attribute("r").as_int(),(Uint8)labels_node.child("settings").child("color").attribute("g").as_int(),(Uint8)labels_node.child("settings").child("color").attribute("b").as_int(),(Uint8)labels_node.child("settings").child("color").attribute("a").as_int() },
	App->fonts->large_size, { labels_node.child("settings").child("pos").attribute("x").as_int(), labels_node.child("settings").child("pos").attribute("y").as_int() }, this);
	
	l_credits = (Labels*)App->gui->createLabel(labels_node.child("credits").child("content").attribute("value").as_string(), { (Uint8)labels_node.child("credits").child("color").attribute("r").as_int(),(Uint8)labels_node.child("credits").child("color").attribute("g").as_int(),(Uint8)labels_node.child("credits").child("color").attribute("b").as_int(),(Uint8)labels_node.child("credits").child("color").attribute("a").as_int() },
	App->fonts->large_size, { labels_node.child("credits").child("pos").attribute("x").as_int(), labels_node.child("credits").child("pos").attribute("y").as_int() }, this);

	l_exit = (Labels*)App->gui->createLabel(labels_node.child("exit").child("content").attribute("value").as_string(), { (Uint8)labels_node.child("exit").child("color").attribute("r").as_int(),(Uint8)labels_node.child("exit").child("color").attribute("g").as_int(),(Uint8)labels_node.child("exit").child("color").attribute("b").as_int(),(Uint8)labels_node.child("exit").child("color").attribute("a").as_int() },
	App->fonts->large_size, { labels_node.child("exit").child("pos").attribute("x").as_int(), labels_node.child("exit").child("pos").attribute("y").as_int() }, this);
	
}

void mainScene::assignFocus()	{
	if (!App->gui->p1_focus_elements.empty())
		App->entities->players[0]->focus = *App->gui->p1_focus_elements.begin();
	if (!App->gui->p2_focus_elements.empty())
		App->entities->players[1]->focus = *App->gui->p2_focus_elements.begin();
}

