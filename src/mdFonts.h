#ifndef _MDFONTS_
#define _MDFONTS_

#include "Module.h"
#include <list>

#include "SDL\include\SDL_pixels.h"

struct SDL_Texture;
struct _TTF_Font;

class mdFonts : public Module {
public:
	mdFonts();
	virtual ~mdFonts();

	bool awake(const pugi::xml_node&);
	bool cleanUp();

	//Load Font
	_TTF_Font* const Load(const char* path, int size);
	
	//Create a surface from text
	SDL_Texture* print(const char* text, SDL_Color color = { 255, 255, 255, 255 }, _TTF_Font* font = NULL);

	bool calcSize(const char* text, int& width, int& height, _TTF_Font* font = NULL) const;

public:
	std::list<_TTF_Font*> fonts;
	_TTF_Font* large_size = nullptr;
	_TTF_Font* medium_size = nullptr;
	_TTF_Font* small_size = nullptr;

};
#endif

