
#ifndef __MDWINDOW__
#define __MDWINDOW__

#include "Module.h"
#include <list>

#define MAX_TEXTURES 50

struct SDL_Texture;
struct SDL_Surface;

class mdTextures : public Module {
public:
	mdTextures();
	~mdTextures();

	bool init();
	bool cleanUp();

	SDL_Texture* const Load(const char* path);
	SDL_Surface* const LoadSurface(const char* path);
	SDL_Texture* const SurfaceToTexture(SDL_Surface* surface);
	bool Unload(SDL_Texture* texture);
	bool UnloadSurface(SDL_Surface* surface);

public:
	std::list<SDL_Texture*> textures;
};

#endif // __MDWINDOW__