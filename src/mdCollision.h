#ifndef __ModuleCollision_H__
#define __ModuleCollision_H__

#define MAX_COLLIDERS 10000

#include "Module.h"

#include "SDL/include/SDL.h"
#include "SDL_image/include/SDL_image.h"
#pragma comment( lib, "SDL_image/libx86/SDL2_image.lib" )
enum COLLIDER_TYPE
{
	COLLIDER_NONE = 0,
	COLLIDER_ATTACK,
	COLLIDER_DEFFENSE,
	COLLIDER_MAX
};

struct collider
{
	SDL_Rect rect;
	bool to_delete = false;
	COLLIDER_TYPE type;
	Module* callback = nullptr;
	//Character* character;      

	collider(SDL_Rect rectangle, COLLIDER_TYPE type, Module* callback = nullptr) : rect(rectangle), type(type), callback(callback)
	{}

	void SetPos(int x, int y)
	{
 		rect.x = x;
		rect.y = y;
	}

	void SetSize(int w, int h)
	{
		rect.w = w;
		rect.h = h;
	}

	bool checkCollision(const SDL_Rect& r) const;
};

class mdCollision : public Module
{
public:

	mdCollision();
	~mdCollision();

	bool preUpdate();
	bool update(float dt);
	bool cleanUp();
	void onCollision(collider*, collider*);

	collider* AddCollider(SDL_Rect rect, COLLIDER_TYPE type, Module* callback = nullptr);
	
	void DebugDraw();


	// PROVISIONAL: Test
	collider* collider_one;
	collider* collider_two;

private:

	std::list<collider*> colliders;      // PROVISIONAL: Should be a list
	bool debug = true;						 // PROVISIONAL: Loaded from the xml
};

#endif // __ModuleCollision_H__