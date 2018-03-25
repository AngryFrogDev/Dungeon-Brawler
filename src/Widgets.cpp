#include "Widgets.h"
#include "mdGuiManager.h"


Widgets::Widgets(ui_elem_type type, std::pair<int, int> pos, Module* callback) : type(type) {
	this->callback = callback;
	position.first = pos.first;
	position.second = pos.second;
	world_area.x = pos.first;
	world_area.y = pos.second;
}

Widgets::~Widgets() {}
