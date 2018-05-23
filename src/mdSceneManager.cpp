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
#include "settingsScene.h"
#include "stageSelScene.h"


mdSceneManager::mdSceneManager()	{
	//PROVISIONAL: Hardcoded
	screen = { 0, 0, 1920, 1080 };
	name = "SceneManager";

	start_scene = new startScene(true);
	main_scene = new mainScene(false);
	settings_scene = new settingsScene(false);
	char_sel_scene = new characterSelScene(false);
	combat_scene = new combatScene(false);
	stage_sel_scene = new stageSelScene(false);

	scene_list.push_back(start_scene);
	scene_list.push_back(main_scene);
	scene_list.push_back(settings_scene);
	scene_list.push_back(char_sel_scene);
	scene_list.push_back(combat_scene);
	scene_list.push_back(stage_sel_scene);
}


mdSceneManager::~mdSceneManager(){}

bool mdSceneManager::awake(const pugi::xml_node & md_config)	{
	
	return true;
}

bool mdSceneManager::start()	{
	bool ret = false;

	for (int i = 0; i < 2; i++)
	{
		App->entities->createPlayer(i);
	}


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

			//SPECIAL CASES
			if (to_disable->name == "Combat Scene")
			{
				if (to_enable->name == "Stage Selection Scene")
					App->entities->show = false, App->entities->setStopped(false);

				else if (!to_disable->rematching)
				{
					App->collision->cleanUp();
					for (int i = 0; i < 2; i++)
					{
						App->entities->players[i]->removeCharacters();
					}
				}
			}
			//Resetting focus
			App->entities->players[0]->focus = nullptr;
			App->entities->players[1]->focus = nullptr;

			App->gui->cleanUp();
			App->render->cleanBlitQueue();
			App->projectiles->cleanUp();
			App->map->map_loaded = false;

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
		SDL_RenderFillRect(App->render->renderer, &screen); // Fade to black should happen over all the screen
	}

}




