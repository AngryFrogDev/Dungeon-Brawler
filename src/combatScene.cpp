#include "Application.h"
#include "combatScene.h"
#include "mdGuiManager.h"
#include "mdEntities.h"
#include "mdMap.h"
#include "mdFonts.h"
#include "Player.h"
#include "DebLog.h"



combatScene::combatScene(bool active) : scene(COMBAT_SCENE)	{
	scene_active = active;
	name = "Combat Scene";

	//Preparing nodes
	labels_node = scene_config.child("combat").child("labels");
	bars_node = scene_config.child("combat").child("bars");
	buttons_node = scene_config.child("combat").child("buttons");
	textures_node = scene_config.child("combat").child("textures");
	window_node = scene_config.child("combat").child("window");
}


combatScene::~combatScene()	{}

bool combatScene::start()	{
	max_time = 99;
	current_time = max_time;
	App->map->map_loaded = true;
	loadSceneUi();

	App->entities->paused = false;
	App->entities->show = true;

	scene_timer.start();
	return true;
}

bool combatScene::update(float dt)	{
	checkSceneInput();
	loadSceneTextures();	
	updateTimer();
//	checkPlayers(); //Check if both player are still alive
	App->gui->draw();
	
	return true;
}

bool combatScene::onEvent(Buttons * button)	{
	bool ret = true;

	switch (button->button_type)
	{
	default:
		break;
	case NO_BUTTON:
		LOG("Non valid button type");
		break;
	case IN_GAME_REMATCH:
		break;
	case IN_GAME_CHAR_SEL:
		break;
	case IN_GAME_MAIN_MENU:
		break;
	case IN_GAME_RESTART:
		break;
	case IN_GAME_SETTINGS:
		break;
	case IN_GAME_STAGE_SEL:
		break;	
	}

	return ret;
}

void combatScene::loadSceneUi() {
	//TEXTURES
	timer_rect = { 421, 142, 59, 59 };
	character1_rect = { 6,175,66,34 };
	character1_image = { 82,175,60,28 };
	character2_rect = character1_rect;
	//PROVISIONAL: When we have more than one playable characters, this should be checked with the selected characters. 
	character2_image = character1_image;

	//LABELS
	auto label_string = std::to_string(current_time);
	timer = (Labels*)App->gui->createLabel(label_string.data(), { (Uint8)labels_node.child("timer").child("color").attribute("r").as_int(),(Uint8)labels_node.child("timer").child("color").attribute("g").as_int(),(Uint8)labels_node.child("timer").child("color").attribute("b").as_int(),(Uint8)labels_node.child("timer").child("color").attribute("a").as_int() },
		App->fonts->extra_large_size, { labels_node.child("timer").child("pos").attribute("x").as_int(),labels_node.child("timer").child("pos").attribute("y").as_int() }, this);

	//BARS
	health_bar1 = (Bars*)App->gui->createBar(HEALTH_BAR, { bars_node.child("health_bar1").child("pos").attribute("x").as_int(),bars_node.child("health_bar1").child("pos").attribute("y").as_int() },
		bars_node.child("health_bar1").child("flip").attribute("value").as_bool(), bars_node.child("health_bar1").child("target_player").attribute("value").as_int(), this);

	health_bar2 = (Bars*)App->gui->createBar(HEALTH_BAR, { bars_node.child("health_bar2").child("pos").attribute("x").as_int(),bars_node.child("health_bar2").child("pos").attribute("y").as_int() },
		bars_node.child("health_bar2").child("flip").attribute("value").as_bool(), bars_node.child("health_bar2").child("target_player").attribute("value").as_int(), this);

	super_bar1 = (Bars*)App->gui->createBar(SUPER_BAR, { bars_node.child("super_bar1").child("pos").attribute("x").as_int(),bars_node.child("super_bar1").child("pos").attribute("y").as_int() },
		bars_node.child("super_bar1").child("flip").attribute("value").as_bool(), bars_node.child("super_bar1").child("target_player").attribute("value").as_int(), this);

	super_bar2 = (Bars*)App->gui->createBar(SUPER_BAR, { bars_node.child("super_bar2").child("pos").attribute("x").as_int(),bars_node.child("super_bar2").child("pos").attribute("y").as_int() },
		bars_node.child("super_bar2").child("flip").attribute("value").as_bool(), bars_node.child("super_bar2").child("target_player").attribute("value").as_int(), this);
}

