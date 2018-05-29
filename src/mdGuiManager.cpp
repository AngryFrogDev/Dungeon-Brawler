#include "Application.h"
#include "DebLog.h"
#include "mdGuiManager.h"
#include "mdTextures.h"
#include "mdInput.h"
#include "mdRender.h"
#include "mdSceneManager.h"

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

	Widgets* object = nullptr;

	std::list<Widgets*>::reverse_iterator ui_iterator = ui_elements.rbegin();
	for (ui_iterator; ui_iterator != ui_elements.rend(); ++ui_iterator) {
		object = *ui_iterator;
		delete object;
	}
	ui_elements.clear();
	p1_focus_elements.clear();
	p2_focus_elements.clear();

	return ret;
}

Widgets* mdGuiManager::createButton(button_types type, button_size size, int id, std::pair<int, int> pos, scene* callback) {

	Widgets* ret = nullptr;

	if (type != 0) {
		ret = new Buttons(type, size, id, pos, callback);
		ui_elements.push_back(ret);
		if (id == -1) {
			p1_focus_elements.push_back(ret);
			p2_focus_elements.push_back(ret);
		}
		else if (id == 0)
			p1_focus_elements.push_back(ret);
		else if (id == 1)
			p2_focus_elements.push_back(ret);
	}
			
	return ret;
}

Widgets* mdGuiManager::createLabel(const char* content, const SDL_Color& color, _TTF_Font* font_size, std::pair<int, int> pos, scene* callback) {

	Widgets* ret = nullptr;

	ret = new Labels(content, color, font_size, pos, callback);
	ui_elements.push_back(ret);

	return ret;
}

Widgets* mdGuiManager::createBar(bar_types type, std::pair<int, int> pos, bool flipped, int target_player, scene* callback) {

	Widgets* ret = nullptr;

	if (type != 0) {
		ret = new Bars(type, pos, flipped, target_player, callback);
		ui_elements.push_back(ret);
	}

	return ret;
}

Widgets* mdGuiManager::createWindow(window_type type, std::pair<int, int> pos, scene* callback)	{
	Widgets* ret = nullptr;

	ret = new UiWindow(type, pos, callback);
	ui_elements.push_back(ret);

	return ret;
}

bool mdGuiManager::destroyWidget(Widgets* widget) {

	bool ret = true;

	if (widget == nullptr)
		ret = false;
	else
	{
		ui_elements.remove(widget);//Deleting from the main ui list

		if (widget->type == BUTTON)//Deleting from the specific focus list
		{
			if (widget->focus_id == -1) {
				p1_focus_elements.remove(widget);
				p2_focus_elements.remove(widget);
			}
			else if (widget->focus_id == 0)
				p1_focus_elements.remove(widget);
			else if (widget->focus_id == 1)
				p2_focus_elements.remove(widget);
		}
	}
	
	return ret;
}

void mdGuiManager::manageFocus() {

	if (!App->entities->players[0]->focus && !App->entities->players[1]->focus)//Check if, at least, one focus has been assigned
		return;
	
	assignP1Focus();
	assignP2Focus();

}

void mdGuiManager::assignP1Focus()	{

	Widgets* object = nullptr;
	
	if (App->input->isButtonState(BUTTON_DPAD_UP, KEY_DOWN, 0) || App->input->isButtonState(BUTTON_DPAD_LEFT, KEY_DOWN, 0)) {

		std::list<Widgets*>::iterator temp_iterator = p1_focus_elements.begin();
		for (temp_iterator; temp_iterator != p1_focus_elements.end(); temp_iterator++)
		{
			object = *temp_iterator;
			if (object->stop_focus)
				continue;
			if (object == App->entities->players[0]->focus)
			{
				if (temp_iterator != p1_focus_elements.begin())
				{
					temp_iterator--;
					App->entities->players[0]->focus = *temp_iterator;
					break;
				}
				else
					App->entities->players[0]->focus = *p1_focus_elements.rbegin(); break;
			}
		}
	}


	if (App->input->isButtonState(BUTTON_DPAD_DOWN, KEY_DOWN, 0) || App->input->isButtonState(BUTTON_DPAD_RIGHT, KEY_DOWN, 0)) {

		std::list<Widgets*>::reverse_iterator temp_iterator = p1_focus_elements.rbegin();
		for (temp_iterator; temp_iterator != p1_focus_elements.rend(); temp_iterator++)
		{
			object = *temp_iterator;
			if (object->stop_focus)
				continue;
			if (object == App->entities->players[0]->focus)
			{
				if (temp_iterator != p1_focus_elements.rbegin())
				{
					temp_iterator--;
					App->entities->players[0]->focus = *temp_iterator;
					break;
				}
				else
					App->entities->players[0]->focus = *p1_focus_elements.begin(); break;
			}
		}
	}

}

void mdGuiManager::assignP2Focus()	{
	
	Widgets* object = nullptr;
	
	if (App->input->isButtonState(BUTTON_DPAD_UP, KEY_DOWN, 1) || App->input->isButtonState(BUTTON_DPAD_LEFT, KEY_DOWN, 1)) {

		std::list<Widgets*>::iterator temp_iterator = p2_focus_elements.begin();
		for (temp_iterator; temp_iterator != p2_focus_elements.end(); temp_iterator++)
		{
			object = *temp_iterator;
			if (object->stop_focus)
				continue;
			if (object == App->entities->players[1]->focus)
			{
				if (temp_iterator != p2_focus_elements.begin())
				{
					temp_iterator--;
					App->entities->players[1]->focus = *temp_iterator;
					break;
				}
				else
					App->entities->players[1]->focus = *p2_focus_elements.rbegin(); break;
			}
		}
	}


	if (App->input->isButtonState(BUTTON_DPAD_DOWN, KEY_DOWN, 1) || App->input->isButtonState(BUTTON_DPAD_RIGHT, KEY_DOWN, 1)) {

		std::list<Widgets*>::reverse_iterator temp_iterator = p2_focus_elements.rbegin();
		for (temp_iterator; temp_iterator != p2_focus_elements.rend(); temp_iterator++)
		{
			object = *temp_iterator;
			if (object->stop_focus)
				continue;
			if (object == App->entities->players[1]->focus)
			{
				if (temp_iterator != p2_focus_elements.rbegin())
				{
					temp_iterator--;
					App->entities->players[1]->focus = *temp_iterator;
					break;
				}
				else
					App->entities->players[1]->focus = *p2_focus_elements.begin(); break;
			}
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
	for (ui_iterator; ui_iterator != ui_elements.end(); ui_iterator++) {

		object = *ui_iterator;
		{
			switch (object->type)
			{
			case BUTTON: // red
				App->render->drawQuad(1, object->world_area, 255, 0, 0, alpha); break;
			case LABEL: // gren
				App->render->drawQuad(1, object->world_area, 0, 255, 0, alpha); break;
			case BAR: //blue
				App->render->drawQuad(1, object->world_area, 0, 0, 255, alpha); break;
			}
		}
	}
}
