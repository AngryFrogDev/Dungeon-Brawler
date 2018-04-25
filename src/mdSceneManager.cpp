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
#include "mdParticleSystem.h"
#include "startScene.h"
#include "mainScene.h"
#include "combatScene.h"
#include "characterSelScene.h"


mdSceneManager::mdSceneManager()	{
	//PROVISIONAL: Hardcoded
	screen = { 0, 0, 1920, 1080 };
	name = "SceneManager";

	start_scene = new startScene(true);
	main_scene = new mainScene(false);
	char_sel_scene = new characterSelScene(false);
	combat_scene = new combatScene(false);

	scene_list.push_back(start_scene);
	scene_list.push_back(main_scene);
	scene_list.push_back(char_sel_scene);
	scene_list.push_back(combat_scene);
}


mdSceneManager::~mdSceneManager(){}

bool mdSceneManager::awake(const pugi::xml_node & md_config)	{
	
	return true;
}

bool mdSceneManager::start()	{
	bool ret = false;

	std::list<scene*>::iterator scene_it = scene_list.begin();
	scene* object = nullptr;
	for (scene_it; scene_it != scene_list.end(); scene_it++)
	{
		object = *scene_it;
		if (object->scene_active)
			ret = object->start();
	}
		
	SDL_SetRenderDrawBlendMode(App->render->renderer, SDL_BLENDMODE_BLEND);
	return ret;
}

bool mdSceneManager::update(float dt)	{

	bool ret = false;

	std::list<scene*>::iterator scene_it = scene_list.begin();
	scene* object = nullptr;
	for (scene_it; scene_it != scene_list.end(); scene_it++)
	{
		object = *scene_it;
		if (object->scene_active)
			ret = object->update(dt);
	}

	//---------------------------------------------------------------
	startSwitch();
	
	return ret;
}

bool mdSceneManager::changeScene(scene* scene_in, scene* scene_out)	{
	bool ret = false;
	if (current_step == fade_step::NONE)
	{
		current_step = fade_step::FADE_TO_BLACK;
		switch_timer.start();
		to_enable = scene_in;
		to_disable = scene_out;
		ret = true;
	}

	return ret;
}

bool mdSceneManager::createCharacters()
{	
/*	for (auto it = current_scene->characters.begin(); it != current_scene->characters.end(); it++) {
		CharacterInfo curr_character_info = *it;
		App->entities->createPlayer(curr_character_info.player, curr_character_info.x_pos, curr_character_info.type, curr_character_info.flipped, 1);
	}
	*/
	App->entities->assignControls();
	App->render->camera.x = (App->render->resolution.first - App->render->camera.w) / 2; //PROVISIONAL: This should be done from the scene manager

	return true;
}

void mdSceneManager::loadSceneCharacters()	{
	//1VS1
/*	player1.x_pos = 400;
	player1.type = WARRIOR;
	player1.player = 0;
	player1.flipped = false;
	obj_sel.characters.push_back(player1);

	player2.x_pos = 1500;
	player2.type = WARRIOR;
	player2.player = 1;
	player2.flipped = true;
	obj_sel.characters.push_back(player2);

	one_vs_one.characters.push_back(player2);

	*/
}

void mdSceneManager::startSwitch()	{
	float normalized = MIN(1.0f, switch_timer.readSec() / fadetime);
	static iPoint temp_cam;

	switch (current_step)
	{
	case fade_step::NONE:
		break;
	case fade_step::FADE_TO_BLACK:
		if (switch_timer.readSec() >= fadetime)
		{
			to_disable->scene_active = false;

			App->collision->cleanUp();
			App->entities->cleanUp();
			App->gui->cleanUp();
			App->render->cleanBlitQueue();
			App->projectiles->cleanUp();
			App->map->map_loaded = false;

		//	createCharacters();

			to_enable->scene_active = true;
			to_enable->start();
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

}




