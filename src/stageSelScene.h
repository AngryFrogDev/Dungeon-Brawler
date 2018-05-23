#ifndef _STAGESELSCENE_
#define _STAGESELSCENE_

#include "scene.h"

class stageSelScene : public scene
{
public:
	stageSelScene(bool active);
	virtual ~stageSelScene();

	bool start();
	bool update(float dt);

private:
	bool onEvent(Buttons* button);
	void loadSceneUi();
	void loadSceneTextures();
	void assignFocus();
	void startingTransition();

private:
	int selected_map = 0;

	SDL_Texture* current_stage = nullptr;
	SDL_Texture* stage_1 = nullptr;
	SDL_Texture* stage_2 = nullptr;
	SDL_Texture* stage_3 = nullptr;

	Labels* scene_title_label = nullptr;
	Labels* current_stage_label = nullptr;
	Labels* stage_1_label = nullptr;
	Labels* stage_2_label = nullptr;
	Labels* stage_3_label = nullptr;

	Buttons* stage_1_button = nullptr;
	Buttons* stage_2_button = nullptr;
	Buttons* stage_3_button = nullptr;

	Mix_Chunk* scene_sfx = nullptr;

	Timer transition_timer;
};
#endif
