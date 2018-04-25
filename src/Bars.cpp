#include "Application.h"
#include "mdRender.h"
#include "Widgets.h"
#include "Bars.h"
#include "DebLog.h"
#include "mdEntities.h"
#include "Player.h"
#include "Character.h"
#include "mdGuiManager.h"


Bars::Bars(bar_types type, std::pair<int, int> pos, bool _flipped, int target, scene* callback) : Widgets(ui_elem_type::BAR, pos, callback) {
	
	bar_type = type;
	flipped = _flipped;
	data = config.child("gui").child("bar_section");
	loadGuiFromAtlas();
	target_player = target;

	if (bar_type == SUPER_BAR)
		current_gauge_rect.w = 0;
}

Bars::~Bars(){
}

bool Bars::preUpdate() {

	bool ret = true;

	world_area = { position.first, position.second, bar_rect.w, bar_rect.h };

	return ret;
}

void Bars::draw()	{

	updateBarGauge();

	App->render->drawSprite(4, App->gui->getAtlas(), position.first, position.second, &bar_rect, 2, flipped, 1.0f, 0, 0, 0, false);
	App->render->drawSprite(5, App->gui->getAtlas(), position.first + relative_pos.x + aux_bar_pos, position.second + relative_pos.y, &current_gauge_rect, 2, flipped, 1.0f, 0, 0, 0, false);
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

	current_gauge_rect.h = gauge.h;
	current_gauge_rect.w = gauge.w;
	current_gauge_rect.x = gauge.x;
	current_gauge_rect.y = gauge.y;

}

void Bars::updateBarGauge() {
		
	if (bar_type == HEALTH_BAR || bar_type == SUPER_BAR) {
		calculateBarGauge();
	}	

}

void Bars::loadGuiFromAtlas()	{
	pugi::xml_node hp = data.child("hp");
	pugi::xml_node super = data.child("super");
	pugi::xml_node border;
	pugi::xml_node gauge;
	
	switch (bar_type)
	{
	case HEALTH_BAR:
		relative_pos = { 4,4 };
		border = hp.child("border");
		gauge = hp.child("gauge");
		getSection({ border.attribute("x").as_int(), border.attribute("y").as_int(), border.attribute("w").as_int(), border.attribute("h").as_int() },
		{ gauge.attribute("x").as_int(),gauge.attribute("y").as_int(), gauge.attribute("w").as_int(), gauge.attribute("h").as_int() });
		break;
	case SUPER_BAR:
		relative_pos = { 5, 0 };
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

void Bars::calculateBarGauge() {

	if (bar_type == HEALTH_BAR) {
	//	current_gauge = App->entities->players[target_player]->getCurrCharacter()->getCurrentLife();
	//	max_gauge = App->entities->players[target_player]->getCurrCharacter()->getMaxLife();

		if (flipped && last_gauge != current_gauge) {
			current_gauge_rect.w = (gauge_rect.w*current_gauge) / max_gauge;
			current_gauge_rect.x = gauge_rect.x + (gauge_rect.w - current_gauge_rect.w);
			last_gauge = current_gauge;

		}
		else if (!flipped && last_gauge != current_gauge) {
			current_gauge_rect.w = (gauge_rect.w*current_gauge) / max_gauge;
			current_gauge_rect.x = gauge_rect.x + (gauge_rect.w - current_gauge_rect.w);
			aux_bar_pos = 2 * (gauge_rect.w - current_gauge_rect.w);
			last_gauge = current_gauge;
		}
	}	
	else if (bar_type == SUPER_BAR) {
	//	 current_gauge = App->entities->players[target_player]->getCurrCharacter()->getCurrentSuperGauge();
	//	 max_gauge = App->entities->players[target_player]->getCurrCharacter()->getMaxSuperGauge();

		 if (flipped && last_gauge != current_gauge) {
			 current_gauge_rect.w = (gauge_rect.w*current_gauge) / max_gauge;
			 current_gauge_rect.x = gauge_rect.x + (gauge_rect.w - current_gauge_rect.w);
			 last_gauge = current_gauge;

		 }
		 else if (!flipped && last_gauge != current_gauge) {
			 current_gauge_rect.w = (gauge_rect.w*current_gauge) / max_gauge;
			 current_gauge_rect.x = gauge_rect.x + (gauge_rect.w - current_gauge_rect.w);
			 aux_bar_pos = 2 * (gauge_rect.w - current_gauge_rect.w);
			 last_gauge = current_gauge;
		 }
	}
	

}

