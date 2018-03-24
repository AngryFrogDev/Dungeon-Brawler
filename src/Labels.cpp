#include "Labels.h"



Labels::Labels(std::pair<int, int> pos, Module* callback) : Widgets(ui_elem_type::LABEL, pos, callback) {
}


Labels::~Labels() {
}

void Labels::draw() {
}

void Labels::setText(const char * content, const SDL_Color & color) {
}

void Labels::setArea(uint w, uint h) {
}

void Labels::changeContent(const char * new_content)  {
}
