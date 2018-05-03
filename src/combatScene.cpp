#include "Application.h"
#include "combatScene.h"
#include "mdGuiManager.h"
#include "mdEntities.h"
#include "mdMap.h"
#include "mdFonts.h"
#include "Player.h"
#include "DebLog.h"
#include "mdSceneManager.h"



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
		App->scene_manager->changeScene(App->scene_manager->char_sel_scene, this);
		break;
	case IN_GAME_MAIN_MENU:
		App->scene_manager->changeScene(App->scene_manager->main_scene, this);
		break;
	case IN_GAME_RESTART:
		if (button->focus_id == 0)
			closeP1Window();
		else
			closeP2Window();
		break;
	case IN_GAME_SETTINGS:
		App->scene_manager->changeScene(App->scene_manager->settings_scene, this);
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
	if (App->entities->players[0]->getController()->isPressed(BUTTON_START))
		popUpP1Window();
	if (App->entities->players[1]->getController()->isPressed(BUTTON_START))
		popUpP2Window();
}

void combatScene::popUpP1Window()	{
	App->entities->paused = true;

	if (!p1_window && !p2_window)
	{
		p1_window = (UiWindow*)App->gui->createWindow(PAUSE, { 625, 400 }, this);
		p1_pause_label = (Labels*)App->gui->createLabel("P1 PAUSE", { 255,255,255,255 }, App->fonts->large_size, { 850, 420 }, this);
		p1_resume_button = (Buttons*)App->gui->createButton(IN_GAME_RESTART, MEDIUM, 0, { 775, 500 }, this);
		p1_resume_label = (Labels*)App->gui->createLabel("RESUME", { 33,33,33,255 }, App->fonts->medium_size, { 850, 515 }, this);
		p1_char_sel_button = (Buttons*)App->gui->createButton(IN_GAME_CHAR_SEL, MEDIUM, 0, { 775, 575 }, this);
		p1_char_sel_label = (Labels*)App->gui->createLabel("CHAR. SELEC.", { 33,33,33,255 }, App->fonts->medium_size, { 800, 590 }, this);
		p1_stage_sel_button = (Buttons*)App->gui->createButton(IN_GAME_STAGE_SEL, MEDIUM, 0, { 775, 650 }, this);
		p1_stage_sel_label = (Labels*)App->gui->createLabel("STAGE SEL.", { 33,33,33,255 }, App->fonts->medium_size, { 800, 665 }, this);
		p1_settings_button = (Buttons*)App->gui->createButton(IN_GAME_SETTINGS, MEDIUM, 0, { 775, 725 }, this);
		p1_settings_label = (Labels*)App->gui->createLabel("SETTINGS", { 33,33,33,255 }, App->fonts->medium_size, { 830, 740 }, this);
		p1_main_menu_button = (Buttons*)App->gui->createButton(IN_GAME_MAIN_MENU, MEDIUM, 0, { 775, 800 }, this);
		p1_main_menu_label = (Labels*)App->gui->createLabel("MAIN MENU", { 33,33,33,255 }, App->fonts->medium_size, { 815, 815 }, this);
		
		assignFocus();
	}
}

void combatScene::popUpP2Window()	{
	App->entities->paused = true;

	if (!p2_window && !p1_window)
	{
		p2_window = (UiWindow*)App->gui->createWindow(PAUSE, { 625, 400 }, this);
		p2_pause_label = (Labels*)App->gui->createLabel("P2 PAUSE", { 255,255,255,255 }, App->fonts->large_size, { 850, 420 }, this);
		p2_resume_button = (Buttons*)App->gui->createButton(IN_GAME_RESTART, MEDIUM, 1, { 775, 500 }, this);
		p2_resume_label = (Labels*)App->gui->createLabel("RESUME", { 33,33,33,255 }, App->fonts->medium_size, { 850, 515 }, this);
		p2_char_sel_button = (Buttons*)App->gui->createButton(IN_GAME_CHAR_SEL, MEDIUM, 1, { 775, 575 }, this);
		p2_char_sel_label = (Labels*)App->gui->createLabel("CHAR. SELEC.", { 33,33,33,255 }, App->fonts->medium_size, { 800, 590 }, this);
		p2_stage_sel_button = (Buttons*)App->gui->createButton(IN_GAME_STAGE_SEL, MEDIUM, 1, { 775, 650 }, this);
		p2_stage_sel_label = (Labels*)App->gui->createLabel("STAGE SEL.", { 33,33,33,255 }, App->fonts->medium_size, { 800, 665 }, this);
		p2_settings_button = (Buttons*)App->gui->createButton(IN_GAME_SETTINGS, MEDIUM, 1, { 775, 725 }, this);
		p2_settings_label = (Labels*)App->gui->createLabel("SETTINGS", { 33,33,33,255 }, App->fonts->medium_size, { 830, 740 }, this);
		p2_main_menu_button = (Buttons*)App->gui->createButton(IN_GAME_MAIN_MENU, MEDIUM, 1, { 775, 800 }, this);
		p2_main_menu_label = (Labels*)App->gui->createLabel("MAIN MENU", { 33,33,33,255 }, App->fonts->medium_size, { 815, 815 }, this);

		assignFocus();
	}
}

void combatScene::closeP1Window()	{
	//Deleting buttons
	std::list<Widgets*>::reverse_iterator it = App->gui->p1_focus_elements.rbegin();
	Widgets* object = nullptr;
	for (it; it != App->gui->p1_focus_elements.rend(); it++)
	{
		object = *it;
		object->to_delete = true;
	}
	p1_window->to_delete = true;
	p1_pause_label->to_delete = true;
	p1_resume_label->to_delete = true;
	p1_char_sel_label->to_delete = true;
	p1_stage_sel_label->to_delete = true;
	p1_settings_label->to_delete = true;
	p1_main_menu_label->to_delete = true;

	App->entities->players[0]->focus = nullptr;
	
	App->entities->paused = false;
}

void combatScene::closeP2Window()	{
	//Deleting buttons
	std::list<Widgets*>::reverse_iterator it = App->gui->p2_focus_elements.rbegin();
	Widgets* object = nullptr;
	for (it; it != App->gui->p2_focus_elements.rend(); it++)
	{
		object = *it;
		object->to_delete = true;
	}
	p2_window->to_delete = true;
	p2_pause_label->to_delete = true;
	p2_resume_label->to_delete = true;
	p2_char_sel_label->to_delete = true;
	p2_stage_sel_label->to_delete = true;
	p2_settings_label->to_delete = true;
	p2_main_menu_label->to_delete = true;

	App->entities->players[1]->focus = nullptr;

	App->entities->paused = false;

}

void combatScene::popUpGeneralWindow()	{
	
}

void combatScene::closeGeneralWindow()	{
}
