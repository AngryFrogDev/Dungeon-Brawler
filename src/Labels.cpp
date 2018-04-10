#include "Application.h"
#include "Labels.h"
#include "mdTextures.h"
#include "mdRender.h"
#include "mdFonts.h"
#include "mdTextures.h"



Labels::Labels(const char* content, const SDL_Color& _color, _TTF_Font* _font_size, std::pair<int, int> pos, Module* callback) : Widgets(ui_elem_type::LABEL, pos, callback) {
	setText(content, _color, _font_size);
	color = _color;
	font_size = _font_size;
}


Labels::~Labels() {
	if (text_texture != nullptr)
		App->textures->unload(text_texture);
}

void Labels::draw() {
	App->render->drawSprite(4, text_texture, position.first, position.second);
}

void Labels::setArea(std::pair<int, int> area) {
	world_area.w = area.first;
	world_area.h = area.second;
}

void Labels::setText(const char * content, const SDL_Color & _color, _TTF_Font* _font_size) {
	text_texture = App->fonts->print(content, _color, _font_size);
	std::pair<int, int> area;
	SDL_QueryTexture(text_texture, NULL, NULL, &area.first, &area.second);
	setArea(area);
}

void Labels::changeContent(const char * new_content)  {
	if (text_texture != nullptr)
		App->textures->unload(text_texture);

	text_texture = App->fonts->print(new_content, color, font_size);
	std::pair<int, int> area;
	SDL_QueryTexture(text_texture, NULL, NULL, &area.first, &area.second);
	setArea(area);
}
