#include "mdSceneManager.h"
#include "Application.h"
#include "mdEntities.h"
#include "mdGuiManager.h"
#include "mdFonts.h"
#include "mdInput.h"
#include "mdCollision.h"
#include "mdMap.h"
#include "Player.h"
#include "mdProjectiles.h"


mdSceneManager::mdSceneManager()	{
	//PROVISIONAL: Hardcoded
	screen = { 0, 0, 1920, 1080 };
	name = "items";
}


mdSceneManager::~mdSceneManager(){}

bool mdSceneManager::awake(const pugi::xml_node & md_config)	{
	scene_config = App->loadConfig("scene_config.xml", scene_config_doc);
	//PROVISIONAL: HARDCODE, super easy to make an xml out of this, just sayin'

	player1item = md_config.attribute("player_1_item").as_bool();
	player2item = md_config.attribute("player_2_item").as_bool();
	start_scene.type = START_SCENE;
	one_vs_one.type = ONE_VS_ONE;
	two_vs_two.type = TWO_VS_TWO;
	main_menu.type = MAIN_MENU;
	
	loadSceneUI();
	loadSceneCharacters();

	current_scene = &start_scene;

	return true;
}

bool mdSceneManager::start()	{
	bool ret = true;

	if (current_scene == nullptr)
		return false;

	health_bar_target = 0; //provisional
	super_bar_target = 0; //provisional
	createCharacters();
	createWidgets();
		
	SDL_SetRenderDrawBlendMode(App->render->renderer, SDL_BLENDMODE_BLEND);
	return true;
}

