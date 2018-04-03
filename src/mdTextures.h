#ifndef __MDTEXTURES__
#define __MDTEXTURES__

#include "Module.h"
#include <list>

struct SDL_Texture;
struct SDL_Surface;

class mdTextures : public Module {
public:
	mdTextures();
	~mdTextures();

	bool init();
	bool cleanUp();

	SDL_Texture* const load(const char* path);
	SDL_Texture* const loadSurface(SDL_Surface* surface);
	SDL_Texture* const surfaceToTexture(SDL_Surface* surface) const;
	bool unload(SDL_Texture* texture);
	bool unloadSurface(SDL_Surface* surface);

public:
	std::list<SDL_Texture*> textures_list;
};

#endif // __MDTEXTURES__