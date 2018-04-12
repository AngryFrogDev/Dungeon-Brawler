#ifndef __MDRENDER__
#define __MDRENDER__

#include <queue>
#include <vector>
#include <utility>

#include "Module.h"
#include "SDL/include/SDL.h"

class spriteToPrint {
public:

	spriteToPrint(int priority, SDL_Texture* texture, int x, int y, const SDL_Rect* section, double scale,bool flip, float speed, double angle, int pivot_x, int pivot_y, bool use_camera) :
		priority(priority), texture(texture), x(x), y(y), section(section), scale(scale),flip(flip), speed(speed), angle(angle), pivot_x(pivot_x), pivot_y(pivot_y), use_camera(use_camera) {}

	int getPriority()const {
		return priority;
	}

public:
	SDL_Texture*		texture;
	int					x;
	int					y;
	const SDL_Rect*		section;
	SDL_Rect			rect;
	float				scale;
	float				speed;
	double				angle;
	int					pivot_x;
	int					pivot_y;
	bool				flip;
	bool				use_camera;
	int					priority;
};

struct spriteOrderCrit {
	bool operator()(const spriteToPrint* obj1, const spriteToPrint* obj2)const {
		return obj1->getPriority() > obj2->getPriority();
	}
};

class quadToPrint {
public:

	quadToPrint(int priority, const SDL_Rect rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool filled, bool use_camera) :
		priority(priority), rect(rect), r(r), g(g), b(b), a(a), filled(filled), use_camera(use_camera) {}

	int getPriority()const {
		return priority;
	}

public:
	const SDL_Rect rect;
	Uint8 r;
	Uint8 g;
	Uint8 b;
	Uint8 a;
	bool filled;
	bool use_camera;

	int					priority;
};

struct quadOrderCrit {
	bool operator()(const quadToPrint* obj1, const quadToPrint* obj2)const {
		return obj1->getPriority() > obj2->getPriority();
	}
};

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

	

	bool drawSprite(int priority, SDL_Texture* texture, int x, int y, const SDL_Rect* section = NULL, double scale = 1,bool flip = false, float speed = 1.0f, double angle = 0, int pivot_x = INT_MAX, int pivot_y = INT_MAX, bool use_camera = true);
	bool blitSprites(std::priority_queue <spriteToPrint*, std::vector<spriteToPrint*>, spriteOrderCrit>& queue) const;
	bool drawQuad(int priority, const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool filled = true, bool use_camera = true);
	bool blitQuads(std::priority_queue <quadToPrint*, std::vector<quadToPrint*>, quadOrderCrit>& queue) const;
	void cleanBlitQueue();
	bool drawLine(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool use_camera = true) const;
	bool drawCircle(int x1, int y1, int redius, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool use_camera = true) const;

	// Set background color
	void setBackgroundColor(SDL_Color color);

public:

	SDL_Renderer *			renderer;
	SDL_Rect				camera;
	SDL_Rect				viewport;
	SDL_Color				background;
	std::pair<uint, uint>	resolution;

private:

	std::priority_queue <spriteToPrint*, std::vector<spriteToPrint*>, spriteOrderCrit> spriteQueue;
	std::priority_queue <quadToPrint*, std::vector<quadToPrint*>, quadOrderCrit> quadQueue;
};

#endif // __MDRENDER__