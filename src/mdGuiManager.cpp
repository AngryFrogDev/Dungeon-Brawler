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
	atlas = App->textures->load(atlas_file_name.data());

	return ret;
}

bool mdGuiManager::preUpdate() {

	bool ret = true;

	manageFocus();

	Widgets* object = nullptr;
	std::list<Widgets*>::iterator ui_iterator = ui_elements.begin();//Now we iterate the temporary list 
	for (ui_iterator; ui_iterator != ui_elements.end() && ret; ui_iterator++) {
		object = *ui_iterator;
		ret = object->preUpdate();
	}
		
	return ret;
}

bool mdGuiManager::update(float dt) {

	bool ret = true;

	Widgets* object = nullptr;

	std::list<Widgets*>::iterator ui_iterator = ui_elements.begin();
	for (ui_iterator; ui_iterator != ui_elements.end() && ret; ui_iterator++) {
		object = *ui_iterator;
		ret = object->update(dt);
	}
	
	debugUi();
	return true;
}

bool mdGuiManager::postUpdate() {
	bool ret = true;

	//Filling the temporary list
	Widgets* object = nullptr;
	std::list<Widgets*>::iterator ui_iterator = ui_elements.begin();
	for (ui_iterator; ui_iterator != ui_elements.end() && *ui_iterator; ui_iterator++) {
		object = *ui_iterator;
		temp_list.push_back(object);
	}

	//Now we iterate the temporary list 
	std::list<Widgets*>::iterator temp_iterator = temp_list.begin();
	for (temp_iterator; temp_iterator != temp_list.end() && *temp_iterator; temp_iterator++) {
		object = *temp_iterator;
		if (object->to_delete)
			ret = destroyWidget(object);//But we delete the elements from the original one
	}

	//Emptying temporary list
	temp_list.clear();
	
		
	return ret;
}

bool mdGuiManager::cleanUp() {

	bool ret = true;
	App->textures->unload(atlas);

	Widgets* object = nullptr;

	std::list<Widgets*>::reverse_iterator ui_iterator = ui_elements.rbegin();
	for (ui_iterator; ui_iterator != ui_elements.rend(); ++ui_iterator) {
		object = *ui_iterator;
		RELEASE(object);
	}

	ui_elements.clear();

	return ret;
}

bool mdGuiManager::cleanUI()
{
	Widgets* object = nullptr;

	std::list<Widgets*>::reverse_iterator ui_iterator = ui_elements.rbegin();
	for (ui_iterator; ui_iterator != ui_elements.rend(); ++ui_iterator) {
		object = *ui_iterator;
		RELEASE(object);
	}

	ui_elements.clear();
	return true;
}


Widgets* mdGuiManager::createButton(button_types type, std::pair<int, int> pos, Module * callback) {

	Widgets* ret = nullptr;

	if (type != 0) {
		ret = new Buttons(type, pos, callback);
		ui_elements.push_back(ret);
		focus_elements.push_back(ret);
	}
			
	return ret;
}

Widgets* mdGuiManager::createLabel(const char* content, const SDL_Color& color, _TTF_Font* font_size, std::pair<int, int> pos, Module * callback) {

	Widgets* ret = nullptr;

	ret = new Labels(content, color, font_size, pos, callback);
	ui_elements.push_back(ret);

	return ret;
}

Widgets* mdGuiManager::createBar(bar_types type, std::pair<int, int> pos, Module * callback) {

	Widgets* ret = nullptr;

	if (type != 0) {
		ret = new Bars(type, pos, callback);
		ui_elements.push_back(ret);
	}

	return ret;
}

bool mdGuiManager::destroyWidget(Widgets* widget) {

	bool ret = true;

	if (widget == nullptr)
		ret = false;
	else
	{
		ui_elements.remove(widget);//Deleting from the main ui list

		if (widget->type == BUTTON)
			focus_elements.remove(widget);//Deleting from the specific focus list
	}
	
	return ret;
}

void mdGuiManager::manageFocus() {

	Widgets* object = nullptr;
	if (focus)//Check if focus has been assigned -> Current scene is not ingame scene
	{
		//Temporary done in keyboard
		if (App->input->getKey(SDL_SCANCODE_UP) == KEY_DOWN || App->input->getKey(SDL_SCANCODE_LEFT) == KEY_DOWN)
		{
			if (focus != *focus_elements.begin())//Case player wants to go up when not at the first button
			{
				std::list<Widgets*>::iterator temp_elem = focus_elements.begin();
				for (temp_elem; temp_elem != focus_elements.end(); temp_elem++)
				{
					object = *temp_elem;
					if (object == focus)//Iterate until find the currently focused element
					{
						temp_elem--;//Move the iterator to the previous ui element
						focus = *temp_elem;//Assign its value to the focused element
						break;
					}
				}
			}
			else
				focus = *focus_elements.rbegin();
		}

		if (App->input->getKey(SDL_SCANCODE_DOWN) == KEY_DOWN || App->input->getKey(SDL_SCANCODE_RIGHT) == KEY_DOWN)
		{
			if (focus != *focus_elements.rbegin())//Case player wants to go down when not at the last button
			{
				std::list<Widgets*>::iterator temp_elem = focus_elements.begin();
				for (temp_elem; temp_elem != focus_elements.end(); temp_elem++)
				{
					object = *temp_elem;
					if (object == focus)
					{
						temp_elem++;
						focus = *temp_elem;
						break;
					}
				}
			}
			else
				focus = *focus_elements.begin();
		}
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
	if ((App->input->getKey(SDL_SCANCODE_F8) == KEY_DOWN))
		debug = !debug;

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
		case BAR: //blue
			App->render->drawQuad(object->world_area, 0, 0, 255, alpha); break;
		}
	}
}