void combatScene::updateTimer()	{
	if (App->entities->paused || App->entities->traning)
		return;

	if (scene_timer.readSec() >= 1)
		current_time--, scene_timer.start();
	if (current_time == 0)
	{
		current_time = max_time;
		popUpGeneralWindow();
	}
	if (current_time > 0)
	{
		auto label_string = std::to_string(current_time);
		timer->changeContent(label_string.data(), { 0,0,0,255 });
	}
}

void combatScene::checkPlayers()	{
/*	int char1_hp = App->entities->players[0]->getCurrCharacter()->getCurrentLife();
	int char2_hp = App->entities->players[1]->getCurrCharacter()->getCurrentLife();
	if (char1_hp <= 0 && !App->entities->paused || char2_hp <= 0 && !App->entities->paused)
		popUpWindow();
		*/
}

void combatScene::loadSceneTextures()	{
	App->render->drawSprite(4, App->gui->atlas, 567, 70, &timer_rect, 2, 0, 0, 0, 0, false);
	App->render->drawSprite(3, App->gui->atlas, 110, 100, &character1_rect, 3, false, 0, 0, 0, 0, false);
	App->render->drawSprite(4, App->gui->atlas, 119, 109, &character1_image, 3, false, 0, 0, 0, 0, false);
	App->render->drawSprite(3, App->gui->atlas, 1570, 100, &character2_rect, 3, false, 0, 0, 0, 0, false);
	App->render->drawSprite(4, App->gui->atlas, 1579, 109, &character2_image, 3, true, 0, 0, 0, 0, false);
}

void combatScene::assignFocus()	{
	if (!App->gui->p1_focus_elements.empty() && !App->entities->players[0]->focus)
		App->entities->players[0]->focus = *App->gui->p1_focus_elements.begin();

	if (!App->gui->p2_focus_elements.empty() && !App->entities->players[1]->focus)
		App->entities->players[1]->focus = *App->gui->p2_focus_elements.begin();
}

void combatScene::checkSceneInput()	{
	//Check if pause menu is opened.
	if (App->entities->players[0]->getController()->isPressed(BUTTON_START))
		popUpP1Window();
	if (App->entities->players[1]->getController()->isPressed(BUTTON_START))
		popUpP2Window();
}

void combatScene::popUpP1Window()	{

}

void combatScene::popUpP2Window()	{
}

void combatScene::closeP1Window()	{
}

void combatScene::closeP2Window()	{
}

void combatScene::popUpGeneralWindow()	{
	//WINDOWS AND RELATED UI
	//BUTTONS
//	rematch = (Buttons*)App->gui->createButton(IN_GAME_REMATCH, MEDIUM, { buttons_node.child("rematch").child("pos").attribute("x").as_int(), buttons_node.child("rematch").child("pos").attribute("y").as_int() }, this);

//	to_main_menu = (Buttons*)App->gui->createButton(IN_GAME_MAIN_MENU, MEDIUM, { 780,680 }, this);

	//LABELS
	rematch_l = (Labels*)App->gui->createLabel("REMATCH", { 0,0,0,255 }, App->fonts->large_size, { 820, 615 }, this);

	to_main_menu_l = (Labels*)App->gui->createLabel("MAIN MENU", { 0,0,0,255 }, App->fonts->large_size, { 795, 695 }, this);

	end_label = (Labels*)App->gui->createLabel("WE HAVE A WINNER!", { 255,255,255,255 }, App->fonts->large_size, { 725, 420 }, this);

	//WINDOW
	window = (UiWindow*)App->gui->createWindow(MATCH_END, { 600,400 }, this);
}

void combatScene::closeGeneralWindow()	{
}
