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

	
	atlas = App->textures->load("atlas.png");
	atlas_file_name = md_config.child("atlas").attribute("file").as_string("");

	return ret;
}

bool mdGuiManager::preUpdate() {

	bool ret = true;

	manageFocus();
	
	Widgets* object = nullptr;
	std::list<Widgets*>::iterator ui_iterator = ui_elements.begin();
	for (ui_iterator; ui_iterator != ui_elements.end() && ret; ui_iterator++) {
		object = *ui_iterator;
		ret = object->preUpdate();
	}

	return ret;
}

bool mdGuiManager::update(float dt) {

	bool ret = true;

	Widgets* object = nullptr;

	
	//Temporary testing-> TO BE REMOVED BEFORE MERGING
	if (App->input->getKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		createButton(NEW_GAME, { 0,0 }, this);
		createButton(NEW_GAME, { 0,150 }, this);
		temp = true;
	}

	//Temporary
	if (temp)
		focused_elem = *ui_elements.begin(), temp = false;

	std::list<Widgets*>::iterator ui_iterator = ui_elements.begin();
	for (ui_iterator; ui_iterator != ui_elements.end() && ret; ui_iterator++) {
		object = *ui_iterator;
		ret = object->update(dt);
	}
	
	//Temporary testing-> TO BE REMOVED BEFORE MERGING
	draw();
	
	debugUi();
	return true;
}

bool mdGuiManager::cleanUp() {

	bool ret = true;
	App->textures->unload(atlas);

	Widgets* object = nullptr;

	std::list<Widgets*>::reverse_iterator ui_iterator = ui_elements.rbegin();
	for (ui_iterator; ui_iterator != ui_elements.rend(); ++ui_iterator) {
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
	else
		RELEASE(widget);
	
	return ret;
}

void mdGuiManager::manageFocus() {

	//Temporary done in keyboard
	if (App->input->getKey(SDL_SCANCODE_UP) == KEY_DOWN)
	{
		if (focused_elem != *ui_elements.begin())//Case player wants to go up when not at the first button
		{
			std::list<Widgets*>::iterator temp_elem = ui_elements.begin();
			for (temp_elem; temp_elem != ui_elements.end(); temp_elem++)
			{
				if (*temp_elem == focused_elem)//Iterate until find the currently focused element
				{
					temp_elem--;//Move the iterator to the previous ui element
					focused_elem = *temp_elem;//Assign its value to the focused element
					break;
				}
			}
		}
		else 
			focused_elem = *ui_elements.rbegin();
	}

	if (App->input->getKey(SDL_SCANCODE_DOWN) == KEY_DOWN)
	{
		if (focused_elem != *ui_elements.rbegin())//Case player wants to go down when not at the last button
		{
			std::list<Widgets*>::iterator temp_elem = ui_elements.begin();
			for (temp_elem; temp_elem != ui_elements.end(); temp_elem++)
			{
				if (*temp_elem == focused_elem)
				{
					temp_elem++;
					focused_elem = *temp_elem;
					break;
				}
			}
		}
		else
			focused_elem = *ui_elements.begin();
	}
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
