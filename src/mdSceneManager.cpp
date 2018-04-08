#include "mdSceneManager.h"
#include "Application.h"
#include "mdEntities.h"
#include "mdGuiManager.h"
#include "mdFonts.h"
#include "mdInput.h"
#include "mdCollision.h"


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

	current_scene = &one_vs_one;

	return true;
}

bool mdSceneManager::start()	{
	bool ret = true;

	if (current_scene == nullptr)
		return false;

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

			current_scene = scene_to_load;

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
	
	//PROVISIONAL: All input should be read from controller too
	if (current_scene == &start_scene)//Logo texture
	{
		App->render->blit(1, game_logo, 150, 150, 0, 1);
		if (App->input->getKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
			changeScene(&main_menu);
	}
	
	if (current_scene == &one_vs_one)
	{
		App->render->blit(5, App->gui->atlas, 850, 100, &timer_rect, 3);
		App->render->blit(2, App->gui->atlas, 110, 100, &character1_rect, 3);
		App->render->blit(2, App->gui->atlas, 119, 109, &character1_image, 3);
		App->render->blit(2, App->gui->atlas, 1570, 100, &character2_rect, 3);
		App->render->blit(2, App->gui->atlas, 1579, 109, &character2_image, 3, true);
	}

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
	if (current_scene->ui_elements.empty())
		return false;

	for (auto it = current_scene->ui_elements.begin(); it != current_scene->ui_elements.end(); it++) {
		WidgetInfo curr_widget_info = *it;
		
		switch (curr_widget_info.type)
		{
		case BAR:
			App->gui->createBar(curr_widget_info.bar_type, { curr_widget_info.pos.x,curr_widget_info.pos.y }, curr_widget_info.flip, this);
			break;
		case BUTTON:
			App->gui->createButton(curr_widget_info.button_type, { curr_widget_info.pos.x,curr_widget_info.pos.y }, this);
			break;
		case LABEL:
			App->gui->createLabel(curr_widget_info.label_info.text, curr_widget_info.label_info.color,curr_widget_info.label_info.font_size,{ curr_widget_info.pos.x,curr_widget_info.pos.y }, this);
			break;
		default:
			//It should never go here
			break;
		}
	}

	if (!App->gui->focus_elements.empty())
		App->gui->focus = *App->gui->focus_elements.begin();
	else
		App->gui->focus = nullptr;

	return true;
}

void mdSceneManager::loadSceneUI() {
	//Start scene
	start_scene.type = START_SCENE;
	game_logo = App->textures->load("assets/game_logo_RA.png");

	intro_label.type = LABEL;
	intro_label.pos = { 830, 900 };
	intro_label.label_info.color = { 100,100,100,100 };
	intro_label.label_info.text = "PRESS ENTER";
	intro_label.label_info.font_size = App->fonts->large_size;
	start_scene.ui_elements.push_back(intro_label);

	//Main Menu
	b_o_vs_o.type = BUTTON;
	b_o_vs_o.pos = { 750,200 };
	b_o_vs_o.button_type = ONE_V_ONE;
	main_menu.ui_elements.push_back(b_o_vs_o);

	b_t_vs_t.type = BUTTON;
	b_t_vs_t.pos = { 750,400 };
	b_t_vs_t.button_type = ONE_V_ONE;
	main_menu.ui_elements.push_back(b_t_vs_t);

	b_exit.type = BUTTON;
	b_exit.pos = { 1350,900 };
	b_exit.button_type = GAME_EXIT;
	main_menu.ui_elements.push_back(b_exit);

	l_o_vs_o.type = LABEL;
	l_o_vs_o.pos = { 795, 225 };
	l_o_vs_o.label_info.color = { 255,255,255,255 };
	l_o_vs_o.label_info.text = "ONE VS ONE";
	l_o_vs_o.label_info.font_size = App->fonts->large_size;
	main_menu.ui_elements.push_back(l_o_vs_o);

	l_t_vs_t.type = LABEL;
	l_t_vs_t.pos = { 795, 425 };
	l_t_vs_t.label_info.color = { 255,255,255,255 };
	l_t_vs_t.label_info.text = "TWO VS TWO";
	l_t_vs_t.label_info.font_size = App->fonts->large_size;
	main_menu.ui_elements.push_back(l_t_vs_t);

	l_exit.type = LABEL;
	l_exit.pos = { 1475, 925 };
	l_exit.label_info.color = { 255,255,255,255 };
	l_exit.label_info.text = "QUIT";
	l_exit.label_info.font_size = App->fonts->large_size;
	main_menu.ui_elements.push_back(l_exit);

	//Combat
	timer_rect = { 421, 142, 59, 59 };
	character1_rect = { 6,175,66,34 };
	character1_image = { 82,175,60,28 };
	character2_rect = character3_rect = character4_rect = character1_rect;
	//PROVISIONAL: When we have more than one playable characters, this should be checked with the selected characters. 
	character2_image = character3_image = character4_image = character1_image;


	health_bar1.type = BAR;
	health_bar1.pos = { 100,195 };
	health_bar1.bar_type = HEALTH_BAR;
	one_vs_one.ui_elements.push_back(health_bar1);

	health_bar2.type = BAR;
	health_bar2.pos = { 1030,195 };
	health_bar2.bar_type = HEALTH_BAR;
	health_bar2.flip = true;
	one_vs_one.ui_elements.push_back(health_bar2);

	super_bar1.type = BAR;
	super_bar1.pos = { 109, 222 };
	super_bar1.bar_type = SUPER_BAR;
	one_vs_one.ui_elements.push_back(super_bar1);

	super_bar2.type = BAR;
	super_bar2.pos = { 1271, 222 };
	super_bar2.bar_type = SUPER_BAR;
	super_bar2.flip = true;
	one_vs_one.ui_elements.push_back(super_bar2);

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

	player3.x_pos = 200;
	player3.type = WARRIOR;
	player3.player = 2;
	player3.flipped = false;
	two_vs_two.characters.push_back(player3);

	player4.x_pos = 900;
	player4.type = WARRIOR;
	player4.player = 3;
	player4.flipped = true;
	two_vs_two.characters.push_back(player4);

}

