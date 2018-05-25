#include "mdInput.h"
#include "DebLog.h"
#define MAX_KEYS 300

Controller::Controller(SDL_GameController* controller, SDL_Haptic* controller_haptic) : 
	controller(controller), controller_haptic(controller_haptic){

	memset(axes, 0.f, sizeof(float) * SDL_CONTROLLER_AXIS_MAX);
	memset(buttons, KEY_IDLE, sizeof(KEY_STATE) * SDL_CONTROLLER_BUTTON_MAX);

	id = SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(controller));

	if (SDL_HapticRumbleSupported(controller_haptic))
		SDL_HapticRumbleInit(controller_haptic);
}

Controller::~Controller() {
	if (controller_haptic != nullptr && SDL_HapticRumbleSupported(controller_haptic))
		SDL_HapticRumbleStop(controller_haptic);
	SDL_HapticClose(controller_haptic);
	SDL_GameControllerClose(controller);
	input_buffer.clear();
}

bool Controller::isPressed(CONTROLLER_BUTTON button, KEY_STATE state) const {
	return buttons[button] == state;
}

const std::list<CONTROLLER_BUTTON> Controller::getInputs() const {
	std::list<CONTROLLER_BUTTON> ret;

	for (std::list<input_record>::const_iterator it = input_buffer.begin(); it != input_buffer.end(); ++it)
		ret.push_back((*it).input);

	return ret;
}

void Controller::addInput(CONTROLLER_BUTTON input, uint timestamp) {
	if (buttons[input] != KEY_REPEAT || buttons[input] != KEY_DOWN) {
		buttons[input] = KEY_DOWN;
		input_record new_input;
		new_input.input = input;
		if (timestamp == NULL)
			timestamp = SDL_GetTicks();
		new_input.timestamp = timestamp;
		input_buffer.push_back(new_input);
	}
}

void Controller::popInput() {
	if (!input_buffer.empty()) 
		input_buffer.pop_front();
}

void Controller::pruneInput(uint milliseconds) {
	while (!input_buffer.empty() && input_buffer.front().timestamp + milliseconds <= SDL_GetTicks()) {
		popInput();
	}
}

bool Controller::shakeController(float intensity, uint milliseconds) const {
	bool ret = !SDL_HapticRumblePlay(controller_haptic, intensity, milliseconds) == 0;
	if (!ret)
		LOG("mdInput : Controller failed to rumble - Error: %s", SDL_GetError());

	return ret;
}

uint Controller::getControllerID() const {
	return id;
}

mdInput::mdInput() : Module(){
	name = "input";
}

mdInput::~mdInput() {}

