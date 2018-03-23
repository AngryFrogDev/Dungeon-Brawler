#ifndef _WIDGETS_
#define _WIDGETS_

#include "Module.h"

enum ui_elem_type
{
	DEFAULT = 0,
	BUTTON,
	LABEL,
	BAR,
};


class Widgets
{
public:
	Widgets();
	virtual ~Widgets();

	virtual bool preUpdate();
	virtual bool update();
	virtual bool cleanUp();
};
#endif
