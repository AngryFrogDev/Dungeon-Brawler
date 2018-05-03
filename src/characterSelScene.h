#ifndef _CHARACTERSELSCENE_
#define _CHARACTERSELSCENE_

#include "scene.h"

struct currentCharacter
{
	SDL_Rect* portrait = nullptr;
	SDL_Rect* name_tex = nullptr;
	CHAR_TYPE character;
	ITEMS item;
	bool has_selected_character = false;
	bool has_selected_item = false;
	uint skin;
};

class characterSelScene : public scene
{
public:
	characterSelScene(bool active);
	virtual ~characterSelScene();

//	bool awake(const pugi::xml_node & md_config);
	bool start();
	bool update(float dt);

private:
	bool onEvent(Buttons* button);
	void loadSceneUi();
	void createP1CharButtons();
	void createP2CharButtons();//Separated in individual function to be able to re-create them when needed
	void loadSceneTextures();
	//Supplementary functions to one above
	void loadObjectTextures();
	void loadCharMiniaturesTex();
	void assignFocus();
	void checkSceneInput();

	void assignCharacterToPlayer();
	void setRects();
	void setCurrentCharDisplay();
	void closeWindow();
	void popUpP1Window();
	void popUpP2Window();
	void closeP1Window();
	void closeP2Window();

	void startingTransition();

private:
	//Character Selection elements
	Labels* scene_title = nullptr;
	Labels* p1_label = nullptr;
	Labels* p2_label = nullptr;

	Buttons* b_warrior_l = nullptr;
	Buttons* b_mage_l = nullptr;
	Buttons* b_rogue_l = nullptr;
	Buttons* b_paladin_l = nullptr;

	Buttons* b_warrior_r = nullptr;
	Buttons* b_mage_r = nullptr;
	Buttons* b_rogue_r = nullptr;
	Buttons* b_paladin_r = nullptr;

	//Object selection elements
	UiWindow* object_win_p1 = nullptr;
	UiWindow* object_win_p2 = nullptr;
	
	Buttons* p1_select_item1 = nullptr;
	Buttons* p1_select_item2 = nullptr;
	Buttons* p2_select_item1 = nullptr;
	Buttons* p2_select_item2 = nullptr;

	Labels* select_object_p1 = nullptr;
	Labels* select_object_p2 = nullptr;
	Labels* affects_special1_p1 = nullptr;
	Labels* affects_special2_p1 = nullptr;
	Labels* affects_special1_p2 = nullptr;
	Labels* affects_special2_p2 = nullptr;

	Labels* obj1_name_p1 = nullptr;
	Labels* obj2_name_p1 = nullptr;
	Labels* obj1_name_p2 = nullptr;
	Labels* obj2_name_p2 = nullptr;

	Labels* obj1_desc_p1 = nullptr;
	Labels* obj2_desc_p1 = nullptr;
	Labels* obj1_desc_p2 = nullptr;
	Labels* obj2_desc_p2 = nullptr;

	Buttons* obj1_p1 = nullptr;
	Buttons* obj2_p1 = nullptr;
	Buttons* obj1_p2 = nullptr;
	Buttons* obj2_p2 = nullptr;

	//Textures
	SDL_Texture* character_potraits = nullptr;
	SDL_Texture* vs_tex = nullptr;
	SDL_Texture* character_names = nullptr;
	SDL_Texture* items = nullptr;
	SDL_Texture* ready_tex = nullptr;

	//Rects
	SDL_Rect warrior_name_tex;
	SDL_Rect mage_name_tex;
	SDL_Rect rogue_name_tex;
	SDL_Rect paladin_name_tex;

	SDL_Rect warrior_portrait;
	SDL_Rect mage_portrait;
	SDL_Rect rogue_portrait;
	SDL_Rect paladin_portrait;

	SDL_Rect warrior_miniature;
	SDL_Rect mage_miniature;
	SDL_Rect rogue_miniature;
	SDL_Rect paladin_miniature;

	SDL_Rect warrior_item2;
	SDL_Rect warrior_item1;
	SDL_Rect mage_item1;
	SDL_Rect mage_item2;
	SDL_Rect rogue_item1;
	SDL_Rect rogue_item2;
	SDL_Rect paladin_item1;
	SDL_Rect paladin_item2;

	currentCharacter player1, player2;

	Timer transition_timer;
	
};

#endif

