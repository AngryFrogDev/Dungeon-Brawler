#include "mdSceneManager.h"
#include "Application.h"
#include "mdEntities.h"
#include "mdGuiManager.h"
#include "mdFonts.h"
#include "mdInput.h"
#include "mdCollision.h"
#include "mdMap.h"
#include "Player.h"


mdSceneManager::mdSceneManager()	{
	//PROVISIONAL: Hardcoded
	screen = { 0, 0, 1920, 1080 };
}


mdSceneManager::~mdSceneManager(){}

bool mdSceneManager::awake(const pugi::xml_node & md_config)	{
	//HARDCODE, super easy to make an xml out of this, just sayin'
	
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
	swap_bar_target = 0; //provisional
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
			App->map->map_loaded = false;
						
			current_scene = scene_to_load;
			if (current_scene == &one_vs_one)
				scene_timer.start();

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
		changeScene(&one_vs_one);
		break;
	case TWO_V_TWO:
		changeScene(&two_vs_two); 
		break;
	case GAME_EXIT:
		ret = false;
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
		object->active = true;
	}

	for (auto it = App->gui->focus_elements.begin(); it != App->gui->focus_elements.end(); it++) {
		object = *it;
		if (object->active)
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
	//Start scene
	start_scene.type = START_SCENE;
	game_logo = App->textures->load("assets/game_logo_RA.png");

	intro_label = (Labels*)App->gui->createLabel("PRESS ENTER", { 100,100,100,100 }, App->fonts->large_size, { 830,900 }, this);
	start_scene.scene_ui_elems.push_back(intro_label);

	//Main Menu
	b_o_vs_o = (Buttons*)App->gui->createButton(ONE_V_ONE, LARGE, { 750, 200 }, this);
	main_menu.scene_ui_elems.push_back(b_o_vs_o);

	b_t_vs_t = (Buttons*)App->gui->createButton(TWO_V_TWO, LARGE, { 750,400 }, this);
	main_menu.scene_ui_elems.push_back(b_t_vs_t);

	b_exit = (Buttons*)App->gui->createButton(GAME_EXIT, LARGE, { 1350,900 }, this);
	main_menu.scene_ui_elems.push_back(b_exit);

	l_o_vs_o = (Labels*)App->gui->createLabel("ONE VS ONE", { 255,255,255,255 }, App->fonts->large_size, { 795, 225 }, this);
	main_menu.scene_ui_elems.push_back(l_o_vs_o);

	l_t_vs_t = (Labels*)App->gui->createLabel("TWO VS TWO", { 255,255,255,255 }, App->fonts->large_size, { 795, 425 }, this);
	main_menu.scene_ui_elems.push_back(l_t_vs_t);

	l_exit = (Labels*)App->gui->createLabel("QUIT", { 255,255,255,255 }, App->fonts->large_size, { 1475, 925 }, this);
	main_menu.scene_ui_elems.push_back(l_exit);
	

	//Combat
	timer_rect = { 421, 142, 59, 59 };
	character1_rect = { 6,175,66,34 };
	character1_image = { 82,175,60,28 };
	character2_rect = character3_rect = character4_rect = character1_rect;
	//PROVISIONAL: When we have more than one playable characters, this should be checked with the selected characters. 
	character2_image = character3_image = character4_image = character1_image;

	auto label_string = std::to_string(current_time);
	timer = (Labels*)App->gui->createLabel(label_string.data(), { 0,0,0,0 }, App->fonts->extra_large_size, { 880,150 }, this);
	one_vs_one.scene_ui_elems.push_back(timer);

	health_bar1 = (Bars*)App->gui->createBar(HEALTH_BAR, { 100,195 }, false, 0, this);
	one_vs_one.scene_ui_elems.push_back(health_bar1);

	health_bar2 = (Bars*)App->gui->createBar(HEALTH_BAR, { 1030, 195 }, true, 1, this);
	one_vs_one.scene_ui_elems.push_back(health_bar2);

	super_bar1 = (Bars*)App->gui->createBar(SUPER_BAR, { 109,222 }, false, 0, this);
	one_vs_one.scene_ui_elems.push_back(super_bar1);

	super_bar2 = (Bars*)App->gui->createBar(SUPER_BAR, { 1271, 222 }, true, 1, this);
	one_vs_one.scene_ui_elems.push_back(super_bar2);

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
	if (paused)
		return;

	if (scene_timer.readSec() >= 1)
		current_time--, scene_timer.start();
	if (current_time == 0)
	{
		current_time = max_time;
		scene_timer.start();
		changeScene(&main_menu);
	}
	auto label_string = std::to_string(current_time);
	timer->changeContent(label_string.data());

}
//PROVISIONAL: This function does a lot of things, should be split up into different things
void mdSceneManager::blitUiTextures()	{
	//PROVISIONAL: All input should be read from controller too
	if (current_scene == &start_scene)//Logo texture
	{
		App->render->blit(1, game_logo, 150, 150, 0, 1);
		if (App->input->getKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
			changeScene(&main_menu);
	}

	if (current_scene == &one_vs_one || current_scene == &two_vs_two || current_scene == &main_menu)
		App->map->map_loaded = true;


	if (current_scene == &one_vs_one || current_scene == &two_vs_two)
	{
		App->render->blit(2, App->gui->atlas, 850, 100, &timer_rect, 3);
		if (current_scene == &one_vs_one)
		{
			App->render->blit(2, App->gui->atlas, 110, 100, &character1_rect, 3);
			App->render->blit(3, App->gui->atlas, 119, 109, &character1_image, 3);
			App->render->blit(2, App->gui->atlas, 1570, 100, &character2_rect, 3);
			App->render->blit(3, App->gui->atlas, 1579, 109, &character2_image, 3, true);
		}
	}
}

