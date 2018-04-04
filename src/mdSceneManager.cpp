#include "mdSceneManager.h"
#include "Application.h"
#include "mdEntities.h"
#include "mdGuiManager.h"


mdSceneManager::mdSceneManager()
{
}


mdSceneManager::~mdSceneManager()
{
}

bool mdSceneManager::awake(const pugi::xml_node & md_config)
{
	//HARDCODE
	Scene TestScene();

	player1.x_pos = 100;
	player1.type = CHAR_TYPE::WARRIOR;
	player1.player = 0;
	player1.flipped = false;
	Test_Scene.characters.push_back(player1);

	player2.x_pos = 1000;
	player2.type = CHAR_TYPE::WARRIOR;
	player2.player = 1;
	player2.flipped = true;
	Test_Scene.characters.push_back(player2);


	current_scene = &Test_Scene;



	return true;
}

bool mdSceneManager::start()
{
	if (current_scene == nullptr)
		return false;

	CreateCharacters();

	test_bar = (Bars*)App->gui->createBar(bar_types::HEALTH_BAR, { 0,0 }, this);
	button = (Buttons*)App->gui->createButton(NEW_GAME, { 100,100 }, this);

	App->entities->assignControls();




	//Very dangerous hardcode to set the partners: 

	//players[0]->getCurrCharacter()->partner = players[1];
	//players[1]->getCurrCharacter()->partner = players[0];
	//players[2]->getCurrCharacter()->partner = players[3];
	//players[3]->getCurrCharacter()->partner = players[2];

	return true;
}

bool mdSceneManager::update(float dt)
{
	App->gui->draw();
	return true;
}

bool mdSceneManager::CreateCharacters()
{	
	if (current_scene->characters.empty())
		return false;

	for (auto it = current_scene->characters.begin(); it != current_scene->characters.end(); it++) {
		CharacterInfo curr_character_info = *it;
		App->entities->createPlayer(curr_character_info.player, curr_character_info.x_pos, curr_character_info.type, curr_character_info.flipped, 1);
	}

	return true;
}

