#include "scene.h"
#include "Application.h"
#include "Player.h"



scene::scene(scene_type _type)
{
	scene_config = App->loadConfig("scene_config.xml", scene_config_doc);
	type = _type;
}


scene::~scene()	{}
