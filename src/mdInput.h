#ifndef __MDINPUT__
#define __MDINPUT__

#include "Module.h"
#include "ProjDefs.h"
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
	BUTTON_INVALID = -1,
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
	AXIS_LEFTY_NEGATIVE = 11,
	AXIS_LEFTY_POSITIVE,
	AXIS_LEFTX_NEGATIVE,
	AXIS_LEFTX_POSITIVE,
	AXIS_RIGHTX_NEGATIVE = 19,
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

	bool isPressed(CONTROLLER_BUTTON button, KEY_STATE state = KEY_REPEAT) const;
	const std::list<CONTROLLER_BUTTON> getInputs() const;
	//Will add an input to the buffer, if no timestamp provided the current time will be used
	void addInput(CONTROLLER_BUTTON input, uint timestamp = NULL);
	//Pops oldest input
	void popInput();
	//Prunes any inputs that are older than milliseconds, if 0 it will clear the queue
	void pruneInput(uint milliseconds = 0);
	bool shakeController(float intensity, uint milliseconds) const;

	uint getControllerID() const;
	
public:
	KEY_STATE buttons[CONTROLLER_BUTTON::BUTTON_MAX];
	float axes[SDL_CONTROLLER_AXIS_MAX];
private:
	uint id;
	SDL_GameController* controller;
	SDL_Haptic* controller_haptic;
	struct input_record {
		CONTROLLER_BUTTON input;
		uint timestamp;
	};
	std::list<input_record> input_buffer;
	//std::list<int> input_times;
};

class mdInput : public Module {
public:
	mdInput();
	~mdInput();

	bool awake(const pugi::xml_node& md_config);
	bool preUpdate();
	bool cleanUp();


	// Returns the state of the pressed key
	KEY_STATE getKey(SDL_Scancode key) const;
	KEY_STATE getControllerButton(int id, SDL_GameControllerButton button);
	//If ID is -1 it will return an OR of all controllers
	bool isButtonState(SDL_GameControllerButton button, KEY_STATE state, int id = -1);

	//Returns controllers that pressed button, if invalid button will return first controller.
	std::list<Controller*> getController(SDL_GameControllerButton button = SDL_CONTROLLER_BUTTON_INVALID);


private:
	void handleAxes(const SDL_Event& event);

private:
	std::list<Controller*> controllers;
	KEY_STATE* keyboard = nullptr;
	uint controller_buffer_timeout = 0;
	float axis_tolerance = 0.1f;
};

#endif