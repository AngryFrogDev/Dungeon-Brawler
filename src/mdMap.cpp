#include "DebLog.h"
#include "Application.h"
#include "mdRender.h"
#include "mdTextures.h"
#include "mdMap.h"
#include <math.h>
#include "Brofiler/Brofiler.h"

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

	return ret;
}

void mdMap::draw() {
	if (map_loaded) {
		//Blit background
		App->render->blit(1, data.background_image, 0, 0, (const SDL_Rect*)0, 4,false,0.3);

		//Blit map
		App->render->blit(2, data.map_image, 0, 0, (const SDL_Rect*)0,4,false,0.2);
	}
}

bool mdMap::update(float dt) {
	if (map_loaded)
	draw();

	if (App->input->getKey(SDL_SCANCODE_1) == KEY_DOWN) {
		unloadMap();
		loadMap(1);
	}
	else if (App->input->getKey(SDL_SCANCODE_2) == KEY_DOWN) {
		unloadMap();
		loadMap(2);
	}
	else if (App->input->getKey(SDL_SCANCODE_3) == KEY_DOWN) {
		unloadMap();
		loadMap(3);
	}
	else if (App->input->getKey(SDL_SCANCODE_4) == KEY_DOWN)
		unloadMap();

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
		if (mapIndex == 1) {		// Load map characteristics, (Provisional, should be done thorugh xml)
			data.map_image = App->textures->load("assets/village.png");
			data.background_image = App->textures->load("assets/village_background.png");
		}
		else if (mapIndex == 2) {
			data.map_image = App->textures->load("assets/village2.png");
			data.background_image = App->textures->load("assets/village_background2.png");
		}
		else if (mapIndex == 3) {
			data.map_image = App->textures->load("assets/village3.png");
			data.background_image = App->textures->load("assets/village_background3.png");
		}
	//}
	map_loaded = true;

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