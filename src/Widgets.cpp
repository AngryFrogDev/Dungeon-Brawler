#include "Application.h"
#include "Widgets.h"

Widgets::Widgets(ui_elem_type type, std::pair<int, int> pos, scene* callback) : type(type) {
	this->callback = callback;
	position.first = pos.first;
	position.second = pos.second;
	config = App->loadConfig("config.xml", config_doc);
}

Widgets::~Widgets() {}
