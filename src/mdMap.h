#ifndef __MDMAP_H__
#define __MDMAP_H__

#include "pugixml\pugixml.hpp"
#include <list>
#include "Module.h"
#include "mdRender.h"
#include "p2Point.h"
#include "mdInput.h"

struct MapData
{
	int				width;
	int				height;
	SDL_Texture*	background_image = nullptr;
	SDL_Texture*	map_image = nullptr;
	SDL_Texture*	foreground_image = nullptr;
	iPoint*			player_starting_value;
	float			parallax_speed;
	int				camera_y_limit;
	int				camera_x_limit;
};

// ----------------------------------------------------
class mdMap : public Module
{
public:

	mdMap();

	// Destructor
	virtual ~mdMap();

	bool awake(const pugi::xml_node& md_config);
	bool update(float dt);
	void draw();
	bool cleanUp();
	int map_id = 0;

private:

	bool loadMap(int mapIndex);
	bool loadMapPropierties(pugi::xml_node& node);
	bool unloadMap();

public:

	MapData data;
	bool map_loaded;

private:

	pugi::xml_document	map_file;

};

#endif // __MDMAP_H__