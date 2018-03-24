#include "Application.h"
#include "DebLog.h"
#include "mdGuiManager.h"
#include "mdTextures.h"
#include "mdInput.h"
#include "mdRender.h"
#include "Buttons.h"
#include "Labels.h"
#include "Bars.h"

mdGuiManager::mdGuiManager() : Module() {
	name = "gui";
}

mdGuiManager::~mdGuiManager() {}

bool mdGuiManager::awake(const pugi::xml_node& md_config) {
	LOG("Loading GUI atlas");
	bool ret = true;

	atlas_file_name = md_config.child("atlas").attribute("file").as_string("");

	return ret;
}

bool mdGuiManager::preUpdate() {
	bool ret = true;

	std::list<Widgets*>::iterator ui_iterator = ui_elements.begin();
	for (ui_iterator; ui_iterator != ui_elements.end() && ret; ui_iterator++) {
		Widgets* object = *ui_iterator;
		ret = object->preUpdate();
	}

	return ret;
}

bool mdGuiManager::update() {
	bool ret = true;

	std::list<Widgets*>::iterator ui_iterator = ui_elements.begin();
	for (ui_iterator; ui_iterator != ui_elements.end() && ret; ui_iterator++) {
		Widgets* object = *ui_iterator;
		ret = object->update();
	}
	
	debugUi();
	return true;
}

bool mdGuiManager::cleanUp() {
	return true;
}

Widgets * mdGuiManager::createWidget(ui_elem_type type, uint x, uint y, Module * callback) {
	Widgets* ret = nullptr;

	//Temporary position variable that is passed to the constructors of the elements
	std::pair<int, int> temp_pos;
	
	switch (type)	{
	case ui_elem_type::BUTTON:
		ret = new Buttons(temp_pos, callback); break;
	case ui_elem_type::LABEL:
		ret = new Labels(temp_pos, callback); break;
	case ui_elem_type::BAR:
		ret = new Bars(temp_pos, callback); break;
	}

	if (ret != nullptr)
		ui_elements.push_back(ret);

	return ret;
}

bool mdGuiManager::destroyWidget(Widgets * widget) {
	return true;
}

SDL_Texture * mdGuiManager::getAtlas() const {
	return atlas;
}

void mdGuiManager::draw() {

	std::list<Widgets*>::iterator ui_iterator = ui_elements.begin();
	for (ui_iterator; ui_iterator != ui_elements.end(); ui_iterator++) {
		Widgets* object = *ui_iterator;
		object->draw();
	}
}

void mdGuiManager::debugUi() {

	//Temporary set to F8
	//if ((App->input.getKey(SDL_SCANCODE_F8) == KEY_DOWN))
	//	debug = !debug;

	if (debug == false)
		return;

	uint alpha = 80;
	std::list<Widgets*>::iterator ui_iterator = ui_elements.begin();
	for (ui_iterator; ui_iterator != ui_elements.end(); ui_iterator++)	{

		Widgets* object = *ui_iterator;
		switch (object->type)
		{
		case BUTTON: // red
			App->render->drawQuad(object->world_area, 255, 0, 0, alpha); break;
		case LABEL: // gren
			App->render->drawQuad(object->world_area, 0, 255, 0, alpha); break;
		case BAR:
			App->render->drawQuad(object->world_area, 0, 0, 255, alpha);
		}
	}
}
