#ifndef _MDGUIMANAGER_
#define _MDGUIMANAGER_

#include "Module.h"
#include "scene.h"
#include "Widgets.h"
#include "ProjDefs.h"
#include <list>

#include "SDL/include/SDL.h"


struct _TTF_Font;

class mdGuiManager : public Module {
public:
	mdGuiManager();
	virtual ~mdGuiManager();

	bool awake(const pugi::xml_node&);
	bool preUpdate();
	bool update(float dt);
	bool postUpdate();
	bool cleanUp();
	
	Widgets* createButton(button_types type, button_size size, int id, std::pair<int, int> pos, scene* callback = nullptr);
	Widgets* createLabel(const char* content, const SDL_Color& color, _TTF_Font* font_size, std::pair<int, int> pos, scene* callback = nullptr);
	Widgets* createBar(bar_types type, std::pair<int, int> pos, bool flipped, int target_player, scene* callback = nullptr);
	Widgets* createWindow(window_type type, std::pair<int, int> pos, scene* callback = nullptr);
	bool destroyWidget(Widgets* widget);

	void manageFocus();
	void assignP1Focus();
	void assignP2Focus();
	SDL_Texture* getAtlas() const;
	void draw();
	void debugUi();
	
public:
	std::list<Widgets*> ui_elements;
	std::list<Widgets*> temp_list; //Temporary list to delete elements
	std::list<Widgets*> p1_focus_elements; //Button list to store elements to be affected by player 1 focus
	std::list<Widgets*> p2_focus_elements; //Button list to store elements to be affected by player 2 focus

	SDL_Texture* atlas;
	std::string atlas_file_name;

private:
	bool debug = false;

};

#endif

