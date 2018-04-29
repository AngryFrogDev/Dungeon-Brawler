#include "Application.h"
#include "Buttons.h"
#include "mdAudio.h"
#include "mdRender.h"
#include "mdInput.h"
#include "mdSceneManager.h"
#include "DebLog.h"
#include "mdGuiManager.h"



Buttons::Buttons(button_types type, button_size _size, int id, std::pair<int, int> pos, scene* callback) : Widgets(ui_elem_type::BUTTON, pos, callback) {
	
	button_type = type;
	focus_id = id;
	size = _size;
	//click_sfx = App->audio->loadSFX(/*Path*/);
	data = config.child("gui").child("button_section");

	loadGuiFromAtlas();
	current_rect = &still_rect;
}

Buttons::~Buttons() {
}

bool Buttons::preUpdate()
{
	bool ret = true;

	if (current_rect == &click_rect && size == CHARACTER_SELECTION)
		return ret;
		
	world_area = { position.first, position.second, current_rect->w, current_rect->h };

	if (App->entities->players[0]->focus == this || App->entities->players[1]->focus == this)
		hovering = true;
	else
		changeVisualState(STILL);
					
	if (hovering)
	{
		changeVisualState(FOCUSED);
		
		if (size == CHARACTER_SELECTION)
			ret = callback->onEvent(this);

		if (App->entities->players[0]->getInput(HEAVY_ATTACK, KEY_DOWN) && focus_id == 0 || App->entities->players[1]->getInput(HEAVY_ATTACK, KEY_DOWN) && focus_id == 1)
		{
			changeVisualState(CLICK);
			being_clicked = true;
			ret = callback->onEvent(this);
		}
		hovering = false;
	}
	

	return ret;
}

void Buttons::draw() {
	App->render->drawSprite(6, App->gui->getAtlas(), position.first, position.second, current_rect, 3, false, 1.0f, 0.0, 0, 0, false);
}

void Buttons::getSection(SDL_Rect idle_sec, SDL_Rect high_sec, SDL_Rect clicked_sec, SDL_Rect disabled_sec) {
	still_rect.x = idle_sec.x;
	still_rect.y = idle_sec.y;
	still_rect.w = idle_sec.w;
	still_rect.h = idle_sec.h;

	highl_rect.x = high_sec.x;
	highl_rect.y = high_sec.y;
	highl_rect.w = high_sec.w;
	highl_rect.h = high_sec.h;

	click_rect.x = clicked_sec.x;
	click_rect.y = clicked_sec.y;
	click_rect.w = clicked_sec.w;
	click_rect.h = clicked_sec.h;

}


void Buttons::changeVisualState(controller_events event) {
	switch (event) {
	case CLICK:
		current_rect = &click_rect; break;
	case FOCUSED:
		current_rect = &highl_rect; break;
	case STILL:
		current_rect = &still_rect; break;
	}
}

void Buttons::loadGuiFromAtlas() {
	//The moment we have buttons visually different, there should be another node to differentiate them 
	pugi::xml_node still;
	pugi::xml_node focused;
	pugi::xml_node clicked;
		
		switch (size)
	{
	case NO_SIZE:
		LOG("No valid button size");
		break;
	case SMALL:
		still = data.child("small").child("still");
		focused = data.child("small").child("focused");
		clicked = data.child("small").child("clicked");
		getSection({ still.attribute("x").as_int(), still.attribute("y").as_int(), still.attribute("w").as_int(), still.attribute("h").as_int() },
			{ focused.attribute("x").as_int(), focused.attribute("y").as_int(), focused.attribute("w").as_int(), focused.attribute("h").as_int() },
			{ clicked.attribute("x").as_int(), clicked.attribute("y").as_int(), clicked.attribute("w").as_int(), clicked.attribute("h").as_int() }, { 0,0,0,0 });
		break;
	case MEDIUM:
		still = data.child("medium").child("still");
		focused = data.child("medium").child("focused");
		clicked = data.child("medium").child("clicked");
		getSection({ still.attribute("x").as_int(), still.attribute("y").as_int(), still.attribute("w").as_int(), still.attribute("h").as_int() },
			{ focused.attribute("x").as_int(), focused.attribute("y").as_int(), focused.attribute("w").as_int(), focused.attribute("h").as_int() },
			{ clicked.attribute("x").as_int(), clicked.attribute("y").as_int(), clicked.attribute("w").as_int(), clicked.attribute("h").as_int() }, { 0,0,0,0 });
		break;
	case LARGE:
		still = data.child("big").child("still");
		focused = data.child("big").child("focused");
		clicked = data.child("big").child("clicked");
		getSection({ still.attribute("x").as_int(), still.attribute("y").as_int(), still.attribute("w").as_int(), still.attribute("h").as_int() },
			{ focused.attribute("x").as_int(), focused.attribute("y").as_int(), focused.attribute("w").as_int(), focused.attribute("h").as_int() },
			{ clicked.attribute("x").as_int(), clicked.attribute("y").as_int(), clicked.attribute("w").as_int(), clicked.attribute("h").as_int() }, { 0,0,0,0 });
		break;
	case CHARACTER_SELECTION:
		still = data.child("char_sel").child("still");
		focused = data.child("char_sel").child("focused");
		clicked = data.child("char_sel").child("clicked");
		getSection({ still.attribute("x").as_int(), still.attribute("y").as_int(), still.attribute("w").as_int(), still.attribute("h").as_int() },
		{ focused.attribute("x").as_int(), focused.attribute("y").as_int(), focused.attribute("w").as_int(), focused.attribute("h").as_int() },
		{ clicked.attribute("x").as_int(), clicked.attribute("y").as_int(), clicked.attribute("w").as_int(), clicked.attribute("h").as_int() }, { 0,0,0,0 });
		break;
	default:
		break;
	}
}
