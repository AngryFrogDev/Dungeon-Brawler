
#include "Application.h"
#include "mdRender.h"
#include "mdTextures.h"

#include "SDL/include/SDL.h"
#include "SDL_image/include/SDL_image.h"
#pragma comment( lib, "SDL_image/libx86/SDL2_image.lib" )

ModuleTextures::ModuleTextures() : Module() {
	for (uint i = 0; i < MAX_TEXTURES; ++i) {
		textures[i] = nullptr;
		surfaces[i] = nullptr;
	}

}

// Destructor
ModuleTextures::~ModuleTextures() {
}

// Called before render is available
bool ModuleTextures::Init() {
	LOG("Init Image library");
	bool ret = true;

	// load support for the PNG image format
	int flags = IMG_INIT_PNG;
	int init = IMG_Init(flags);

	if ((init & flags) != flags) {
		LOG("Could not initialize Image lib. IMG_Init: %s", IMG_GetError());
		ret = false;
	}

	return ret;
}

// Called before q	uitting
bool ModuleTextures::CleanUp() {
	LOG("Freeing textures and Image library\n");

	for (int i = (MAX_TEXTURES - 1); i >= 0; --i) {
		if (textures[i] != nullptr)
			SDL_DestroyTexture(textures[i]);
		textures[i] = nullptr;

		if (surfaces[i] != nullptr)
			SDL_FreeSurface(surfaces[i]);
		surfaces[i] = nullptr;
	}

	IMG_Quit();
	return true;
}

// Load new texture from file path
SDL_Texture* const ModuleTextures::Load(const char* path) {
	SDL_Surface* image = IMG_Load(path);
	SDL_Texture* ret = nullptr;
	if (image == NULL) {
		LOG("Failed to load image \"%s\" IMG_Load: %s\n", path, IMG_GetError());
	}
	else {
		last_texture = MAX_TEXTURES;

		for (int i = 0; i < MAX_TEXTURES; ++i) {
			if (textures[i] == nullptr) {
				last_texture = i;
				break;
			}
		}
		if (last_texture == MAX_TEXTURES) {
			LOG("Overflow error: Overwriting textures. \n");
			last_texture = 0;
		}
		if (textures[last_texture] != nullptr)
			SDL_DestroyTexture(textures[last_texture]);
		textures[last_texture] = nullptr;
		textures[last_texture] = SDL_CreateTextureFromSurface(App->render->renderer, image);
		if (textures[last_texture] == NULL) {
			LOG("Failed to create texture from surface SDL_CreateTextureFromSurface: %s\n", SDL_GetError());
		}
		else
			ret = textures[last_texture];

		SDL_FreeSurface(image);
	}
	return ret;
}

SDL_Surface * const ModuleTextures::LoadSurface(const char * path) {
	SDL_Surface* ret = nullptr;
	ret = IMG_Load(path);
	if (ret == NULL)
		LOG("Failed to load image \"%s\" IMG_Load: %s\n", path, IMG_GetError());

	return ret;
}

SDL_Texture * const ModuleTextures::SurfaceToTexture(SDL_Surface * surface) {
	SDL_Texture* ret = SDL_CreateTextureFromSurface(App->render->renderer, surface);

	return ret;
}

//// Load new texture from file path
bool ModuleTextures::Unload(SDL_Texture* texture) {
	bool ret = false;

	for (uint i = 0; i < MAX_TEXTURES; ++i) {
		if (texture == textures[i]) {
			SDL_DestroyTexture(textures[i]);
			textures[i] = nullptr;
			ret = true;
			break;
		}
	}

	return ret;
}

bool ModuleTextures::UnloadSurface(SDL_Surface * surface) {
	bool ret = true;
	SDL_FreeSurface(surface);
	return ret;
}
