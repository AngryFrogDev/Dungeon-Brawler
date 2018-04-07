#include "Application.h"
#include "mdRender.h"
#include "Widgets.h"
#include "Bars.h"
#include "DebLog.h"


Bars::Bars(bar_types type, std::pair<int, int> pos, bool _flipped, Module* callback) : Widgets(ui_elem_type::BAR, pos, callback) {
	
	bar_type = type;
	flipped = _flipped;
	data = config.child("gui").child("bar_section");
	loadGuiFromAtlas();
}

Bars::~Bars(){
}

bool Bars::preUpdate() {

	bool ret = true;

	world_area = { position.first, position.second, bar_rect.w, bar_rect.h };

	return ret;
}

void Bars::draw(){
	App->render->blit(3, App->gui->getAtlas(), position.first, position.second, &bar_rect, 2, flipped);
	App->render->blit(3, App->gui->getAtlas(), position.first + 4, position.second + 4, &gauge_rect, 2, flipped);
}

void Bars::getSection(SDL_Rect rect, SDL_Rect gauge){
	bar_rect.h = rect.h;
	bar_rect.w = rect.w;
	bar_rect.x = rect.x;
	bar_rect.y = rect.y;

	gauge_rect.h = gauge.h;
	gauge_rect.w = gauge.w;
	gauge_rect.x = gauge.x;
	gauge_rect.y = gauge.y;

	gauge_y_position = position.second;
}

void Bars::updateBarGauge(uint new_gauge) {

	if (bar_type == SWAP_BAR) {
		gauge_rect.h -= new_gauge;
		gauge_y_position += new_gauge;
	}
	else
		gauge_rect.w -= new_gauge;
}

void Bars::loadGuiFromAtlas()	{
	pugi::xml_node hp = data.child("hp");
	pugi::xml_node super = data.child("super");
	pugi::xml_node border;
	pugi::xml_node gauge;
	
	switch (bar_type)
	{
	case HEALTH_BAR:
		border = hp.child("border");
		gauge = hp.child("gauge");
		getSection({ border.attribute("x").as_int(), border.attribute("y").as_int(), border.attribute("w").as_int(), border.attribute("h").as_int() },
		{ gauge.attribute("x").as_int(),gauge.attribute("y").as_int(), gauge.attribute("w").as_int(), gauge.attribute("h").as_int() });
		break;
	case SUPER_BAR:
		border = super.child("border");
		gauge = super.child("gauge");
		getSection({ border.attribute("x").as_int(), border.attribute("y").as_int(), border.attribute("w").as_int(), border.attribute("h").as_int() },
		{ gauge.attribute("x").as_int(),gauge.attribute("y").as_int(), gauge.attribute("w").as_int(), gauge.attribute("h").as_int() });
		break;
	case SWAP_BAR:
		break;
	case NO_BAR:
		LOG("Non valid Bar type");
		break;
	}
}
