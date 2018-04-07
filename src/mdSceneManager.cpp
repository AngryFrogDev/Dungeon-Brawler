#include "mdSceneManager.h"
#include "Application.h"
#include "mdEntities.h"
#include "mdGuiManager.h"
#include "mdFonts.h"
#include "mdInput.h"
#include "mdCollision.h"


mdSceneManager::mdSceneManager(){}


mdSceneManager::~mdSceneManager(){}

bool mdSceneManager::awake(const pugi::xml_node & md_config)	{
	//HARDCODE, super easy to make an xml out of this, just sayin'

	//Start scene
	start_scene.type = START_SCENE;
	game_logo = App->textures->load("assets/game_logo_RA.png");

	intro_label.type = LABEL;
	intro_label.pos = { 450, 600 };
	intro_label.label_info.color = { 100,100,100,100 };
	intro_label.label_info.text = "PRESS ENTER";
	intro_label.label_info.font_size = App->fonts->medium_size;
	start_scene.ui_elements.push_back(intro_label);

	//One vs one
	one_vs_one.type = ONE_VS_ONE;

	///Characters
	player1.x_pos = 100;
	player1.type = WARRIOR;
	player1.player = 0;
	player1.flipped = false;
	one_vs_one.characters.push_back(player1);

	player2.x_pos = 1000;
	player2.type = WARRIOR;
	player2.player = 1;
	player2.flipped = true;
	one_vs_one.characters.push_back(player2);

	//Two vs Two
	two_vs_two.type = TWO_VS_TWO;

	///Characters
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
	
	//Main menu
	main_menu.type = MAIN_MENU;
	
	///UI
	b_o_vs_o.type = BUTTON;
	b_o_vs_o.pos = { 475,200 };
	b_o_vs_o.button_type = ONE_V_ONE;
	main_menu.ui_elements.push_back(b_o_vs_o);

	b_t_vs_t.type = BUTTON;
	b_t_vs_t.pos = { 475,350 };
	b_t_vs_t.button_type = ONE_V_ONE;
	main_menu.ui_elements.push_back(b_t_vs_t);

	b_exit.type = BUTTON;
	b_exit.pos = { 475,500 };
	b_exit.button_type = GAME_EXIT;
	main_menu.ui_elements.push_back(b_exit);

	l_o_vs_o.type = LABEL;
	l_o_vs_o.pos = { 490, 215 };
	l_o_vs_o.label_info.color = { 255,255,255,255 };
	l_o_vs_o.label_info.text = "ONE VS ONE";
	l_o_vs_o.label_info.font_size = App->fonts->medium_size;
	main_menu.ui_elements.push_back(l_o_vs_o);

	l_t_vs_t.type = LABEL;
	l_t_vs_t.pos = { 490, 365 };
	l_t_vs_t.label_info.color = { 255,255,255,255 };
	l_t_vs_t.label_info.text = "TWO VS TWO";
	l_t_vs_t.label_info.font_size = App->fonts->medium_size;
	main_menu.ui_elements.push_back(l_t_vs_t);

	l_exit.type = LABEL;
	l_exit.pos = { 550, 515 };
	l_exit.label_info.color = { 255,255,255,255 };
	l_exit.label_info.text = "EXIT";
	l_exit.label_info.font_size = App->fonts->medium_size;
	main_menu.ui_elements.push_back(l_exit);
		

	current_scene = &start_scene;

	return true;
}

bool mdSceneManager::start()	{
	bool ret = true;

	if (current_scene == nullptr)
		return false;

	CreateCharacters();

	CreateWidgets();
	

	return true;
}

bool mdSceneManager::update(float dt)	{
	//PROVISIONAL: All input should be read from controller too
	if (App->input->getKey(SDL_SCANCODE_M) == KEY_DOWN) //"M" from menu :-)
		changeScene(one_vs_one); //To test the code

	if (App->input->getKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
		changeScene(main_menu);

	if (current_scene == &start_scene)//Logo texture
		App->render->blit(1, game_logo, 100, 100, 0, 1);
	
	
	App->gui->draw();
	return true;
}

void mdSceneManager::changeScene(Scene scene_to_load)	{
	App->gui->cleanUI();
	App->collision->cleanUp();
	App->entities->cleanUp();

	current_scene = &scene_to_load;

	CreateCharacters();
	CreateWidgets();
}

bool mdSceneManager::onEvent(Buttons* button)	{
	bool ret = true;

	switch (button->button_type)
	{
	default:
		break;
	case ONE_V_ONE:
		changeScene(one_vs_one);
		break;
	case TWO_V_TWO:
		changeScene(two_vs_two); 
		break;
	case GAME_EXIT:
		ret = false;
		break;
	}

	return ret;
}

bool mdSceneManager::CreateCharacters()
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

bool mdSceneManager::CreateWidgets()
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

	return true;
}

