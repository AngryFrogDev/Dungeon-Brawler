
#include "Application.h"
#include "mdRender.h"
#include "mdTextures.h"

#include "DebLog.h"

#include "SDL/include/SDL.h"
#include "SDL_image/include/SDL_image.h"
#pragma comment( lib, "SDL_image/libx86/SDL2_image.lib" )

mdTextures::mdTextures() : Module() {}

// Destructor
mdTextures::~mdTextures() {}

// Called before render is available
bool mdTextures::init() {
	LOG("mdTextures : Init Image library");
	bool ret = true;

	// load support for the PNG image format
	int flags = IMG_INIT_PNG;
	int init = IMG_Init(flags);

	if ((init & flags) != flags) {
		LOG("mdTextures : Could not initialize Image lib. IMG_Init: %s", IMG_GetError());
		ret = false;
	}

	return ret;
}

// Called before q	uitting
bool mdTextures::cleanUp() {
	LOG("Freeing textures and Image library\n");


	for (std::list<SDL_Texture*>::iterator it = textures_list.begin(); it != textures_list.end(); it++) {
		SDL_DestroyTexture(*it);
		*it = nullptr;
	}

	textures_list.clear();
	IMG_Quit();

	return true;
}

// Load new texture from file path
SDL_Texture* const mdTextures::load(const char* path) {
	SDL_Surface* image = IMG_Load(path);
	SDL_Texture* texture_loaded = nullptr;

	if (image == NULL) {
		LOG("mdTextures : Failed to load image \"%s\" IMG_Load: %s\n", path, IMG_GetError());
	}

	else {
		texture_loaded = SDL_CreateTextureFromSurface(App->render->renderer, image);

		if (texture_loaded == NULL) {
			LOG("mdTextures : Failed to create texture from surface SDL_CreateTextureFromSurface: %s\n", SDL_GetError());
		}

		else
			textures_list.push_back(texture_loaded);

		SDL_FreeSurface(image);
	}
	return texture_loaded;
}

SDL_Texture* const mdTextures::loadSurface(SDL_Surface* surface)
{
	SDL_Texture* texture = SDL_CreateTextureFromSurface(App->render->renderer, surface);

	if (texture == NULL)
	{
		LOG("Unable to create texture from surface! SDL Error: %s\n", SDL_GetError());
	}
	else
	{
		textures_list.push_back(texture);
	}

	return texture;
}

SDL_Surface* mdTextures::flipSurface(SDL_Surface *surface, int flags) {
	//Pointer to the soon to be flipped surface
	SDL_Surface *flipped = NULL;

	//If the image is color keyed
	flipped = SDL_CreateRGBSurface(SDL_SWSURFACE, surface->w, surface->h, surface->format->BitsPerPixel, surface->format->Rmask, surface->format->Gmask, surface->format->Bmask, surface->format->Amask);

	//If the surface must be locked
	if (SDL_MUSTLOCK(surface)) {
		//Lock the surface
		SDL_LockSurface(surface);
	}

	//Go through columns
	for (int x = 0, rx = flipped->w - 1; x < flipped->w; x++, rx--) {
		//Go through rows
		for (int y = 0, ry = flipped->h - 1; y < flipped->h; y++, ry--) {
			//Get pixel
			Uint32 pixel = getPixel(surface, x, y);

			//Copy pixel
			if ((flags & FLIP_VERTICAL) && (flags & FLIP_HORIZONTAL)) {
				putPixel(flipped, rx, ry, pixel);
			}
			else if (flags & FLIP_HORIZONTAL) {
				putPixel(flipped, rx, y, pixel);
			}
			else if (flags & FLIP_VERTICAL) {
				putPixel(flipped, x, ry, pixel);
			}
		}
	}

	return flipped;
}

SDL_Texture * const mdTextures::surfaceToTexture(SDL_Surface * surface) const {
	SDL_Texture* ret = SDL_CreateTextureFromSurface(App->render->renderer, surface);

	return ret;
}

//// Load new texture from file path
bool mdTextures::unload(SDL_Texture* texture) {

	if (texture == nullptr)
		return false;

	for (std::list<SDL_Texture*>::iterator it = textures_list.begin(); it != textures_list.end(); it++) {
		if (texture == (*it)) {
			SDL_DestroyTexture((*it));
			textures_list.erase(it);
			return true;
		}
	}

	return false;
}

bool mdTextures::unloadSurface(SDL_Surface * surface) {
	bool ret = true;
	SDL_FreeSurface(surface);
	return ret;
}

void mdTextures::putPixel(SDL_Surface * surface, int x, int y, int color) {
	unsigned int *ptr = (unsigned int*)surface->pixels;
	int lineoffset = y * (surface->pitch / 4);
	ptr[lineoffset + x] = color;
}

int mdTextures::getPixel(SDL_Surface * surface, int x, int y) {
	int ret = 0;
	unsigned int *ptr = (unsigned int*)surface->pixels;
	int lineoffset = y * (surface->pitch / 4);
	ret = ptr[lineoffset + x];
	
	return ret;
}
