#include "mdSceneManager.h"
#include "Application.h"
#include "mdEntities.h"


mdSceneManager::mdSceneManager()
{
}


mdSceneManager::~mdSceneManager()
{
}

bool mdSceneManager::awake(const pugi::xml_node & md_config)
{
	std::list<CharacterInfo> characters_in_scene_1;
	CharacterInfo player1;

	player1.postion = { 0,100 };

	//App->entities->createPlayer(1,1000, CHAR_TYPE::WARRIOR, true, 1 ); //play with the lane (last argument) for 2v2
	//createPlayer(2, 1200, CHAR_TYPE::WARRIOR, true, 1);
	//createPlayer(3, 300, CHAR_TYPE::WARRIOR, true, 2);

	//Very dangerous hardcode to set the partners: 

	//players[0]->getCurrCharacter()->partner = players[1];
	//players[1]->getCurrCharacter()->partner = players[0];
	//players[2]->getCurrCharacter()->partner = players[3];
	//players[3]->getCurrCharacter()->partner = players[2];
	

	//Hardcode


	return true;
}

bool mdSceneManager::start()
{

	App->entities->createPlayer(3, 100, CHAR_TYPE::WARRIOR, false, 2);
	return true;
}
