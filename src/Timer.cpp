#include "Timer.h"
#include "SDL\include\SDL_timer.h"

Timer::Timer() {
	start();
}

void Timer::start() {
	started_at = SDL_GetTicks();
	active = true;
}
uint32 Timer::read() const {
	if (active)
	{
		if (paused)
			return (paused_at - started_at);
		else
			return (SDL_GetTicks() - started_at);
	}
	else
		return 0;
}
float Timer::readSec() const {
	return float(read()) / 1000.0f;
}

void Timer::stop() {
	active = false;
}

//Pause or unpause timer
void Timer::pauseUnpause() {
	if (paused == true)
	{
		started_at += (SDL_GetTicks() - paused_at);

		paused_at = 0;
		paused = false;
	}
	else
	{
		paused_at = SDL_GetTicks();
		paused = true;
	}
}

bool Timer::isPaused() {
	return paused;
}
bool Timer::isActive() {
	return active;
}