bool mdSceneManager::update(float dt)	{
	bool ret = true;

	float normalized = MIN(1.0f, switch_timer.readSec() / fadetime);
	static iPoint temp_cam;

	switch (current_step)
	{
	case fade_step::NONE:
		break;
	case fade_step::FADE_TO_BLACK:
		if (switch_timer.readSec() >= fadetime)
		{
			App->gui->cleanUI();
			App->collision->cleanUp();
			App->entities->cleanUp();
			App->render->cleanBlitQueue();
			App->projectiles->cleanUp();
			App->map->map_loaded = false;
						
			current_scene = scene_to_load;
			if (current_scene == &one_vs_one)
				scene_timer.start(), current_time = max_time;

			createCharacters();
			createWidgets();

			switch_timer.start();
			current_step = fade_step::FADE_FROM_BLACK;
		}
		break;
	case fade_step::FADE_FROM_BLACK:
		normalized = 1.0f - normalized;

		if (switch_timer.readSec() >= fadetime)
			current_step = fade_step::NONE;
		break;
	default:
		break;
	}

	if (current_step != fade_step::NONE) {
		SDL_SetRenderDrawColor(App->render->renderer, 0, 0, 0, (Uint8)(normalized * 255.0f));
		SDL_RenderFillRect(App->render->renderer, &screen);
	}
	
	if (App->input->getKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;
	if (App->input->getKey(SDL_SCANCODE_F3) == KEY_DOWN)
		changeScene(&main_menu);

	if (current_scene == &one_vs_one)//Should be moved to entities?
	{
		updateTimer();
		int char1_hp = App->entities->players[0]->getCurrCharacter()->getCurrentLife();
		int char2_hp = App->entities->players[1]->getCurrCharacter()->getCurrentLife();
		if (char1_hp <= 0 || char2_hp <= 0)
			changeScene(&main_menu);
	}
	
	blitUiTextures();
	App->gui->draw();

	return ret;
}

bool mdSceneManager::changeScene(Scene* _scene_to_load)	{
	bool ret = false;
	if (current_step == fade_step::NONE)
	{
		current_step = fade_step::FADE_TO_BLACK;
		switch_timer.start();
		scene_to_load = _scene_to_load;
		ret = true;
	}

	return ret;
}

bool mdSceneManager::onEvent(Buttons* button)	{
	bool ret = true;

	switch (button->button_type)
	{
	default:
		break;
	case ONE_V_ONE:
		App->entities->traning = false;
		changeScene(&one_vs_one);
		break;
	case TRANING:
		App->entities->traning = true;
		changeScene(&one_vs_one);
		break;
	case TWO_V_TWO:
		changeScene(&two_vs_two); 
		break;
	case GAME_EXIT:
		ret = false;
		break;
	case IN_GAME_MAIN_MENU:
		changeScene(&main_menu);
		break;
	case IN_GAME_REMATCH:
		//PROVISIONAL
		App->entities->players[0]->getCurrCharacter()->resetCharacter();
		App->entities->players[1]->getCurrCharacter()->resetCharacter();
		//changeScene(&one_vs_one);
		break;
	}

	return ret;
}

bool mdSceneManager::createCharacters()
{	
	if (current_scene->characters.empty())
		return false;

	if (current_scene->characters.size() == 2) {
		for (auto it = current_scene->characters.begin(); it != current_scene->characters.end(); it++) {
			CharacterInfo curr_character_info = *it;
			App->entities->createPlayer(curr_character_info.player, curr_character_info.x_pos, curr_character_info.type, curr_character_info.flipped, 1);
		}
	}

	if (current_scene->characters.size() == 4) {

		bool alternate = true; //The idea is to place character 0 and 2 in the same lane, as well as 1 and 3

		for (auto it = current_scene->characters.begin(); it != current_scene->characters.end(); it++) {
			CharacterInfo curr_character_info = *it;

			if (alternate == true)
				App->entities->createPlayer(curr_character_info.player, curr_character_info.x_pos, curr_character_info.type, curr_character_info.flipped, 1);
			else //(alternate == false)
				App->entities->createPlayer(curr_character_info.player, curr_character_info.x_pos, curr_character_info.type, curr_character_info.flipped, 2);

			alternate = !alternate;
		}

		//Very dangerous hardcode to set the partners: 
		App->entities->assignPartners();
	}

	//This will need to change
	if (player1item)
		App->entities->players[0]->getCurrCharacter()->giveItem(SPECIAL_ITEM_1);
	else
		App->entities->players[0]->getCurrCharacter()->giveItem(SPECIAL_ITEM_2);

	if (player2item)
		App->entities->players[1]->getCurrCharacter()->giveItem(SPECIAL_ITEM_1);
	else
		App->entities->players[1]->getCurrCharacter()->giveItem(SPECIAL_ITEM_2);

	App->entities->assignControls();

	return true;
}

bool mdSceneManager::createWidgets()
{
	if (current_scene->scene_ui_elems.empty())
		return false;

	Widgets* object = nullptr;
	for (auto it = current_scene->scene_ui_elems.begin(); it != current_scene->scene_ui_elems.end(); it++) {
		object = *it;
		if (object->active)
			object->visible = true;
	}

	for (auto it = App->gui->focus_elements.begin(); it != App->gui->focus_elements.end(); it++) {
		object = *it;
		if (object->visible)
		{
			App->gui->focus = object;
			break;
		}
		else
			App->gui->focus = nullptr;
	}

	return true;
}

void mdSceneManager::loadSceneUI() {
	//START SCENE
	//Preparing nodes
	start_scene.scene_data = scene_config.child("start_scene");
	labels_node = start_scene.scene_data.child("labels");
	textures_node = start_scene.scene_data.child("textures");

	//Loading variables 
	//
	game_logo = App->textures->load(textures_node.child("game_logo").attribute("path").as_string());
	intro_label = (Labels*)App->gui->createLabel(labels_node.child("content").attribute("value").as_string(),{ (Uint8)labels_node.child("color").attribute("r").as_int(),(Uint8)labels_node.child("color").attribute("g").as_int(),(Uint8)labels_node.child("color").attribute("b").as_int(),(Uint8)labels_node.child("color").attribute("a").as_int() }, 
	App->fonts->large_size, { labels_node.child("pos").attribute("x").as_int(),labels_node.child("pos").attribute("y").as_int() }, this);
	intro_label->active = labels_node.child("active").attribute("value").as_bool();
	start_scene.scene_ui_elems.push_back(intro_label);


	//MAIN MENU
	//Preparing nodes
	main_menu.scene_data = scene_config.child("main_menu");
	buttons_node = main_menu.scene_data.child("buttons");
	labels_node = main_menu.scene_data.child("labels");

	//Loading variables
	b_o_vs_o = (Buttons*)App->gui->createButton(ONE_V_ONE, LARGE, { buttons_node.child("o_vs_o").child("pos").attribute("x").as_int(), buttons_node.child("o_vs_o").child("pos").attribute("y").as_int() }, this);
	b_o_vs_o->active = buttons_node.child("o_vs_o").child("active").attribute("value").as_bool();
	main_menu.scene_ui_elems.push_back(b_o_vs_o);
/*
	b_t_vs_t = (Buttons*)App->gui->createButton(TWO_V_TWO, LARGE, { buttons_node.child("t_vs_t").child("pos").attribute("x").as_int(), buttons_node.child("t_vs_t").child("pos").attribute("y").as_int() }, this);
	b_t_vs_t->active = buttons_node.child("t_vs_t").child("active").attribute("value").as_bool();
	main_menu.scene_ui_elems.push_back(b_t_vs_t);
*/
	// PROVISIONAL
	b_training = (Buttons*)App->gui->createButton(TRANING, LARGE, { 750, 600 }, this);
	b_training->active = true;
	main_menu.scene_ui_elems.push_back(b_training);

	b_exit = (Buttons*)App->gui->createButton(GAME_EXIT, LARGE, { buttons_node.child("exit").child("pos").attribute("x").as_int(), buttons_node.child("exit").child("pos").attribute("y").as_int() }, this);
	b_exit->active = buttons_node.child("exit").child("active").attribute("value").as_bool();
	main_menu.scene_ui_elems.push_back(b_exit);

	l_o_vs_o = (Labels*)App->gui->createLabel(labels_node.child("o_vs_o").child("content").attribute("value").as_string(), { (Uint8)labels_node.child("o_vs_o").child("color").attribute("r").as_int(),(Uint8)labels_node.child("o_vs_o").child("color").attribute("g").as_int(),(Uint8)labels_node.child("o_vs_o").child("color").attribute("b").as_int(),(Uint8)labels_node.child("o_vs_o").child("color").attribute("a").as_int()},
	App->fonts->large_size, { labels_node.child("o_vs_o").child("pos").attribute("x").as_int(), labels_node.child("o_vs_o").child("pos").attribute("y").as_int() }, this);
	l_o_vs_o->active = labels_node.child("o_vs_o").child("active").attribute("value").as_bool();
	main_menu.scene_ui_elems.push_back(l_o_vs_o);
/*
	l_t_vs_t = (Labels*)App->gui->createLabel(labels_node.child("t_vs_t").child("content").attribute("value").as_string(), { (Uint8)labels_node.child("t_vs_t").child("color").attribute("r").as_int(),(Uint8)labels_node.child("t_vs_t").child("color").attribute("g").as_int(),(Uint8)labels_node.child("t_vs_t").child("color").attribute("b").as_int(),(Uint8)labels_node.child("t_vs_t").child("color").attribute("a").as_int() },
	App->fonts->large_size, { labels_node.child("t_vs_t").child("pos").attribute("x").as_int(), labels_node.child("t_vs_t").child("pos").attribute("y").as_int() }, this);
	l_t_vs_t->active = labels_node.child("t_vs_t").child("active").attribute("value").as_bool();
	main_menu.scene_ui_elems.push_back(l_t_vs_t);
*/
	l_traning = (Labels*)App->gui->createLabel("TRAINING", { 255,255,255,255 },App->fonts->large_size, {825, 625}, this);
	l_traning->active = true;
	main_menu.scene_ui_elems.push_back(l_traning);
	
	l_exit = (Labels*)App->gui->createLabel(labels_node.child("exit").child("content").attribute("value").as_string(), { (Uint8)labels_node.child("exit").child("color").attribute("r").as_int(),(Uint8)labels_node.child("exit").child("color").attribute("g").as_int(),(Uint8)labels_node.child("exit").child("color").attribute("b").as_int(),(Uint8)labels_node.child("exit").child("color").attribute("a").as_int() },
	App->fonts->large_size, { labels_node.child("exit").child("pos").attribute("x").as_int(), labels_node.child("exit").child("pos").attribute("y").as_int() }, this);
	l_exit->active = labels_node.child("exit").child("active").attribute("value").as_bool();
	main_menu.scene_ui_elems.push_back(l_exit);


	//COMBAT
	//Preparing nodes
	one_vs_one.scene_data = scene_config.child("combat").child("one_vs_one");
	labels_node = one_vs_one.scene_data.child("labels");
	buttons_node = one_vs_one.scene_data.child("buttons");
	bars_node = one_vs_one.scene_data.child("bars");
	textures_node = one_vs_one.scene_data.child("textures");

	timer_rect = { 421, 142, 59, 59 };
	character1_rect = { 6,175,66,34 };
	character1_image = { 82,175,60,28 };
	character2_rect = character3_rect = character4_rect = character1_rect;
	//PROVISIONAL: When we have more than one playable characters, this should be checked with the selected characters. 
	character2_image = character3_image = character4_image = character1_image;

	auto label_string = std::to_string(current_time);
	timer = (Labels*)App->gui->createLabel(label_string.data(), { (Uint8)labels_node.child("timer").child("color").attribute("r").as_int(),(Uint8)labels_node.child("timer").child("color").attribute("g").as_int(),(Uint8)labels_node.child("timer").child("color").attribute("b").as_int(),(Uint8)labels_node.child("timer").child("color").attribute("a").as_int() }, 
	App->fonts->extra_large_size, { labels_node.child("timer").child("pos").attribute("x").as_int(),labels_node.child("timer").child("pos").attribute("y").as_int() }, this);
	timer->active = labels_node.child("timer").child("active").attribute("value").as_bool();
	one_vs_one.scene_ui_elems.push_back(timer);

	health_bar1 = (Bars*)App->gui->createBar(HEALTH_BAR, { bars_node.child("health_bar1").child("pos").attribute("x").as_int(),bars_node.child("health_bar1").child("pos").attribute("y").as_int() },
	bars_node.child("health_bar1").child("flip").attribute("value").as_bool(), bars_node.child("health_bar1").child("target_player").attribute("value").as_int(), this);
	health_bar1->active = bars_node.child("health_bar1").child("active").attribute("value").as_bool();
	one_vs_one.scene_ui_elems.push_back(health_bar1);

	health_bar2 = (Bars*)App->gui->createBar(HEALTH_BAR, { bars_node.child("health_bar2").child("pos").attribute("x").as_int(),bars_node.child("health_bar2").child("pos").attribute("y").as_int() },
	bars_node.child("health_bar2").child("flip").attribute("value").as_bool(), bars_node.child("health_bar2").child("target_player").attribute("value").as_int(), this);
	health_bar2->active = bars_node.child("health_bar2").child("active").attribute("value").as_bool(); 
	one_vs_one.scene_ui_elems.push_back(health_bar2);

	super_bar1 = (Bars*)App->gui->createBar(SUPER_BAR, { bars_node.child("super_bar1").child("pos").attribute("x").as_int(),bars_node.child("super_bar1").child("pos").attribute("y").as_int() }, 
	bars_node.child("super_bar1").child("flip").attribute("value").as_bool(), bars_node.child("super_bar1").child("target_player").attribute("value").as_int(), this);
	super_bar1->active = bars_node.child("super_bar1").child("active").attribute("value").as_bool();
	one_vs_one.scene_ui_elems.push_back(super_bar1);

	super_bar2 = (Bars*)App->gui->createBar(SUPER_BAR, { bars_node.child("super_bar2").child("pos").attribute("x").as_int(),bars_node.child("super_bar2").child("pos").attribute("y").as_int() },
	bars_node.child("super_bar2").child("flip").attribute("value").as_bool(), bars_node.child("super_bar2").child("target_player").attribute("value").as_int(), this);
	super_bar2->active = bars_node.child("super_bar2").child("active").attribute("value").as_bool();
	one_vs_one.scene_ui_elems.push_back(super_bar2);

	//WINDOWS AND RELATED UI
	rematch = (Buttons*)App->gui->createButton(IN_GAME_REMATCH, MEDIUM, {buttons_node.child("rematch").child("pos").attribute("x").as_int(), buttons_node.child("rematch").child("pos").attribute("y").as_int()}, this);
	rematch->active = buttons_node.child("rematch").child("active").attribute("value").as_bool();
	one_vs_one.scene_ui_elems.push_back(rematch);

	to_main_menu = (Buttons*)App->gui->createButton(IN_GAME_MAIN_MENU, MEDIUM, { 100,100 }, this);
	to_main_menu->active = false;
	one_vs_one.scene_ui_elems.push_back(to_main_menu);

	window = (UiWindow*)App->gui->createWindow(MATCH_END, { 600,400 }, this);
	window->active = false;
	one_vs_one.scene_ui_elems.push_back(window);

/* This needs to be revised, but for the moment we won't use 2vs2

	health_bar1.pos = { 100, 125 };
	two_vs_two.ui_elements.push_back(health_bar1);
	health_bar2.pos = { 1030, 125 };
	two_vs_two.ui_elements.push_back(health_bar2);
	super_bar1.pos = { 109, 152 };
	two_vs_two.ui_elements.push_back(super_bar1);
	super_bar2.pos = { 1271, 152 };
	two_vs_two.ui_elements.push_back(super_bar2);

	health_bar3.type = BAR;
	health_bar3.pos = { 100,240 };
	health_bar3.bar_type = HEALTH_BAR;
	two_vs_two.ui_elements.push_back(health_bar3);

	health_bar4.type = BAR;
	health_bar4.pos = { 1030,240 };
	health_bar4.bar_type = HEALTH_BAR;
	health_bar4.flip = true;
	two_vs_two.ui_elements.push_back(health_bar4);
	*/
}

void mdSceneManager::loadSceneCharacters()	{
	//1VS1
	player1.x_pos = 400;
	player1.type = WARRIOR;
	player1.player = 0;
	player1.flipped = false;
	one_vs_one.characters.push_back(player1);

	player2.x_pos = 1500;
	player2.type = WARRIOR;
	player2.player = 1;
	player2.flipped = true;
	one_vs_one.characters.push_back(player2);

	//2VS2
	two_vs_two.characters.push_back(player1); //Perfectly reusable
	two_vs_two.characters.push_back(player2); //Perfectly reusable

	player3.x_pos = 1500;
	player3.type = WARRIOR;
	player3.player = 2;
	player3.flipped = false;
	two_vs_two.characters.push_back(player3);

	player4.x_pos = 400;
	player4.type = WARRIOR;
	player4.player = 3;
	player4.flipped = true;
	two_vs_two.characters.push_back(player4);

}

void mdSceneManager::updateTimer()	{
	if (paused || App->entities->traning)
		return;

	if (scene_timer.readSec() >= 1)
		current_time--, scene_timer.start();
	if (current_time == 0)
	{
		current_time = max_time;
		//PROVISIONAL
		paused = true;
		window->active = true;
		rematch->active = true;
		to_main_menu->active = true;
		createWidgets();
		//current_time = max_time;
		//
		scene_timer.start();
	}
	if (current_time > 0)
	{
		auto label_string = std::to_string(current_time);
		timer->changeContent(label_string.data());
	}
	
}
//PROVISIONAL: This function does a lot of things, should be split up into different things
void mdSceneManager::blitUiTextures()	{
	
	if (current_scene == &start_scene)//Logo texture
	{
		App->render->drawSprite(1, game_logo, 150, 150, 0, 1);
		Controller* temp = nullptr;
		if (!App->input->getController().empty())
			temp = App->input->getController().front();
		if (App->input->getKey(SDL_SCANCODE_RETURN) == KEY_DOWN || (temp != nullptr && temp->isPressed(CONTROLLER_BUTTON::BUTTON_A)))
			changeScene(&main_menu);
	}

	if (current_scene == &one_vs_one || current_scene == &two_vs_two || current_scene == &main_menu)
		App->map->map_loaded = true;


	if (current_scene == &one_vs_one || current_scene == &two_vs_two)
	{
		App->render->drawSprite(2, App->gui->atlas, 850, 100, &timer_rect, 3);
		if (current_scene == &one_vs_one)
		{
			App->render->drawSprite(2, App->gui->atlas, 110, 100, &character1_rect, 3);
			App->render->drawSprite(3, App->gui->atlas, 119, 109, &character1_image, 3);
			App->render->drawSprite(2, App->gui->atlas, 1570, 100, &character2_rect, 3);
			App->render->drawSprite(3, App->gui->atlas, 1579, 109, &character2_image, 3, true);
		}
	}
}



