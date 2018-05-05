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
	checkPlayers(); //Check if both player are still alive
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
	case IN_GAME_CHAR_SEL:
		if (button->focus_id == 0)
			closeP1Window();
		else
			closeP2Window();
		App->scene_manager->changeScene(App->scene_manager->char_sel_scene, this);
		break;
	case IN_GAME_MAIN_MENU:
		if (button->focus_id == 0)
			closeP1Window();
		else
			closeP2Window();
		App->scene_manager->changeScene(App->scene_manager->main_scene, this);
		break;
	case IN_GAME_SETTINGS:
		if (button->focus_id == 0)
			closeP1Window();
		else
			closeP2Window();
		App->scene_manager->changeScene(App->scene_manager->settings_scene, this);
		break;
	case IN_GAME_STAGE_SEL:
		break;
	case MATCH_END_CHAR_SEL:
		closeGeneralWindow();
		App->scene_manager->changeScene(App->scene_manager->char_sel_scene, this);
		break;
	case MATCH_END_MAIN_MENU:
		closeGeneralWindow();
		App->scene_manager->changeScene(App->scene_manager->main_scene, this);
		break;
	case MATCH_END_REMATCH:
		break;
	case MATCH_END_STAGE_SEL:
		break;
	case MATCH_END_SETTINGS:
		closeGeneralWindow();
		App->scene_manager->changeScene(App->scene_manager->settings_scene, this);
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
	int char1_hp = App->entities->players[0]->getCurrCharacter()->getCurrentLife();
	int char2_hp = App->entities->players[1]->getCurrCharacter()->getCurrentLife();
	if (char1_hp <= 0 && !App->entities->paused || char2_hp <= 0 && !App->entities->paused)
		popUpGeneralWindow();
		
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
	if (App->entities->players[0]->getInput(START, KEY_DOWN))	{
		if (p1_window)
			closeP1Window(), App->entities->paused = false;
		else
			popUpP1Window();
	}
	if (App->entities->players[1]->getInput(START, KEY_DOWN))	{
		if (p2_window)
			closeP2Window(), App->entities->paused = false;
		else
			popUpP2Window();
	}
}

void combatScene::popUpP1Window()	{
	App->entities->paused = true;

	if (!p1_window && !p2_window)
	{
		p1_window = (UiWindow*)App->gui->createWindow(PAUSE, { 625, 400 }, this);
		p1_pause_label = (Labels*)App->gui->createLabel("P1 PAUSE", { 255,255,255,255 }, App->fonts->large_size, { 850, 420 }, this);
		p1_char_sel_button = (Buttons*)App->gui->createButton(IN_GAME_CHAR_SEL, MEDIUM, 0, { 815, 525 }, this);
		p1_char_sel_label = (Labels*)App->gui->createLabel("CHARACTER SEL.", { 33,33,33,255 }, App->fonts->medium_size, { 835, 540 }, this);
		p1_stage_sel_button = (Buttons*)App->gui->createButton(IN_GAME_STAGE_SEL, MEDIUM, 0, { 815, 625 }, this);
		p1_stage_sel_label = (Labels*)App->gui->createLabel("STAGE SEL.", { 33,33,33,255 }, App->fonts->medium_size, { 865, 640 }, this);
		p1_settings_button = (Buttons*)App->gui->createButton(IN_GAME_SETTINGS, MEDIUM, 0, { 815, 725 }, this);
		p1_settings_label = (Labels*)App->gui->createLabel("SETTINGS", { 33,33,33,255 }, App->fonts->medium_size, { 875, 740 }, this);
		p1_main_menu_button = (Buttons*)App->gui->createButton(IN_GAME_MAIN_MENU, MEDIUM, 0, { 815, 825 }, this);
		p1_main_menu_label = (Labels*)App->gui->createLabel("MAIN MENU", { 33,33,33,255 }, App->fonts->medium_size, { 870, 840 }, this);
		
		assignFocus();
	}
}

