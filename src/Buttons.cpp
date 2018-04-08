#include "Application.h"
#include "Buttons.h"
#include "mdAudio.h"
#include "mdRender.h"
#include "mdInput.h"
#include "mdSceneManager.h"
#include "DebLog.h"



Buttons::Buttons(button_types type, std::pair<int, int> pos, Module* callback) : Widgets(ui_elem_type::BUTTON, pos, callback) {
	
	button_type = type;
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

	if (!enabled)
	{
		current_rect = &disabled_rect;
		return ret;
	}
	
	world_area = { position.first, position.second, current_rect->w, current_rect->h };

	if (App->gui->focus == this)
		hovering = true;
	else
		changeVisualState(STILL);
					
	if (hovering)
	{
		changeVisualState(FOCUSED);
		if (App->input->getKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
		{
			changeVisualState(CLICK); 
			ret = App->scene_manager->onEvent(this);
		}
		hovering = false;
	}
	

	return ret;
}

void Buttons::draw() {
	App->render->blit(3, App->gui->getAtlas(), position.first, position.second, current_rect, 3);
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

	if (disabled_sec.h != 0 && disabled_sec.w != 0)
	{
		disabled_rect.h = disabled_sec.h;
		disabled_rect.w = disabled_sec.w;
		disabled_rect.x = disabled_sec.x;
		disabled_rect.y = disabled_sec.y;

		enabled = false;
	}
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
	pugi::xml_node still = data.child("still");
	pugi::xml_node focused = data.child("focused");
	pugi::xml_node clicked = data.child("clicked");
		
	switch (button_type)
	{
	case NO_BUTTON:
		LOG("Non valid Button type");
		break;
	case ONE_V_ONE:
		getSection({ still.attribute("x").as_int(), still.attribute("y").as_int(), still.attribute("w").as_int(), still.attribute("h").as_int() },
		{ focused.attribute("x").as_int(), focused.attribute("y").as_int(), focused.attribute("w").as_int(), focused.attribute("h").as_int() },
		{ clicked.attribute("x").as_int(), clicked.attribute("y").as_int(), clicked.attribute("w").as_int(), clicked.attribute("h").as_int() }, { 0,0,0,0 });
		break;
	case TWO_V_TWO:
		getSection({ still.attribute("x").as_int(), still.attribute("y").as_int(), still.attribute("w").as_int(), still.attribute("h").as_int() },
		{ focused.attribute("x").as_int(), focused.attribute("y").as_int(), focused.attribute("w").as_int(), focused.attribute("h").as_int() },
		{ clicked.attribute("x").as_int(), clicked.attribute("y").as_int(), clicked.attribute("w").as_int(), clicked.attribute("h").as_int() }, { 0,0,0,0 });
		break;
	case SETTINGS:
		getSection({ still.attribute("x").as_int(), still.attribute("y").as_int(), still.attribute("w").as_int(), still.attribute("h").as_int() },
		{ focused.attribute("x").as_int(), focused.attribute("y").as_int(), focused.attribute("w").as_int(), focused.attribute("h").as_int() },
		{ clicked.attribute("x").as_int(), clicked.attribute("y").as_int(), clicked.attribute("w").as_int(), clicked.attribute("h").as_int() }, { 0,0,0,0 });
		break;
	case CREDITS:
		getSection({ still.attribute("x").as_int(), still.attribute("y").as_int(), still.attribute("w").as_int(), still.attribute("h").as_int() },
		{ focused.attribute("x").as_int(), focused.attribute("y").as_int(), focused.attribute("w").as_int(), focused.attribute("h").as_int() },
		{ clicked.attribute("x").as_int(), clicked.attribute("y").as_int(), clicked.attribute("w").as_int(), clicked.attribute("h").as_int() }, { 0,0,0,0 });
		break;
	case GAME_EXIT:
		getSection({ still.attribute("x").as_int(), still.attribute("y").as_int(), still.attribute("w").as_int(), still.attribute("h").as_int() },
		{ focused.attribute("x").as_int(), focused.attribute("y").as_int(), focused.attribute("w").as_int(), focused.attribute("h").as_int() },
		{ clicked.attribute("x").as_int(), clicked.attribute("y").as_int(), clicked.attribute("w").as_int(), clicked.attribute("h").as_int() }, { 0,0,0,0 });
		break;
	case MUSIC_VOL_UP:
		break;
	case MUSIC_VOL_DOWN:
		break;
	case SFX_VOL_UP:
		break;
	case SFX_VOL_DOWN:
		break;
	case BACK:
		break;
	default:
		break;
	}
}
