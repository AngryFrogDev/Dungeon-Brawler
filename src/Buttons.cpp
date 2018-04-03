#include "Application.h"
#include "Buttons.h"
#include "mdAudio.h"
#include "mdRender.h"
#include "mdInput.h"



Buttons::Buttons(button_types type, std::pair<int, int> pos, Module* callback) : Widgets(ui_elem_type::BUTTON, pos, callback) {
	
	button_type = type;
	current_rect = &idle_rect;
	//click_sfx = App->audio->loadSFX(/*Path*/);

	loadButtonsFromAtlas();
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
			changeVisualState(CLICK);//Should call the onEvent function for each scene
		hovering = false;
	}
	

	return ret;
}

void Buttons::draw() {
	App->render->blit(3, App->gui->getAtlas(), position.first, position.second, current_rect);
}

void Buttons::getSection(SDL_Rect idle_sec, SDL_Rect high_sec, SDL_Rect clicked_sec, SDL_Rect disabled_sec) {
	idle_rect.x = idle_sec.x;
	idle_rect.y = idle_sec.y;
	idle_rect.w = idle_sec.w;
	idle_rect.h = idle_sec.h;

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
		current_rect = &idle_rect; break;
	}
}

void Buttons::loadButtonsFromAtlas() {

	pugi::xml_node button = ui_config.child("button"); //Still not implemented because functions to load config files are missing

	switch (button_type)
	{
	case NO_BUTTON:
		break;
	case NEW_GAME:
		getSection({ 0,0,288,63 }, { 0,150,288,63 }, { 0, 75, 288, 63 }, { 0,0,0,0 }); //Read from XML
		break;
	case SETTINGS:
		getSection({ 0,0,288,63 }, { 0,150,288,63 }, { 0, 75, 288, 63 }, { 0,0,0,0 }); //Read from XML
		break;
	case CREDITS:
		break;
	case EXIT:
		break;
	case TWO_PLAYERS: 
		break;
	case FOUR_PLAYERS:
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
