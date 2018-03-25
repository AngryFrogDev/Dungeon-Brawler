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

	//atlas = App->textures->load();
	atlas_file_name = md_config.child("atlas").attribute("file").as_string("");

	return ret;
}

bool mdGuiManager::preUpdate() {
	bool ret = true;

	Widgets* object = nullptr;

	std::list<Widgets*>::iterator ui_iterator = ui_elements.begin();
	for (ui_iterator; ui_iterator != ui_elements.end() && ret; ui_iterator++) {
		object = *ui_iterator;
		ret = object->preUpdate();
	}

	return ret;
}

bool mdGuiManager::update() {
	bool ret = true;

	Widgets* object = nullptr;

	std::list<Widgets*>::iterator ui_iterator = ui_elements.begin();
	for (ui_iterator; ui_iterator != ui_elements.end() && ret; ui_iterator++) {
		object = *ui_iterator;
		ret = object->update();
	}
	
	debugUi();
	return true;
}

bool mdGuiManager::cleanUp() {
	bool ret = true;
	App->textures->unload(atlas);

	Widgets* object = nullptr;

	std::list<Widgets*>::iterator ui_iterator = ui_elements.begin();
	for (ui_iterator; ui_iterator != ui_elements.end() && ret; ui_iterator++) {
		object = *ui_iterator;
		ret = destroyWidget(object);
	}

	ui_elements.clear();

	return ret;
}

Widgets* mdGuiManager::createButton(button_types type, std::pair<int, int> pos, Module * callback) {
	Widgets* ret = nullptr;

	if (type != 0)
		ret = new Buttons(type, pos, callback), ui_elements.push_back(ret);
		
	return ret;
}

Widgets* mdGuiManager::createLabel(std::pair<int, int> pos, Module * callback) {
	Widgets* ret = nullptr;

	ret = new Labels(pos, callback);
	ui_elements.push_back(ret);

	return ret;
}

Widgets* mdGuiManager::createBar(bar_types type, std::pair<int, int> pos, Module * callback) {
	Widgets* ret = nullptr;

	if (type != 0)
		ret = new Bars(type, pos, callback), ui_elements.push_back(ret);

	return ret;
}

bool mdGuiManager::destroyWidget(Widgets* widget) {
	bool ret = true;

	if (widget == nullptr)
		ret = false;

	Widgets* object = nullptr;

	std::list<Widgets*>::iterator ui_iterator = ui_elements.begin();
	for (ui_iterator; ui_iterator != ui_elements.end() && ret; ui_iterator++) {
		object = *ui_iterator;
		
		if (object == widget)
		{
			ui_elements.remove(object);
			RELEASE(object);
		}
	}

	return ret;
}

SDL_Texture * mdGuiManager::getAtlas() const {
	return atlas;
}

void mdGuiManager::draw() {
	Widgets* object = nullptr;

	std::list<Widgets*>::iterator ui_iterator = ui_elements.begin();
	for (ui_iterator; ui_iterator != ui_elements.end(); ui_iterator++) {
		object = *ui_iterator;
		object->draw();
	}
}

void mdGuiManager::debugUi() {

	//Temporary set to F8
	//if ((App->input.getKey(SDL_SCANCODE_F8) == KEY_DOWN))
	//	debug = !debug;

	if (debug == false)
		return;

	Widgets* object = nullptr;
	uint alpha = 80;

	std::list<Widgets*>::iterator ui_iterator = ui_elements.begin();
	for (ui_iterator; ui_iterator != ui_elements.end(); ui_iterator++)	{

		object = *ui_iterator;
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
