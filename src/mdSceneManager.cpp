#include "mdSceneManager.h"
#include "Application.h"
#include "mdEntities.h"
#include "mdGuiManager.h"
#include "mdFonts.h"
#include "mdInput.h"


mdSceneManager::mdSceneManager()
{
}


mdSceneManager::~mdSceneManager()
{
}

bool mdSceneManager::awake(const pugi::xml_node & md_config)
{
	//HARDCODE, super easy to make an xml out of this, just sayin'

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

	///UI 
	left_health_bar.pos = { 100,10 };
	left_health_bar.type = BAR;
	left_health_bar.bar_type = HEALTH_BAR;
	one_vs_one.ui_elements.push_back(left_health_bar);

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
	random_button.type = BUTTON;
	random_button.pos = { 400,100 };
	random_button.button_type = NEW_GAME;
	main_menu.ui_elements.push_back(random_button);

	random_label.type = LABEL;
	random_label.pos = { 400,0 };
	random_label.label_info.color = { 100,100,100,100 };
	random_label.label_info.text = "PENE";
	main_menu.ui_elements.push_back(random_label);


	current_scene = &one_vs_one;

	return true;
}

bool mdSceneManager::start()
{
	bool ret = true;

	if (current_scene == nullptr)
		return false;

	CreateCharacters();

	CreateWidgets();



	return true;
}

bool mdSceneManager::update(float dt)
{
	App->gui->draw();


	if (App->input->getKey(SDL_SCANCODE_M) == KEY_REPEAT) //"M" from menu :-)
	{
		changeScene(main_menu); //To test the code
	}


	return true;
}

void mdSceneManager::changeScene(Scene scene_to_load)
{
	App->gui->cleanUI();
	App->entities->cleanUp();

	current_scene = &scene_to_load;

	CreateCharacters();
	CreateWidgets();
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
			App->gui->createBar(curr_widget_info.bar_type, { curr_widget_info.pos.x,curr_widget_info.pos.y }, this);
			break;
		case BUTTON:
			App->gui->createButton(curr_widget_info.button_type, { curr_widget_info.pos.x,curr_widget_info.pos.y }, this);
			break;
		case LABEL:
			App->gui->createLabel(random_label.label_info.text.c_str(), random_label.label_info.color,*(App->fonts->fonts.begin()),{ curr_widget_info.pos.x,curr_widget_info.pos.y }, this);
			break;
		default:
			//It should never go here
			break;
		}
	}

	return true;
}

