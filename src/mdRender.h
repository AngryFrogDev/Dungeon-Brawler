#ifndef __MDRENDER__
#define __MDRENDER__

#include <queue>
#include <vector>

#include "Module.h"
#include "SDL/include/SDL.h"

using namespace std;

class objectToPrint {
public:

	objectToPrint(int priority, SDL_Texture* texture, int x, int y, const SDL_Rect* section, double scale,bool flip, float speed, double angle, int pivot_x, int pivot_y) :
		priority(priority), texture(texture), x(x), y(y), section(section), scale(scale),flip(flip), speed(speed), angle(angle), pivot_x(pivot_x), pivot_y(pivot_y) {}

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

	int					priority;
};

struct orderCrit {
	bool operator()(const objectToPrint* obj1, const objectToPrint* obj2)const {
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

	

	bool blit(int priority, SDL_Texture* texture, int x, int y, const SDL_Rect* section = NULL, double scale = 1,bool flip = false, float speed = 1.0f, double angle = 0, int pivot_x = INT_MAX, int pivot_y = INT_MAX);
	bool drawBlit(priority_queue <objectToPrint*, vector<objectToPrint*>, orderCrit>& Queue) const;
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

private:

	priority_queue <objectToPrint*, vector<objectToPrint*>, orderCrit> blitQueue;
};

#endif // __MDRENDER__