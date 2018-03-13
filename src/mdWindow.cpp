#include "mdWindow.h"
#include "DebLog.h"

#include "SDL/include/SDL.h"

mdWindow::mdWindow() : Module() {
	name = "window";
	
}

bool mdWindow::awake(const pugi::xml_node& md_config) {
	LOG("mdWindow : Initiating SDL window and surface");
	bool ret = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		LOG("mdWndow : SDL_VIDEO could not initialize! SDL_error: %s", SDL_GetError());
		ret = false;
	}
	else {
		uint32 flags = SDL_WINDOW_SHOWN;
		//TODO: Here this module should read configuration files.
		//This is hardcoded and should be read from the configuration file
		bool fullscreen = false;
		bool borderless = false;
		bool resizable = false;
		bool fullscreen_window = false;

		uint width = 640;
		uint height = 480;
		title = "Test title";
		//End of hardcode

		if (borderless) {
			flags |= SDL_WINDOW_BORDERLESS;
		}

		if (fullscreen) {
			SDL_DisplayMode display_mode;
			SDL_GetDesktopDisplayMode(0, &display_mode);
			width = display_mode.w;
			height = display_mode.h;
			flags |= SDL_WINDOW_FULLSCREEN;
		}
		else {
			if (fullscreen_window) {
				flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
			}

			if (resizable) {
				flags |= SDL_WINDOW_RESIZABLE;
			}
		}

		window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);
		if (window == NULL) {
			LOG("mdWindow : Window could not be created! SDL_Error: %s", SDL_GetError());
			ret = false;
		}
		else
			window_surface = SDL_GetWindowSurface(window);
	}

	return ret;
}

bool mdWindow::cleanUp() {
	LOG("mdWindow : Destroying SDL window");

	if (window != NULL && window != nullptr) {
		SDL_DestroyWindow(window);
		window = nullptr;
	}

	SDL_Quit();
	return true;
}

void mdWindow::setWindowTitle(const char * new_title) {
	title = new_title;
	SDL_SetWindowTitle(window, new_title);
}

void mdWindow::getWindowSize(uint & width, uint & height) const {
	SDL_GetWindowSize(window, (int*)width, (int*)height);
}

void mdWindow::setWindowSize(const uint& width, const uint& height) {
	SDL_SetWindowSize(window, width, height);
}

