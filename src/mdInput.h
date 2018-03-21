#ifndef __MDINPUT__
#define __MDINPUT__

#include "Module.h"
#include "ProjDefs.h"
#include <deque>
#include <list>

#include "SDL/include/SDL.h"
#include "SDL/include/SDL_gamecontroller.h"

enum KEY_STATE {
	KEY_NULL = 0,
	KEY_IDLE,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};

enum CONTROLLER_BUTTON {
	BUTTON_INVALID = 0,
	BUTTON_A,
	BUTTON_B,
	BUTTON_X,
	BUTTON_Y,
	BUTTON_BACK,
	BUTTON_GUIDE,
	BUTTON_START,
	BUTTON_LEFTSTICK,
	BUTTON_RIGHTSTICK,
	BUTTON_LEFTSHOULDER,
	BUTTON_RIGHTSHOULDER,
	BUTTON_DPAD_UP,
	BUTTON_DPAD_DOWN,
	BUTTON_DPAD_LEFT,
	BUTTON_DPAD_RIGHT,
	AXIS_LEFTX_NEGATIVE,
	AXIS_LEFTX_POSITIVE,
	AXIS_LEFTY_NEGATIVE,
	AXIS_LEFTY_POSITIVE,
	AXIS_RIGHTX_NEGATIVE,
	AXIS_RIGHTX_POSITIVE,
	AXIS_RIGHTY_NEGATIVE,
	AXIS_RIGHTY_POSITIVE,
	AXIS_TRIGGERLEFT,
	AXIS_TRIGGERRIGHT,
	BUTTON_MAX
};

class Controller {
public:
	Controller(SDL_GameController* controller, SDL_Haptic* controller_haptic);
	virtual ~Controller();

	const std::deque<CONTROLLER_BUTTON>& getInputs() const;
	//Will add an input to the buffer, if no timestamp provided the current time will be used
	void addInput(CONTROLLER_BUTTON input, uint timestamp = NULL);
	//Pops oldest input
	void popInput();
	//Prunes any inputs that are older than milliseconds, if 0 it will clear the queue
	void pruneInput(uint milliseconds = 0);
	bool shakeController(float intensity, uint milliseconds) const;

	uint getControllerID() const;
	
public:
	KEY_STATE buttons[SDL_CONTROLLER_BUTTON_MAX];
	float axes[SDL_CONTROLLER_AXIS_MAX];

private:
	uint id;
	SDL_GameController* controller;
	SDL_Haptic* controller_haptic;

	std::deque<CONTROLLER_BUTTON> input_buffer;
	std::deque<uint> input_times;
};

class mdInput : public Module {
public:
	mdInput();
	~mdInput();

	bool awake(const pugi::xml_node& md_config);
	bool preUpdate();
	bool cleanUp();

	KEY_STATE getKey(int key); 

private:
	void handleAxes(const SDL_Event& event);

private:
	std::list<Controller*> controllers;
	KEY_STATE* keyboard = nullptr;
	uint controller_buffer_timeout = 0;
	float axis_tolerance = 0.2f;
};

#endif