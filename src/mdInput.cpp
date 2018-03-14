#include "mdInput.h"
#include "SDL/include/SDL.h"
#include "SDL/include/SDL_gamecontroller.h"
#include "DebLog.h"

Controller::Controller(SDL_GameController * controller, SDL_Haptic* controller_haptic) : 
	controller(controller), controller_haptic(controller_haptic) {
	for (int i = 0; i < SDL_CONTROLLER_AXIS_MAX; ++i)
		axes[i] = 0.f;

	for (int i = 0; i < SDL_CONTROLLER_BUTTON_MAX; ++i)
		buttons[i] = KEY_IDLE;
}

Controller::~Controller() {
}

const std::deque<CONTROLLER_BUTTON>& Controller::getInputs() const {
	return input_buffer;
}

void Controller::addInput(CONTROLLER_BUTTON input, uint timestamp) {
	input_buffer.push_back(input);
	if (timestamp == NULL)
		timestamp = SDL_GetTicks();
	input_times.push_back(timestamp);
}

void Controller::popInput() {
	if (!input_times.empty()) {
		input_buffer.pop_front();
		input_times.pop_front();
	}
}

void Controller::pruneInput(uint milliseconds) {
	while (!input_times.empty() && input_times[0] + milliseconds <= SDL_GetTicks()) {
		input_buffer.pop_front();
		input_times.pop_front();
	}
}
