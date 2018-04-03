#include "Application.h"
#include "mdRender.h"
#include "Widgets.h"
#include "Bars.h"


Bars::Bars(bar_types type, std::pair<int, int> pos, Module* callback) : Widgets(ui_elem_type::BAR, pos, callback) {
	
	bar_type = type;
	
	getSection({ 0, 75, 228, 63 }, { 0, 298, 228, 63 });
}

Bars::~Bars(){
}

bool Bars::preUpdate() {

	bool ret = true;

	world_area = { position.first, position.second, current_bar_rect.w, current_bar_rect.h };

	return ret;
}

void Bars::draw(){
	App->render->blit(3, App->gui->getAtlas(), position.first, position.second, &current_bar_rect);
	App->render->blit(3, App->gui->getAtlas(), position.first, gauge_y_position, &current_gauge_rect);

}

void Bars::getSection(SDL_Rect rect, SDL_Rect gauge){
	max_bar_rect.h = rect.h;
	max_bar_rect.w = rect.w;
	max_bar_rect.x = rect.x;
	max_bar_rect.y = rect.y;

	current_bar_rect.h = rect.h;
	current_bar_rect.w = rect.w;
	current_bar_rect.x = rect.x;
	current_bar_rect.y = rect.y;

	max_gauge_rect.h = gauge.h;
	max_gauge_rect.w = gauge.w;
	max_gauge_rect.x = gauge.x;
	max_gauge_rect.y = gauge.y;

	current_gauge_rect.h = gauge.h;
	current_gauge_rect.w = gauge.w;
	current_gauge_rect.x = gauge.x;
	current_gauge_rect.y = gauge.y;

	gauge_y_position = position.second;
}

void Bars::updateBarGauge(uint new_gauge) {

	if (bar_type == SWAP_BAR) {
		current_gauge_rect.h -= new_gauge;
		gauge_y_position += new_gauge;
	}
	else
		current_gauge_rect.w -= new_gauge;
}
