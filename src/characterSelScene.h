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
	bool has_selected_skin = false;
	uint skin = 4; //Initialized with this value because 0-3 are skins
};

class characterSelScene : public scene
{
public:
	characterSelScene(bool active);
	virtual ~characterSelScene();

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
	void loadSkinTextures();
	void loadCharMiniaturesTex();
	void assignFocus();
	void checkSceneInput();
	void assignSkins(Buttons* button);

	void assignCharacterToPlayer();
	void setRects();
	void setCurrentCharDisplay();
	void popUpP1Window();
	void popUpP2Window();
	void closeP1Window();
	void closeP2Window();
	void popUpP1SkinWindow();
	void popUpP2SkinWindow();
	void closeP1SkinWindow();
	void closeP2SkinWindow();
	void resetSceneValues();

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

	//Skin selection
	SDL_Rect warrior_skin_1_preview;
	SDL_Rect warrior_skin_2_preview;
	SDL_Rect warrior_skin_3_preview;
	SDL_Rect warrior_skin_4_preview;

	SDL_Rect mage_skin_1_preview;
	SDL_Rect mage_skin_2_preview;
	SDL_Rect mage_skin_3_preview;
	SDL_Rect mage_skin_4_preview;

	SDL_Rect rogue_skin_1_preview;
	SDL_Rect rogue_skin_2_preview;
	SDL_Rect rogue_skin_3_preview;
	SDL_Rect rogue_skin_4_preview;

	SDL_Rect paladin_skin_1_preview;
	SDL_Rect paladin_skin_2_preview;
	SDL_Rect paladin_skin_3_preview;
	SDL_Rect paladin_skin_4_preview;

	SDL_Rect* p1_currentchar_skin_1_preview = nullptr;
	SDL_Rect* p1_currentchar_skin_2_preview = nullptr;
	SDL_Rect* p1_currentchar_skin_3_preview = nullptr;
	SDL_Rect* p1_currentchar_skin_4_preview = nullptr;

	SDL_Rect* p2_currentchar_skin_1_preview = nullptr;
	SDL_Rect* p2_currentchar_skin_2_preview = nullptr;
	SDL_Rect* p2_currentchar_skin_3_preview = nullptr;
	SDL_Rect* p2_currentchar_skin_4_preview = nullptr;

	UiWindow* p1_skin_sel_window = nullptr;
	UiWindow* p2_skin_sel_window = nullptr;

	Labels* p1_skin_general_label = nullptr;
	Labels* p2_skin_general_label = nullptr;

	Buttons* p1_skin_1_button = nullptr;
	Buttons* p1_skin_2_button = nullptr;
	Buttons* p1_skin_3_button = nullptr;
	Buttons* p1_skin_4_button = nullptr;
	Buttons* p2_skin_1_button = nullptr;
	Buttons* p2_skin_2_button = nullptr;
	Buttons* p2_skin_3_button = nullptr;
	Buttons* p2_skin_4_button = nullptr;

	//SFX
	Mix_Chunk* scene_sfx1 = nullptr;
	Mix_Chunk* scene_sfx2 = nullptr;
	uint random_sfx = 0;

	currentCharacter player1, player2;

	Timer transition_timer;
	
};

#endif

