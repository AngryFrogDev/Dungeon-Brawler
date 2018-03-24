#ifndef _LABELS_
#define _LABELS_

#include "Widgets.h"

#include "SDL/include/SDL.h"

class Labels : public Widgets {
public:
	Labels(std::pair<int, int> pos, Module* callback);
	virtual ~Labels();

	void draw();
	void setText(const char* content, const SDL_Color &color);  //font size is missing until mdFonts is created
	void setArea(uint w, uint h); //consider changing to iPoint when they exist
	void changeContent(const char* new_content);

public:
	SDL_Texture* text_texture;
};

#endif

