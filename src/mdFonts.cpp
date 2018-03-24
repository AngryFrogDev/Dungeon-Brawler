#include "Application.h"
#include "DebLog.h"
#include "mdFonts.h"
#include "mdTextures.h"

#include "SDL\include\SDL.h"
#include "SDL_ttf\include\SDL_ttf.h"
#pragma comment( lib, "SDL_ttf/libx86/SDL2_ttf.lib" )

mdFonts::mdFonts() : Module() {
	name = "fonts";
}

mdFonts::~mdFonts() {}

bool mdFonts::awake(const pugi::xml_node& md_config) {
	LOG("Init True Type Font library");
	bool ret = true;

	if (TTF_Init() == -1)
	{
		LOG("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
		ret = false;
	}

	else
	{
		//Load font
	}

	return ret;
}

bool mdFonts::cleanUp() {
	LOG("Freeing True Type fonts and library");
	std::list<_TTF_Font*>::iterator item;

	for (item = fonts.begin(); item != fonts.end(); item++)
	{
		_TTF_Font* object = *item;
		TTF_CloseFont(object);
	}

	fonts.clear();
	TTF_Quit();
	return true;
}

_TTF_Font * const mdFonts::Load(const char * path, int size) {
	TTF_Font* font = TTF_OpenFont(path, size);

	if (font == NULL)
	{
		LOG("Could not load TTF font with path: %s. TTF_OpenFont: %s", path, TTF_GetError());
	}
	else
	{
		LOG("Successfully loaded font %s size %d", path, size);
		fonts.push_back(font);
	}

	return font;
}

SDL_Texture * mdFonts::print(const char * text, SDL_Color color, _TTF_Font * font) {
	SDL_Texture* ret = NULL;
	SDL_Surface* surface = TTF_RenderText_Blended((font) ? font : medium_size, text, color);

	if (surface == NULL)
	{
		LOG("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}
	else
	{
		//ret = App->textures->loadSurface(surface);
		SDL_FreeSurface(surface);
	}
	
	return ret;
}

bool mdFonts::calcSize(const char * text, int & width, int & height, _TTF_Font * font) const {
	bool ret = false;

	if (TTF_SizeText((font) ? font : medium_size, text, &width, &height) != 0)
		LOG("Unable to calc size of text surface! SDL_ttf Error: %s\n", TTF_GetError());
	else
		ret = true;

	return ret;
}
