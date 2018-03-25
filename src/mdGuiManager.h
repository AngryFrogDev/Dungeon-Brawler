#ifndef _MDGUIMANAGER_
#define _MDGUIMANAGER_

#include "Module.h"
#include "Widgets.h"
#include "ProjDefs.h"
#include <list>

#include "SDL/include/SDL.h"

//Some events may be missing
enum controller_events {
	NO_EVENT = 0,
	CLICK,
	RELEASE,
};

class mdGuiManager : public Module {
public:
	mdGuiManager();
	virtual ~mdGuiManager();

	bool awake(const pugi::xml_node&);
	bool preUpdate();
	bool update();
	bool cleanUp();

	Widgets* createButton(button_types type, std::pair<int, int> pos, Module* callback = nullptr);
	Widgets* createLabel(std::pair<int, int> pos, Module* callback = nullptr);
	Widgets* createBar(bar_types type, std::pair<int, int> pos, Module* callback = nullptr);
	bool destroyWidget(Widgets* widget);

	SDL_Texture* getAtlas() const;
	void draw();
	void debugUi();

public:
	std::list<Widgets*> ui_elements;
	SDL_Texture* atlas;
	std::string atlas_file_name;

private:
	bool debug = false;

};

#endif

