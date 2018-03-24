#include "DebLog.h"
#include "Application.h"
#include "mdRender.h"
#include "mdTextures.h"
#include "mdMap.h"
#include <math.h>

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
	data.map_image = App->textures->load("village.png");
	data.background_image = App->textures->load("village_background.png");
	map_loaded = true;

	return ret;
}

void mdMap::draw() {
	if (map_loaded) {
		//Blit background
		App->render->blit(data.background_image, 0, 0, (const SDL_Rect*)0, 3);

		//Blit map
		App->render->blit(data.map_image, 0, 0, (const SDL_Rect*)0, 3);
	}
}

bool mdMap::update(float dt) {
	if (map_loaded)
	draw();

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
bool mdMap::loadMap()
{
	bool ret = true;
	pugi::xml_node map = map_file.child("map");

	if (map == NULL) {
		LOG("Error parsing map xml file: Cannot find 'map' tag.");
		ret = false;
	}
	else {
		// Load map characteristics
	}

	return ret;
}

bool mdMap::loadBackground(pugi::xml_node& node) {
	// Load background image

	return true;
}

bool mdMap::loadForeground(pugi::xml_node& node) {
	// Load background image

	return true;
}

bool mdMap::loadMapPropierties(pugi::xml_node& node) {
	pugi::xml_node iterator = node.child("map").child("properties").child("property");
	while (iterator != nullptr) {
		// Load data from XML
	}

	return true;
}