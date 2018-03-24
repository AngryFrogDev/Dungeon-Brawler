#include "Application.h"
#include "DebLog.h"
#include "mdGuiManager.h"
#include "mdTextures.h"



mdGuiManager::mdGuiManager() : Module()
{
	name = "gui";
}

mdGuiManager::~mdGuiManager() {}

bool mdGuiManager::awake(pugi::xml_node& conf)
{
	LOG("Loading GUI atlas");
	bool ret = true;

	atlas_file_name = conf.child("atlas").attribute("file").as_string("");

	return ret;
}

bool mdGuiManager::preUpdate()
{
	return true;
}

bool mdGuiManager::update()
{
	return true;
}

bool mdGuiManager::cleanUp()
{
	return true;
}

Widgets * mdGuiManager::createWidget(ui_elem_type type, uint x, uint y, Module * callback)
{
	return nullptr;
}

bool mdGuiManager::destroyWidget(Widgets * widget)
{
	return true;
}

SDL_Texture * mdGuiManager::getAtlas() const
{
	return nullptr;
}

void mdGuiManager::draw()
{
}

void mdGuiManager::debugUi()
{
}
