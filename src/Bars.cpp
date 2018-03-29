#include "Application.h"
#include "mdRender.h"
#include "Widgets.h"
#include "Bars.h"


Bars::Bars(bar_types type, std::pair<int, int> pos, Module* callback) : Widgets(ui_elem_type::BAR, pos, callback) {
	bar_type = type;
	
	getSection({0, 224, 228, 63}, 20);
}

Bars::~Bars(){
}

bool Bars::preUpdate() {


	return true;
}

void Bars::draw(){
	App->render->blit(App->gui->getAtlas(), position.first, position.second, &current_bar_rect);
}

void Bars::getSection(SDL_Rect rect, uint gauge){
	max_bar_rect.h = rect.h;
	max_bar_rect.w = rect.w;
	max_bar_rect.x = rect.x;
	max_bar_rect.y = rect.y;
	max_gauge = gauge;

	current_bar_rect.h = rect.h;
	current_bar_rect.w = rect.w;
	current_bar_rect.x = rect.x;
	current_bar_rect.y = rect.y;
}

void Bars::updateBarGauge(uint new_gauge) {

	current_gauge = new_gauge;
	current_bar_rect.w -= current_gauge;
}
