#include "PerfTimer.h"
#include "SDL\include\SDL_timer.h"

uint64 PerfTimer::frequency = 0;

PerfTimer::PerfTimer() {
	frequency = SDL_GetPerformanceFrequency();
	start();
}

void PerfTimer::start() {
	started_at = SDL_GetPerformanceCounter();
}
double PerfTimer::readMs() const {
	return 1000.0 * (double(SDL_GetPerformanceCounter() - started_at) / double(frequency));
}
uint64 PerfTimer::readTicks() const {
	return SDL_GetPerformanceCounter() - started_at;
}