bool mdInput::awake(const pugi::xml_node & md_config) {
	bool ret = true;
	LOG("mdInput : Initiating SDL input event system");

	SDL_Init(0);

	if (SDL_InitSubSystem(SDL_INIT_EVENTS) != 0) {
		LOG("mdInput : SDL Events could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	if (SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER) != 0) {
		LOG("mdInput : SDL Controller could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	if (SDL_InitSubSystem(SDL_INIT_HAPTIC) != 0) {
		LOG("mdInput : SDL Haptic could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	keyboard = new KEY_STATE[MAX_KEYS];
	memset(keyboard, KEY_IDLE, sizeof(KEY_STATE) * MAX_KEYS);
	controller_buffer_timeout = md_config.child("buffer").attribute("window").as_uint(0);
	axis_tolerance = md_config.child("axis").attribute("tolerance").as_float(0.2f);

	return ret;
}

bool mdInput::preUpdate() {
	bool ret = true;
	const Uint8* keys = SDL_GetKeyboardState(NULL);

	for (int i = 0; i < MAX_KEYS; ++i) {
		if (keys[i] == 1) {
			if (keyboard[i] == KEY_IDLE)
				keyboard[i] = KEY_DOWN;
			else
				keyboard[i] = KEY_REPEAT;
		}
		else {
			if (keyboard[i] == KEY_REPEAT || keyboard[i] == KEY_DOWN)
				keyboard[i] = KEY_UP;
			else
				keyboard[i] = KEY_IDLE;
		}
	}

	for (std::list<Controller*>::iterator it = controllers.begin(); it != controllers.end(); ++it) {
		(*it)->pruneInput(controller_buffer_timeout);
		for (int i = 0; i < BUTTON_MAX; ++i) {
			if ((*it)->buttons[i] == KEY_UP)
				(*it)->buttons[i] = KEY_IDLE;
			else if ((*it)->buttons[i] == KEY_DOWN)
				(*it)->buttons[i] = KEY_REPEAT;
		}
	}

	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			ret = false; //TODO : This should call a quit function
			break;
		case SDL_CONTROLLERDEVICEADDED:
			if (SDL_IsGameController(event.cdevice.which)) {
				int index = event.cdevice.which;
				Controller* ctrl = new Controller(SDL_GameControllerOpen(index), SDL_HapticOpen(index));
				if (ctrl != nullptr)
					controllers.push_back(ctrl);
			}
			break;
		case SDL_CONTROLLERDEVICEREMOVED:
			for (std::list<Controller*>::iterator it = controllers.begin(); it != controllers.end(); ++it) {
				if ((*it)->getControllerID() == event.cdevice.which) {
					RELEASE(*it);
					it = controllers.erase(it);
					break;
				}
			}
			break;
		case SDL_CONTROLLERBUTTONDOWN:
			for (std::list<Controller*>::iterator it = controllers.begin(); it != controllers.end(); ++it) {
				if ((*it)->getControllerID() == event.cbutton.which) {
					(*it)->addInput((CONTROLLER_BUTTON)event.cbutton.button, event.cbutton.timestamp);
					break;
				}
			}
			break;

		case SDL_CONTROLLERBUTTONUP:
			for (std::list<Controller*>::iterator it = controllers.begin(); it != controllers.end(); ++it) {
				if ((*it)->getControllerID() == event.cbutton.which) {
					(*it)->buttons[event.cbutton.button] = KEY_UP;
					break;
				}
			}
			break;

		case SDL_CONTROLLERAXISMOTION:
			handleAxes(event);
			break;
		}
	}

	return ret;
}

bool mdInput::cleanUp() {
	bool ret = true;
	for (std::list<Controller*>::iterator it = controllers.begin(); it != controllers.end(); ++it)
		RELEASE(*it);
	controllers.clear();

	LOG("mdInput : Quitting SDL Event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	LOG("mdInput : Quitting SDL Controller subsystem");
	SDL_QuitSubSystem(SDL_INIT_GAMECONTROLLER);
	LOG("mdInput : Quitting SDL Haptic subsystem");
	SDL_QuitSubSystem(SDL_INIT_HAPTIC);

	return ret;
}

void mdInput::handleAxes(const SDL_Event & event) {
	uint device_index = event.caxis.which;
	for (std::list<Controller*>::iterator it = controllers.begin(); it != controllers.end(); ++it) {
		if ((*it)->getControllerID() == device_index) {
			float axis_value = event.caxis.value / 16000; //32767 is the max value for an SDL axis
			float prior_value = (*it)->axes[event.caxis.axis];
			(*it)->axes[event.caxis.axis] = axis_value;
			switch (event.caxis.axis) {
			case SDL_CONTROLLER_AXIS_LEFTX:
				if (axis_value < prior_value) {
					if (prior_value >= axis_tolerance && axis_value < axis_tolerance)
						(*it)->buttons[AXIS_LEFTX_POSITIVE] = KEY_UP;
					if (prior_value > -axis_tolerance && axis_value <= -axis_tolerance)
						(*it)->addInput(AXIS_LEFTX_NEGATIVE, event.caxis.timestamp);
				}
				else {
					if (prior_value <= -axis_tolerance && axis_value > -axis_tolerance)
						(*it)->buttons[AXIS_LEFTX_NEGATIVE] = KEY_UP;
					if (prior_value < axis_tolerance && axis_value >= axis_tolerance)
						(*it)->addInput(AXIS_LEFTX_POSITIVE, event.caxis.timestamp);
				}
				break;
			case SDL_CONTROLLER_AXIS_LEFTY:
				if (axis_value < prior_value) {
					if (prior_value >= axis_tolerance && axis_value < axis_tolerance)
						(*it)->buttons[AXIS_LEFTY_POSITIVE] = KEY_UP;
					if (prior_value > -axis_tolerance && axis_value <= -axis_tolerance)
						(*it)->addInput(AXIS_LEFTY_NEGATIVE, event.caxis.timestamp);
				}
				else {
					if (prior_value <= -axis_tolerance && axis_value > -axis_tolerance)
						(*it)->buttons[AXIS_LEFTY_NEGATIVE] = KEY_UP;
					if (prior_value < axis_tolerance && axis_value >= axis_tolerance)
						(*it)->addInput(AXIS_LEFTY_POSITIVE, event.caxis.timestamp);
				}
				break;
			}
		}
	}
}


KEY_STATE mdInput::getKey(SDL_Scancode key) const{
	return keyboard[key];
}

KEY_STATE mdInput::getControllerButton(int id, SDL_GameControllerButton button) {
	KEY_STATE ret = KEY_NULL;
	for ( std::list<Controller*>::iterator it = controllers.begin(); it != controllers.end(); ++it) {
		if ((*it)->getControllerID() == id) {
			ret = (*it)->buttons[button];
			break;
		}
	}

	return ret;
}

bool mdInput::isButtonState(SDL_GameControllerButton button, KEY_STATE state, int id) {
	bool ret = false;
	for (std::list<Controller*>::iterator it = controllers.begin(); !ret && it != controllers.end(); ++it) {
		if (id == -1 || (*it)->getControllerID() == id)
			ret = (*it)->buttons[button] == state;
	}
	return ret;
}

std::list<Controller*> mdInput::getController(SDL_GameControllerButton button){
	std::list<Controller*> ret;
	if (button == SDL_CONTROLLER_BUTTON_INVALID)
		ret = controllers;
	else {
		for (std::list<Controller*>::iterator it = controllers.begin(); it != controllers.end(); ++it)
			if ((*it)->buttons[button]) ret.push_back(*it);
	}

	return ret;
}
