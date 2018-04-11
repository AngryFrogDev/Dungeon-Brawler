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
		viewport.w = camera.w = App->window->window_surface->w;
		viewport.h = camera.h = App->window->window_surface->h;
		viewport.x = camera.x = 0;
		viewport.y = camera.y = 0;
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
	blitSprites(spriteQueue);
	blitQuads(quadQueue);

	return true;
}

bool mdRender::postUpdate() {
	//BROFILER_CATEGORY("Render PostUpdate", Profiler::Color::Red);
	//drawBlit(blitQueue);
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
bool mdRender::drawSprite(int priority, SDL_Texture* texture, int x, int y, const SDL_Rect* section, double scale,bool flip, float speed, double angle, int pivot_x, int pivot_y) {
	bool ret = true;

	spriteToPrint* sprite = new spriteToPrint(priority, texture, x, y, section, scale, flip, speed, angle, pivot_x, pivot_y);
	spriteQueue.push(sprite);

	return ret;
}

// Blit to screen
bool mdRender::blitSprites(priority_queue <spriteToPrint*, vector<spriteToPrint*>, spriteOrderCrit>& queue) const {

	bool ret = true;

	while (queue.empty() == false) {
		spriteToPrint* first = queue.top();

		SDL_Rect rect;
		rect.x = first->x - (int)(camera.x * first->speed);
		rect.y = first->y - (int)(camera.y * first->speed);

		if (first->section != NULL) {
			rect.w = first->section->w;
			rect.h = first->section->h;
		}
		else
			SDL_QueryTexture(first->texture, NULL, NULL, &rect.w, &rect.h);

		rect.w *= first->scale;
		rect.h *= first->scale;

		//If the sprite is out of the viewport, no need to render it.
		if (rect.x + rect.w < 0 || rect.x > viewport.w ||
			rect.y + rect.h < 0 || rect.y > viewport.h) {
			RELEASE(first);
			queue.pop();
			continue;
		}

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
		
		SDL_RenderSetLogicalSize(renderer, 1920, 1080); // PROVISIONAL: This should be loaded from xml

		if (SDL_RenderCopyEx(renderer, first->texture, first->section, &rect, first->angle, p, flip_flag) != 0) {
			LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
		}

		RELEASE(first);
		queue.pop();
	}

		return ret;
}


bool mdRender::drawQuad(int priority, const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool filled, bool use_camera) {
	bool ret = true;

	quadToPrint* quad = new quadToPrint(priority, rect, r, g, b, a, filled, use_camera);
	quadQueue.push(quad);

	return ret;
}

bool mdRender::blitQuads(priority_queue <quadToPrint*, vector<quadToPrint*>, quadOrderCrit>& queue) const {

	bool ret = true;

	while (queue.empty() == false) {
		quadToPrint* first = queue.top();

		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(renderer, first->r, first->g, first->b, first->a);

		SDL_Rect rec(first->rect);
		if (first->use_camera) {
			rec.x = (int)(camera.x + first->rect.x);
			rec.y = (int)(camera.y + first->rect.y);
		}

		int result = (first->filled) ? SDL_RenderFillRect(renderer, &rec) : SDL_RenderDrawRect(renderer, &rec);

		if (result != 0) {
			LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		}

		RELEASE(first);
		queue.pop();
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

void mdRender::cleanBlitQueue() {
	while (spriteQueue.empty() == false) {
		spriteToPrint* first = spriteQueue.top();
		delete first;
		spriteQueue.pop();
	}
	while (quadQueue.empty() == false) {
		quadToPrint* first = quadQueue.top();
		delete first;
		quadQueue.pop();
	}
}