#include "DebLog.h"
#include "Application.h"
#include "mdWindow.h"
#include "mdRender.h"

#include "Brofiler/Brofiler.h"

mdRender::mdRender() : Module() {
	name = "renderer";
	background.r = 0;
	background.g = 0;
	background.b = 0;
	background.a = 100;
}

// Destructor
mdRender::~mdRender()
{}

// Called before render is available
bool mdRender::awake(const pugi::xml_node& md_config) {
	LOG("Create SDL rendering context");
	bool ret = true;
	// load flags
	Uint32 flags = SDL_RENDERER_ACCELERATED;

	if (md_config.child("vsync").attribute("value").as_bool(true)) {
		flags |= SDL_RENDERER_PRESENTVSYNC;
		LOG("Using vsync");
	}

	renderer = SDL_CreateRenderer(App->window->window, -1, flags);

	if (renderer == NULL) {
		LOG("Could not create the renderer! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else {
		camera.w = App->window->window_surface->w;
		camera.h = App->window->window_surface->h;
		camera.x = 0;
		camera.y = 0;
	}

	return ret;
}

// Called before the first frame
bool mdRender::start() {
	LOG("render start");
	// back background
	SDL_RenderGetViewport(renderer, &viewport);
	return true;
}

// Called each loop iteration
bool mdRender::preUpdate() {
	SDL_RenderClear(renderer);

	return true;
}

bool mdRender::postUpdate() {
	//BROFILER_CATEGORY("Render PostUpdate", Profiler::Color::Red);
	drawBlit(blitQueue);
	SDL_SetRenderDrawColor(renderer, background.r, background.g, background.g, background.a);
	BROFILER_CATEGORY("Swapping Buffers", 0xFFFF0000);
	SDL_RenderPresent(renderer);

	return true;
}

// Called before quitting
bool mdRender::cleanUp() {
	LOG("Destroying SDL render");
	SDL_DestroyRenderer(renderer);
	return true;
}

void mdRender::setBackgroundColor(SDL_Color color) {
	background = color;
}

void mdRender::setViewPort(const SDL_Rect& rect) {
	SDL_RenderSetViewport(renderer, &rect);
}

void mdRender::resetViewPort() {
	SDL_RenderSetViewport(renderer, &viewport);
}

// Add to blit queue according to priority
bool mdRender::blit(int priority, SDL_Texture* texture, int x, int y, const SDL_Rect* section, double scale,bool flip, float speed, double angle, int pivot_x, int pivot_y) {
	bool ret = true;

	objectToPrint* sprite = new objectToPrint(priority, texture, x, y, section, scale, flip, speed, angle, pivot_x, pivot_y);
	blitQueue.push(sprite);

	return ret;
}

// Blit to screen
bool mdRender::drawBlit(priority_queue <objectToPrint*, vector<objectToPrint*>, orderCrit>& queue) const {

	bool ret = true;

	while (queue.empty() == false) {
		objectToPrint* first = queue.top();

		SDL_Rect rect;
		rect.x = (int)(camera.x * first->speed) + first->x;
		rect.y = (int)(camera.y * first->speed) + first->y;

		if (first->section != NULL) {
			rect.w = first->section->w;
			rect.h = first->section->h;
		}
		else
			SDL_QueryTexture(first->texture, NULL, NULL, &rect.w, &rect.h);

		rect.w *= first->scale;
		rect.h *= first->scale;

		SDL_Point* p = NULL;
		SDL_Point pivot;

		if (first->pivot_x != INT_MAX && first->pivot_y != INT_MAX) {
			pivot.x = first->pivot_x;
			pivot.y = first->pivot_y;
			p = &pivot;
		}

		SDL_RendererFlip flip_flag;
		if (first->flip)
			flip_flag = SDL_FLIP_HORIZONTAL;
		else
			flip_flag = SDL_FLIP_NONE;
			
		if (SDL_RenderCopyEx(renderer, first->texture, first->section, &rect, first->angle, p, flip_flag) != 0) {
			LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
			ret = false;
		}

		RELEASE(first);

		queue.pop();


	}

		return ret;
}


bool mdRender::drawQuad(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool filled, bool use_camera) const {
	bool ret = true;

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	SDL_Rect rec(rect);
	if (use_camera) {
		rec.x = (int)(camera.x + rect.x);
		rec.y = (int)(camera.y + rect.y);
	}

	int result = (filled) ? SDL_RenderFillRect(renderer, &rec) : SDL_RenderDrawRect(renderer, &rec);

	if (result != 0) {
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool mdRender::drawLine(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera) const {
	bool ret = true;

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	int result = -1;

	if (use_camera)
		result = SDL_RenderDrawLine(renderer, camera.x + x1, camera.y + y1, camera.x + x2, camera.y + y2);
	else
		result = SDL_RenderDrawLine(renderer, x1, y1, x2, y2);

	if (result != 0) {
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool mdRender::drawCircle(int x, int y, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera) const {
	bool ret = true;

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	int result = -1;
	SDL_Point points[360];

	float factor = (float)M_PI / 180.0f;

	for (uint i = 0; i < 360; ++i) {
		points[i].x = (int)(x + radius * cos(i * factor));
		points[i].y = (int)(y + radius * sin(i * factor));
	}

	result = SDL_RenderDrawPoints(renderer, points, 360);

	if (result != 0) {
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}
