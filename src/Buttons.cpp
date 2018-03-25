#include "Application.h"
#include "Buttons.h"
#include "mdAudio.h"
#include "mdRender.h"



Buttons::Buttons(button_types type, std::pair<int, int> pos, Module* callback) : Widgets(ui_elem_type::BUTTON, pos, callback) {
	current_rect = &idle_rect;
	//click_sfx = App->audio->loadSFX(/*Path*/);
}


Buttons::~Buttons() {
}

void Buttons::draw() {
	App->render->blit(App->gui->getAtlas(), position.first, position.second, current_rect);
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

void Buttons::setButtonType(button_types type) {
	button_type = type;
}

void Buttons::setArea(uint w, uint h) {
	world_area.w = w;
	world_area.h = h;
}

void Buttons::changeVisualState(controller_events event) {
	switch (event) {
	case CLICK:
		current_rect = &click_rect; break;
	case RELEASE:
		current_rect = &highl_rect; break;
	}
}
