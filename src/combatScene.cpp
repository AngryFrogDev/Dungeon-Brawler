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

	setRects();
}


combatScene::~combatScene()	{}

bool combatScene::start()	{
	char1 = App->entities->players[0]->getCurrCharacter()->getType();
	char2 = App->entities->players[1]->getCurrCharacter()->getType();
	
	loadSceneUi();
	resetSceneValues();

	return true;
}

bool combatScene::update(float dt)	{
	//Temporary
	char1_hp = App->entities->players[0]->getCurrCharacter()->getCurrentLife();
	char2_hp = App->entities->players[1]->getCurrCharacter()->getCurrentLife();
	
	checkSceneInput();
	loadSceneTextures();	
	updateSceneTimer();
	manageRounds();
	checkTimers();
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
		if (button->focus_id == 0)
			closeP1Window();
		else
			closeP2Window();
		App->scene_manager->changeScene(App->scene_manager->stage_sel_scene, this);
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
		closeGeneralWindow();
		rematching = true;
		App->scene_manager->changeScene(App->scene_manager->combat_scene, this);
		break;
	case MATCH_END_STAGE_SEL:
		closeGeneralWindow();
		App->scene_manager->changeScene(App->scene_manager->stage_sel_scene, this);
		break;
	case MATCH_END_SETTINGS:
		closeGeneralWindow();
		App->scene_manager->changeScene(App->scene_manager->settings_scene, this);
		break;
	}

	return ret;
}

