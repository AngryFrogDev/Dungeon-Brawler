#include "Application.h"
#include "mdRender.h"
#include "Bars.h"


Bars::Bars(bar_types type, std::pair<int, int> pos, Module* callback) : Widgets(ui_elem_type::BAR, pos, callback) {
}

Bars::~Bars(){
}

void Bars::draw(){
	App->render->blit(App->gui->getAtlas(), position.first, position.second, current_bar_rect);
}

void Bars::setType(bar_types type){
	bar_type = type;
}

void Bars::getSection(SDL_Rect rect){
	
}