void combatScene::popUpP2Window()	{
	App->entities->paused = true;

	if (!p2_window && !p1_window)
	{
		p2_window = (UiWindow*)App->gui->createWindow(PAUSE, { 625, 400 }, this);
		p2_pause_label = (Labels*)App->gui->createLabel("P2 PAUSE", { 255,255,255,255 }, App->fonts->large_size, { 850, 420 }, this);
		p2_char_sel_button = (Buttons*)App->gui->createButton(IN_GAME_CHAR_SEL, MEDIUM, 1, { 815, 525 }, this);
		p2_char_sel_label = (Labels*)App->gui->createLabel("CHARACTER SEL.", { 33,33,33,255 }, App->fonts->medium_size, { 835, 540 }, this);
		p2_stage_sel_button = (Buttons*)App->gui->createButton(IN_GAME_STAGE_SEL, MEDIUM, 1, { 815, 625 }, this);
		p2_stage_sel_label = (Labels*)App->gui->createLabel("STAGE SEL.", { 33,33,33,255 }, App->fonts->medium_size, { 865, 640 }, this);
		p2_settings_button = (Buttons*)App->gui->createButton(IN_GAME_SETTINGS, MEDIUM, 1, { 815, 725 }, this);
		p2_settings_label = (Labels*)App->gui->createLabel("SETTINGS", { 33,33,33,255 }, App->fonts->medium_size, { 875, 740 }, this);
		p2_main_menu_button = (Buttons*)App->gui->createButton(IN_GAME_MAIN_MENU, MEDIUM, 1, { 815, 825 }, this);
		p2_main_menu_label = (Labels*)App->gui->createLabel("MAIN MENU", { 33,33,33,255 }, App->fonts->medium_size, { 870, 840 }, this);

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
	p1_window = nullptr;
	p1_pause_label->to_delete = true;
	p1_char_sel_label->to_delete = true;
	p1_stage_sel_label->to_delete = true;
	p1_settings_label->to_delete = true;
	p1_main_menu_label->to_delete = true;

	App->entities->players[0]->focus = nullptr;
	
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
	p2_window = nullptr;
	p2_pause_label->to_delete = true;
	p2_char_sel_label->to_delete = true;
	p2_stage_sel_label->to_delete = true;
	p2_settings_label->to_delete = true;
	p2_main_menu_label->to_delete = true;

	App->entities->players[1]->focus = nullptr;

	App->entities->paused = false;
}

void combatScene::popUpGeneralWindow()	{
	App->entities->paused = true;

	if (!general_window)
	{
		general_window = (UiWindow*)App->gui->createWindow(MATCH_END, { 625, 400 }, this);
		
		const char* winner = nullptr;
		int winner_id = 0;

		if (App->entities->players[0]->getCurrCharacter()->getCurrentLife() > 0 && App->entities->players[1]->getCurrCharacter()->getCurrentLife() <= 0)
			winner = "PLAYER 1 WINS!";
		else if (App->entities->players[1]->getCurrCharacter()->getCurrentLife() > 0 && App->entities->players[0]->getCurrCharacter()->getCurrentLife() <= 0)
			winner = "PLAYER 2 WINS!", winner_id = 1;
		else
			winner = "IT'S A DRAW!";

		match_end_label = (Labels*)App->gui->createLabel(winner, { 255,255,255,255 }, App->fonts->large_size, { 800, 420 }, this);
		rematch_button = (Buttons*)App->gui->createButton(MATCH_END_REMATCH, MEDIUM, winner_id, { 815, 495 }, this);
		rematch_label = (Labels*)App->gui->createLabel("REMATCH", { 33,33,33,255 }, App->fonts->medium_size, { 875, 510 }, this);
		char_sel_button = (Buttons*)App->gui->createButton(MATCH_END_CHAR_SEL, MEDIUM, winner_id, { 815, 575 }, this);
		char_sel_label = (Labels*)App->gui->createLabel("CHARACTER SEL.", { 33,33,33,255 }, App->fonts->medium_size, { 835, 590 }, this);
		stage_sel_button = (Buttons*)App->gui->createButton(MATCH_END_STAGE_SEL, MEDIUM, winner_id, { 815, 655 }, this);
		stage_sel_label = (Labels*)App->gui->createLabel("STAGE SEL.", { 33, 33, 33, 255 }, App->fonts->medium_size, { 865, 670 }, this);
		settings_button = (Buttons*)App->gui->createButton(MATCH_END_SETTINGS, MEDIUM, winner_id, { 815, 735 }, this);
		settings_label = (Labels*)App->gui->createLabel("SETTINGS", { 33,33,33,255 }, App->fonts->medium_size, { 875, 750 }, this);
		main_menu_button = (Buttons*)App->gui->createButton(MATCH_END_MAIN_MENU, MEDIUM, winner_id, { 815, 815 }, this);
		main_menu_label = (Labels*)App->gui->createLabel("MAIN MENU", { 33,33,33,255 }, App->fonts->medium_size, { 870, 830 }, this);
	}

	assignFocus();
}

void combatScene::closeGeneralWindow()	{
	//Deleting buttons
	std::list<Widgets*>::reverse_iterator it;
	Widgets* object = nullptr;
	if (!App->gui->p1_focus_elements.empty())
	{
		it = App->gui->p1_focus_elements.rbegin();
		for (it; it != App->gui->p1_focus_elements.rend(); it++)
		{
			object = *it;
			object->to_delete = true;
		}
		App->entities->players[0]->focus = nullptr;
	}
	else if (!App->gui->p2_focus_elements.empty())
	{
		it = App->gui->p2_focus_elements.rbegin();
		for (it; it != App->gui->p2_focus_elements.rend(); it++)
		{
			object = *it;
			object->to_delete = true;
		}
		App->entities->players[1]->focus = nullptr;
	}
	
	general_window->to_delete = true;
	general_window = nullptr;
	match_end_label->to_delete = true;
	char_sel_label->to_delete = true;
	stage_sel_label->to_delete = true;
	settings_label->to_delete = true;
	main_menu_label->to_delete = true;
}
