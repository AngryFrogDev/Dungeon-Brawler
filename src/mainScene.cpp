#include "Application.h"
#include "mdMap.h"
#include "mdFonts.h"
#include "mainScene.h"
#include "DebLog.h"
#include "mdGuiManager.h"
#include "mdSceneManager.h"


mainScene::mainScene(bool active) : scene(MAIN_SCENE)	{
	scene_active = active;

	//Preparing nodes to ease XML reading
	labels_node = scene_config.child("main_scene").child("labels");
	buttons_node = scene_config.child("main_scene").child("buttons");
}

mainScene::~mainScene()	{}

bool mainScene::start()	{
	loadSceneUi();
	assignFocus();
	App->map->map_loaded = true;

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
		break;
	case TRAINING:
		App->entities->traning = true;
		App->scene_manager->changeScene(App->scene_manager->char_sel_scene, this);
		break;
	case CREDITS:
		break;
	case GAME_EXIT:
		ret = false;
		break;
	case SETTINGS:
		break;
	}

	return ret;
}

void mainScene::loadSceneUi()	{
	//BUTTONS
	b_o_vs_o = (Buttons*)App->gui->createButton(ONE_V_ONE, LARGE, { buttons_node.child("o_vs_o").child("pos").attribute("x").as_int(), buttons_node.child("o_vs_o").child("pos").attribute("y").as_int() }, this);
	b_training = (Buttons*)App->gui->createButton(TRAINING, LARGE, { buttons_node.child("training").child("pos").attribute("x").as_int(), buttons_node.child("training").child("pos").attribute("y").as_int() }, this);
	b_settings = (Buttons*)App->gui->createButton(SETTINGS, LARGE, { buttons_node.child("settings").child("pos").attribute("x").as_int(), buttons_node.child("settings").child("pos").attribute("y").as_int() }, this);
	b_credits = (Buttons*)App->gui->createButton(CREDITS, LARGE, { buttons_node.child("credits").child("pos").attribute("x").as_int(), buttons_node.child("credits").child("pos").attribute("y").as_int() }, this);
	b_exit = (Buttons*)App->gui->createButton(GAME_EXIT, LARGE, { buttons_node.child("exit").child("pos").attribute("x").as_int(), buttons_node.child("exit").child("pos").attribute("y").as_int() }, this);
	
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
	if (!App->gui->focus_elements.empty())
		App->gui->focus = *App->gui->focus_elements.begin();
}

