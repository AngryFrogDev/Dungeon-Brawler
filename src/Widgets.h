#ifndef _WIDGETS_
#define _WIDGETS_

#include "Module.h"
#include "ProjDefs.h"

#include "SDL\include\SDL.h"

enum ui_elem_type
{
	NO_ELEM = 0,
	BUTTON,
	LABEL,
	BAR,
};


class Widgets
{
public:
	Widgets();
	virtual ~Widgets();

	virtual bool preUpdate() { return true; };
	virtual bool update() { return true; };
	virtual bool cleanUp() { return true; };

	virtual void draw() { return; };

public:
	ui_elem_type type;
	bool being_clicked = false;
	SDL_Rect world_area;

protected:
	Module* callback = nullptr;
};
#endif
