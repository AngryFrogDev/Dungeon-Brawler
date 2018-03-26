#ifndef __MDRENDER__
#define __MDRENDER__

#include "Module.h"
#include "SDL/include/SDL.h"

class mdRender : public Module {
public:

	mdRender();

	// Destructor
	virtual ~mdRender();

	// Called before render is available
	bool awake(const pugi::xml_node& md_config);

	// Called before the first frame
	bool start();

	// Called each loop iteration
	bool preUpdate();
	bool postUpdate();

	// Called before quitting
	bool cleanUp();

	// Blit
	void setViewPort(const SDL_Rect& rect);
	void resetViewPort();
	bool blit(SDL_Texture* texture, int x, int y, const SDL_Rect* section = NULL,double scale = 1,bool flip = false, float speed = 1.0f, double angle = 0, int pivot_x = INT_MAX, int pivot_y = INT_MAX) const;
	bool drawQuad(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool filled = true, bool use_camera = true) const;
	bool drawLine(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool use_camera = true) const;
	bool drawCircle(int x1, int y1, int redius, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool use_camera = true) const;

	// Set background color
	void setBackgroundColor(SDL_Color color);

public:

	SDL_Renderer *	renderer;
	SDL_Rect		camera;
	SDL_Rect		viewport;
	SDL_Color		background;

};

#endif // __MDRENDER__