#include "mdGuiManager.h"



mdGuiManager::mdGuiManager()
{
}


mdGuiManager::~mdGuiManager()
{
}

bool mdGuiManager::awake(pugi::xml_node &)
{
	return false;
}

bool mdGuiManager::preUpdate()
{
	return false;
}

bool mdGuiManager::update()
{
	return false;
}

bool mdGuiManager::cleanUp()
{
	return false;
}

Widgets * mdGuiManager::createWidget(ui_elem_type type, uint x, uint y, Module * callback)
{
	return nullptr;
}

bool mdGuiManager::destroyWidget(Widgets * widget)
{
	return false;
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
