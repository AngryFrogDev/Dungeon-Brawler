#ifndef _MDGUIMANAGER_
#define _MDGUIMANAGER_

#include "Module.h"
#include "Widgets.h"
#include "ProjDefs.h"
#include <list>

#include "SDL/include/SDL.h"

struct Buttons;
struct Labels;
struct Bars;
struct _TTF_Font;

//Some events may be missing
enum controller_events {
	NO_EVENT = 0,
	STILL,
	FOCUSED,
	CLICK,
};

class mdGuiManager : public Module {
public:
	mdGuiManager();
	virtual ~mdGuiManager();

	bool awake(const pugi::xml_node&);
	bool preUpdate();
	bool update(float dt);
	bool postUpdate();
	bool cleanUp();
	bool cleanUI();
	
	Widgets* createButton(button_types type, std::pair<int, int> pos, Module* callback = nullptr);
	Widgets* createLabel(const char* content, const SDL_Color& color, _TTF_Font* font_size, std::pair<int, int> pos, Module* callback = nullptr);
	Widgets* createBar(bar_types type, std::pair<int, int> pos, Module* callback = nullptr);
	bool destroyWidget(Widgets* widget);

	void manageFocus();
	SDL_Texture* getAtlas() const;
	void draw();
	void debugUi();

public:
	std::list<Widgets*> ui_elements;
	std::list<Widgets*> temp_list; //Temporary list to delete elements
	std::list<Widgets*> focus_elements; //Button list to store elements to be affected by focus
	SDL_Texture* atlas;
	std::string atlas_file_name;
	Widgets* focus = nullptr;

private:
	bool debug = false;


};

#endif