void combatScene::loadSceneUi() {
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

void combatScene::updateSceneTimer()	{
	if (App->entities->paused || App->entities->traning || !scene_timer.isActive())
		return;

	if (scene_timer.readSec() >= 1)
		current_time--, scene_timer.start();
	if (current_time == 0)
	{
		if (rounds_left != 0)
			manageRounds();
		else
		{
			current_time = 0;
			taunt_timer.start();
		}
	}
	if (current_time >= 0)
	{
		auto label_string = std::to_string(current_time);
		timer->changeContent(label_string.data(), { 0,0,0,255 });
	}
}

void combatScene::loadSceneTextures()	{
	//CHARACTER IMAGES
	switch (char1)
	{
	case WARRIOR:
		character1_image = warrior_rect;
		break;
	case MAGE:
		character1_image = mage_rect;
		break;
	case ROGUE:
		character1_image = rogue_rect;
		break;
	case PALADIN:
		character1_image = paladin_rect;
		break;
	default:
		break;
	}

	switch (char2)
	{
	case WARRIOR:
		character2_image = warrior_rect;
		break;
	case MAGE:
		character2_image = mage_rect;
		break;
	case ROGUE:
		character2_image = rogue_rect;
		break;
	case PALADIN:
		character2_image = paladin_rect;
		break;
	default:
		break;
	}

	App->render->drawSprite(4, App->gui->atlas, 567, 70, &timer_rect, 2, 0, 0, 0, 0, false);
	App->render->drawSprite(3, App->gui->atlas, 110, 100, &character1_rect, 3, false, 0, 0, 0, 0, false);
	App->render->drawSprite(3, App->gui->atlas, 1570, 100, &character2_rect, 3, false, 0, 0, 0, 0, false);
	App->render->drawSprite(4, App->gui->atlas, 119, 109, &character1_image, 3, false, 0, 0, 0, 0, false);
	App->render->drawSprite(4, App->gui->atlas, 1579, 109, &character2_image, 3, true, 0, 0, 0, 0, false);

	//ROUND INDICATORS
	switch (p1_rounds_won)
	{
	case 0:
		App->render->drawSprite(4, App->gui->atlas, 680, 265, &still_round_indicator, 4, false, 1.0f, 0, 0, 0, false);
		App->render->drawSprite(4, App->gui->atlas, 780, 265, &still_round_indicator, 4, false, 1.0f, 0, 0, 0, false);
		break;
	case 1:
		App->render->drawSprite(4, App->gui->atlas, 680, 265, &still_round_indicator, 4, false, 1.0f, 0, 0, 0, false);
		App->render->drawSprite(4, App->gui->atlas, 782, 266, &won_round_indicator, 4, false, 1.0f, 0, 0, 0, false);
		break;
	case 2:
		App->render->drawSprite(4, App->gui->atlas, 682, 266, &won_round_indicator, 4, false, 1.0f, 0, 0, 0, false);
		App->render->drawSprite(4, App->gui->atlas, 782, 266, &won_round_indicator, 4, false, 1.0f, 0, 0, 0, false);
		break;
	}
	switch (p2_rounds_won)
	{
	case 0:
		App->render->drawSprite(4, App->gui->atlas, 1060, 265, &still_round_indicator, 4, false, 1.0f, 0, 0, 0, false);
		App->render->drawSprite(4, App->gui->atlas, 1160, 265, &still_round_indicator, 4, false, 1.0f, 0, 0, 0, false);
		break;
	case 1:
		App->render->drawSprite(4, App->gui->atlas, 1062, 266, &won_round_indicator, 4, false, 1.0f, 0, 0, 0, false);
		App->render->drawSprite(4, App->gui->atlas, 1160, 265, &still_round_indicator, 4, false, 1.0f, 0, 0, 0, false);
		break;
	case 2:
		App->render->drawSprite(4, App->gui->atlas, 1062, 266, &won_round_indicator, 4, false, 1.0f, 0, 0, 0, false);
		App->render->drawSprite(4, App->gui->atlas, 1162, 266, &won_round_indicator, 4, false, 1.0f, 0, 0, 0, false);
		break;
	}
}

void combatScene::setRects()	{
	//PROVISIONAL
	timer_rect = { 421, 142, 59, 59 };
	character1_rect = { 6,175,66,34 };
	character2_rect = character1_rect;

	pugi::xml_node warrior = textures_node.child("warrior_image");
	pugi::xml_node mage = textures_node.child("mage_image");
	pugi::xml_node rogue = textures_node.child("rogue_image");
	pugi::xml_node paladin = textures_node.child("paladin_image");

	warrior_rect.x = warrior.attribute("x").as_int();
	warrior_rect.y = warrior.attribute("y").as_int();
	warrior_rect.w = warrior.attribute("w").as_int();
	warrior_rect.h = warrior.attribute("h").as_int();

	mage_rect.x = mage.attribute("x").as_int();
	mage_rect.y = mage.attribute("y").as_int();
	mage_rect.w = mage.attribute("w").as_int();
	mage_rect.h = mage.attribute("h").as_int();

	rogue_rect.x = rogue.attribute("x").as_int();
	rogue_rect.y = rogue.attribute("y").as_int();
	rogue_rect.w = rogue.attribute("w").as_int();
	rogue_rect.h = rogue.attribute("h").as_int();

	paladin_rect.x = paladin.attribute("x").as_int();
	paladin_rect.y = paladin.attribute("y").as_int();
	paladin_rect.w = paladin.attribute("w").as_int();
	paladin_rect.h = paladin.attribute("h").as_int();

	still_round_indicator = { 283, 132, 8, 5 };
	won_round_indicator = { 293, 132, 12, 7 };
}

void combatScene::assignFocus()	{
	if (!App->gui->p1_focus_elements.empty() && !App->entities->players[0]->focus)
		App->entities->players[0]->focus = *App->gui->p1_focus_elements.begin();

	if (!App->gui->p2_focus_elements.empty() && !App->entities->players[1]->focus)
		App->entities->players[1]->focus = *App->gui->p2_focus_elements.begin();
}

void combatScene::checkSceneInput()	{
	if (char1_hp <= 0 || char2_hp <= 0)
		return;

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
}

void combatScene::popUpGeneralWindow()	{
	App->entities->paused = true;

	if (!general_window)
	{
		general_window = (UiWindow*)App->gui->createWindow(MATCH_END, { 625, 400 }, this);
		
		const char* winner = nullptr;
		int winner_id = 0;

		if (p1_rounds_won > p2_rounds_won) //Player 1 wins
			winner = "PLAYER 1 WINS!";
		else if (p1_rounds_won < p2_rounds_won)// Player 2 wins
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
	rematch_label->to_delete = true;
	char_sel_label->to_delete = true;
	stage_sel_label->to_delete = true;
	settings_label->to_delete = true;
	main_menu_label->to_delete = true;
}

void combatScene::resetSceneValues()	{
	//Resetting characters
	App->entities->players[0]->getCurrCharacter()->resetCharacter();
	App->entities->players[1]->getCurrCharacter()->resetCharacter();
	//Resetting camera
	App->render->camera.x = (App->render->resolution.first - App->render->camera.w) / 2;
	//Timer
	max_time = 99;
	current_time = max_time;
	taunt_timer.stop();
	round_timer.stop();
	scene_timer.start();
	//Bools
	App->map->map_loaded = true;
	App->entities->paused = false;
	App->entities->show = true;
	rematching = false;
	//Setting windows to nullptr
	general_window = nullptr;
	p1_window = nullptr;
	p2_window = nullptr;
	//Rounds (only if not going to next round)
	if (!next_round)
	{
		max_rounds = 3;
		rounds_left = max_rounds;
		p1_rounds_won = 0;
		p2_rounds_won = 0;
		extra_round = false;
	}
	
}

void combatScene::checkTimers()	{
	if (taunt_timer.isActive() || round_timer.isActive())
		scene_timer.stop();

	if (taunt_timer.readSec() >= 1.5 && !general_window)
	{
		if (char1_hp >= 0 && char2_hp <= 0 || char1_hp > char2_hp)
			App->entities->players[0]->getCurrCharacter()->tauntFor(2);
		else if (char2_hp >= 0 && char1_hp <= 0 || char2_hp > char1_hp)
			App->entities->players[1]->getCurrCharacter()->tauntFor(2);
		else
			App->entities->players[0]->getCurrCharacter()->tauntFor(2), App->entities->players[1]->getCurrCharacter()->tauntFor(2);

		//Starting taunt timer
		if (taunt_timer.readSec() >= 3.5)
			popUpGeneralWindow();
	}

	if (round_timer.readSec() >= 2 && !taunt_timer.isActive())
		next_round = true, App->scene_manager->changeScene(App->scene_manager->combat_scene, this);
}

void combatScene::manageRounds()	{
	if (taunt_timer.isActive() || App->entities->paused || round_timer.isActive())
		return;

	//case players die
	if (current_time > 0)
	{
		if (char1_hp <= 0 || char2_hp <= 0)
		{
			if (char1_hp > 0 && char2_hp <= 0) //Player 2 dies
				p1_rounds_won++, rounds_left--;

			else if (char2_hp > 0 && char1_hp <= 0)//Player 1 dies
				p2_rounds_won++, rounds_left--;
			
			if (p1_rounds_won == 1 && p2_rounds_won == 1)
				extra_round = true;
			else
				extra_round = false;
			
			if (rounds_left > 1 || extra_round)
				rematching = true, round_timer.start(), App->entities->paused = true;
		}
	}
	
	else
	{
		if (char1_hp > char2_hp) // Player 1 wins
			p1_rounds_won++, rounds_left--;
		else if (char2_hp > char1_hp) // Player2 wins
			p2_rounds_won++, rounds_left--;

		if (p1_rounds_won == 1 && p2_rounds_won == 1)
			extra_round = true;
		else
			extra_round = false;

		if (rounds_left > 1 || extra_round)
			rematching = true, round_timer.start(), App->entities->paused = true;
	}

	if (rounds_left >= 0 && rounds_left <= 1 && !extra_round)
		taunt_timer.start(), next_round = false;
	
}

