#ifndef __ModuleCollision_H__
#define __ModuleCollision_H__

#define MAX_COLLIDERS 10000

#include "Module.h"

#include "SDL/include/SDL.h"
#include "SDL_image/include/SDL_image.h"
#pragma comment( lib, "SDL_image/libx86/SDL2_image.lib" )

class Character;
enum COLLIDER_TYPE
{
	COLLIDER_NONE = 0,
	HURTBOX,
	HITBOX
};

struct collider
{

	// Collider properties
	SDL_Rect rect;
	COLLIDER_TYPE type;
	int life; //In miliseconds (set -1 so the collider is not erased by time)
	//Callbacks
	Module* callback = nullptr;
	Character* character;      
	// Collider state
	int born; //In miliseconds 
	bool to_delete = false;

	collider(SDL_Rect rectangle, COLLIDER_TYPE type, int life, Module* callback, Character* character) : 
		rect(rectangle), type(type), life(life), callback(callback), character(character) {
		born = SDL_GetTicks();
	}

	void SetPos(int x, int y) {
 		rect.x = x;
		rect.y = y;
	}

	void SetSize(int w, int h) {
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

	collider* AddCollider(SDL_Rect rect, COLLIDER_TYPE type,int life, Module* callback = nullptr, Character* character = nullptr);
	
	void DebugDraw();


private:

	std::list<collider*> colliders;      
	bool debug = true;						 // PROVISIONAL: Loaded from the xml
};

#endif // __ModuleCollision_H__