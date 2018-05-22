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

	bool loadMap(int mapIndex);
private:

	bool loadMapPropierties(pugi::xml_node& node);
	bool unloadMap();
	int selected_map = 0;

public:

	MapData data;
	bool map_loaded;
	bool parallax = true;

private:

	pugi::xml_document	map_file;
	int mapx = 0;
	int mapx2 = 0;
	bool firstfront = true;
	int iterator = 0;
	int parallax_speed = 0;
	bool change_music = true;
};

#endif // __MDMAP_H__