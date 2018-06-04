#ifndef __MDTEXTURES__
#define __MDTEXTURES__

#include "Module.h"
#include <list>

struct SDL_Texture;
struct SDL_Surface;
#define FLIP_VERTICAL 1
#define FLIP_HORIZONTAL 2

class mdTextures : public Module {
public:
	mdTextures();
	~mdTextures();

	bool init();
	bool cleanUp();

	SDL_Texture* const load(const char* path);
	SDL_Texture* const loadSurface(SDL_Surface* surface);
	SDL_Surface* flipSurface(SDL_Surface* surface, int flags);
	SDL_Texture* const surfaceToTexture(SDL_Surface* surface) const;
	bool unload(SDL_Texture* texture);
	bool unloadSurface(SDL_Surface* surface);

private:
	void putPixel(SDL_Surface* surface, int x, int y, int color);
	int getPixel(SDL_Surface* surface, int x, int y);

public:
	std::list<SDL_Texture*> textures_list;
};

#endif // __MDTEXTURES__