#include "DebLog.h"
#include "Application.h"
#include "mdRender.h"
#include "mdTextures.h"
#include "mdMap.h"
#include <math.h>
#include "Brofiler/Brofiler.h"
#include "mdAudio.h"
#include "mdEntities.h"

mdMap::mdMap() : Module(), map_loaded(false) {
	name = "map";
}

// Destructor
mdMap::~mdMap()
{}

// Called before render is available
bool mdMap::awake(const pugi::xml_node& md_config) {
	LOG("Loading Map Parser");
	bool ret = true;

	// Provisional, should be loaded from XML
	////data.map_image = App->textures->load("assets/village.png");
	////data.background_image = App->textures->load("assets/village_background.png");
	////map_loaded = true;

	// Load map characteristics, (Provisional, should be done thorugh xml)
	data.camera_x_limit = 3400;
	data.width = 512 * 6;
	
	return ret;
}

void mdMap::draw() {
	if (map_loaded) {
		if (selected_map == 1) {
			//Blit background
			App->render->drawSprite(1, data.background_image, mapx, 0, (const SDL_Rect*)0, 6, false, 0.3);
			App->render->drawSprite(1, data.background_image, mapx2, 0, (const SDL_Rect*)0, 6, false, 0.3);
			//Blit map
			App->render->drawSprite(2, data.map_image, 0, 200, (const SDL_Rect*)0, 5, false);
		}
		else if (selected_map == 2) {
			//Blit background
			App->render->drawSprite(1, data.background_image, mapx, 0, (const SDL_Rect*)0, 6, false, 0.3);
			App->render->drawSprite(1, data.background_image, mapx2, 0, (const SDL_Rect*)0, 6, false, 0.3);
			//Blit map
			App->render->drawSprite(2, data.map_image, 0, 400, (const SDL_Rect*)0, 4, false);
		}
		else if (selected_map == 3) {
			//Blit background
			App->render->drawSprite(1, data.background_image, mapx, 100, (const SDL_Rect*)0, 6, false, 0.3);
			App->render->drawSprite(1, data.background_image, mapx2, 100, (const SDL_Rect*)0, 6, false, 0.3);
			//Blit map
			App->render->drawSprite(2, data.map_image, -400, 750, (const SDL_Rect*)0, 4, false);
		}
	}
}

bool mdMap::update(float dt) {
	if (map_loaded)
	{
		if (change_music)
		{
			if (selected_map == 1)
				App->audio->playMusic(App->audio->loadMusic("SFX/BGM_2.ogg"));
			else if (selected_map == 2)
				App->audio->playMusic(App->audio->loadMusic("SFX/BGM_1.ogg"));
			else if (selected_map == 3)
				App->audio->playMusic(App->audio->loadMusic("SFX/BGM_2.ogg"));
			change_music = false;
		}

		// Provisional: this is soooooo hardcoded
		if (parallax && !App->entities->paused) {
			if (firstfront) {
				mapx -= parallax_speed;
				mapx2 = mapx + data.width;

				if (mapx2 <= 0) {
					mapx = data.width;
					firstfront = false;
				}
			}
			else {
				mapx2 -= parallax_speed;
				mapx = mapx2 + data.width;

				if (mapx <= 0) {
					mapx2 = data.width;
					firstfront = true;
				}
			}
		}

		draw();
	}
	else if (!change_music) {
		App->audio->playMusic(App->audio->loadMusic("SFX/BGM_1.ogg"));
		change_music = true;
	}

	return true;
}

bool mdMap::cleanUp() {
	LOG("Unloading map");

	App->textures->unload(data.map_image);
	App->textures->unload(data.background_image);

	map_loaded = false;
	return true;
}

// Load map general properties
bool mdMap::loadMap(int mapIndex) {
	BROFILER_CATEGORY("Loading Map", 0xFFEE82EE);
	bool ret = true;
	pugi::xml_node map = map_file.child("map");

	loadMapPropierties(map);

	//if (map == NULL) {
	//	LOG("Error parsing map xml file: Cannot find 'map' tag.");
	//	ret = false;
	//}
	//else {
		selected_map = mapIndex;

		if (mapIndex == 1) {		// Load map characteristics, (Provisional, should be done thorugh xml)
			data.map_image = App->textures->load("assets/ship.png");
			data.background_image = App->textures->load("assets/ship_background.png");
			parallax_speed = 5;
			selected_map = 1;
		}
		else if (mapIndex == 2) {
			data.map_image = App->textures->load("assets/water.png");
			data.background_image = App->textures->load("assets/water_background.png");
			selected_map = mapIndex;;
			parallax_speed = 5;
		}
		else if (mapIndex == 3) {
			data.map_image = App->textures->load("assets/train.png");
			data.background_image = App->textures->load("assets/train_background.png");
			selected_map = mapIndex;
			parallax_speed = 10;
		}
	//}
	
	return ret;
}

bool mdMap::unloadMap() {
	BROFILER_CATEGORY("Unloading Map", 0xFF9ACD32);
	bool ret = true;
	App->textures->unload(data.map_image);
	App->textures->unload(data.background_image);
	map_loaded = false;
	
	return ret;
}

bool mdMap::loadMapPropierties(pugi::xml_node& node) {
	pugi::xml_node iterator = node.child("map").child("properties").child("property");
	while (iterator != nullptr) {
		// Load data from XML
	}

	return true;
}