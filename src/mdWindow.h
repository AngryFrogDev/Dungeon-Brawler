#ifndef __MDWINDOW__
#define __MDWINDOW__

#include "Module.h"
#include "ProjDefs.h"

struct SDL_Window;
struct SDL_Surface;

class mdWindow : public Module {
public:

	mdWindow();
	virtual ~mdWindow() {}

	bool awake(const pugi::xml_node& md_config);
	bool cleanUp();

	void setWindowTitle(const char* new_title);

	void getWindowSize(uint& width, uint& height) const;
	void setWindowSize(const uint& width, const uint& height);
public:
	SDL_Window*		window = nullptr;
	SDL_Surface*	window_surface = nullptr;

private:
	std::string		title;

	uint			scale; //requiered by the renderer
};

#endif //__MDWINDOW__