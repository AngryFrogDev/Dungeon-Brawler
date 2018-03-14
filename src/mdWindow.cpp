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
		bool fullscreen = md_config.child("fullscreen").attribute("value").as_bool(false);
		bool borderless = md_config.child("borderless").attribute("value").as_bool(false);
		bool resizable = md_config.child("resizable").attribute("value").as_bool(false);
		bool fullscreen_window = md_config.child("fullscreen_window").attribute("value").as_bool(false);

		uint width = md_config.child("resolution").attribute("width").as_int(640);
		uint height = md_config.child("resolution").attribute("height").as_int(480);
		title = md_config.child("title").text().as_string();

		scale = md_config.child("resolution").attribute("scale").as_int(1);

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

