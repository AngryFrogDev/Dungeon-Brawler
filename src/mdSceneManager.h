#ifndef __MDSCENEMANAGER__
#define __MDSCENEMANAGER__
#include "Module.h"
#include <list>
#include "DebLog.h"
#include "SDL\include\SDL.h"
#include "p2Point.h"
#include "Character.h"
#include "mdInput.h"
#include "PerfTimer.h"
#include "scene.h"


struct CharacterInfo {
	int player;
	int x_pos;
	CHAR_TYPE type;
	bool flipped;
};


class mdSceneManager : public Module{
public:
	mdSceneManager();
	~mdSceneManager();

	bool awake(const pugi::xml_node& md_config) override;
	bool start() override;
	bool update(float dt) override;

	bool changeScene(scene* scene_in, scene* scene_out);
	
private:
	bool createCharacters();
	void loadSceneCharacters();
	void startSwitch();
	
public:
	scene* start_scene = nullptr;
	scene* main_scene = nullptr;
	scene* char_sel_scene = nullptr;
	scene* settings_scene = nullptr;
	scene* combat_scene = nullptr;

private:
	enum fade_step
	{
		NONE,
		FADE_TO_BLACK,
		FADE_FROM_BLACK
	} current_step = fade_step::NONE;

	std::list<scene*> scene_list;

	SDL_Rect screen;
	float fadetime = 1.0f;
	Timer switch_timer;
	
	scene* to_enable = nullptr;
	scene* to_disable = nullptr;

};

#endif