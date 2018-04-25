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
};

class characterSelScene : public scene
{
public:
	characterSelScene(bool active);
	~characterSelScene();

//	bool awake(const pugi::xml_node & md_config);
	bool start();
	bool update(float dt);

private:
	bool onEvent(Buttons* button);
	void loadSceneUi();
	void loadSceneTextures();
	void assignFocus();
	void checkSceneInput();
	void assignCharacterToPlayer();
	void setRects();
	void setCurrentCharDisplay();

private:
	Labels* scene_title = nullptr;
	Buttons* b_warrior = nullptr;
	Buttons* b_mage = nullptr;
	Buttons* b_rogue = nullptr;
	Buttons* b_paladin = nullptr;
	
	SDL_Texture* character_potraits = nullptr;
	SDL_Texture* vs_tex = nullptr;
	SDL_Texture* character_names = nullptr;

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

	currentCharacter player1, player2;
	
};

#endif

