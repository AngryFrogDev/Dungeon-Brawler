#ifndef __ModuleCollision_H__
#define __ModuleCollision_H__

#define MAX_COLLIDERS 10000

#include "Module.h"
#include "Character.h"

#include "SDL/include/SDL.h"
#include "SDL_image/include/SDL_image.h"
#pragma comment( lib, "SDL_image/libx86/SDL2_image.lib" )

class Character;
enum COLLIDER_TYPE
{
	COLLIDER_NONE = 0,
	HURTBOX,
	PROJECTILE_INVENCIBLE_HURTBOX,
	HITBOX,
	PUSHBOX,
	PROJECTILE_HITBOX
};

struct collider
{

	// Collider properties
	SDL_Rect rect;
	COLLIDER_TYPE type;
	basic_attack_deff attack_type;
	int life; //In miliseconds (set -1 so the collider is not erased by time)
	//Callbacks
	Character* character;      
	// Collider state
	int born; //In miliseconds 
	bool to_delete = false;
	bool active = true;

	collider(SDL_Rect rectangle, COLLIDER_TYPE type, int life,basic_attack_deff attack_type, Character* character) :
		rect(rectangle), type(type), life(life), attack_type(attack_type), character(character) {
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

	bool awake(const pugi::xml_node& md_config);
	bool preUpdate();
	bool update(float dt);
	bool cleanUp();
	void onCollision(collider*, collider*); //Just for the test

	collider* AddCollider(SDL_Rect rect, COLLIDER_TYPE type, int life, basic_attack_deff attack_type, Character* character);
	
	void DebugDraw();

	std::list<COLLIDER_TYPE> hurtbox, projectile_invencible_hurtbox, pushbox, hitbox, projectile_hitbox;
	bool isInteractive(COLLIDER_TYPE type1, COLLIDER_TYPE type2);
	bool lookForColliderType(std::list<COLLIDER_TYPE>* list, COLLIDER_TYPE type);
	bool debug = false;						 // PROVISIONAL: Loaded from the xml

private:

	std::list<collider*> colliders;      
};

#endif // __ModuleCollision_H__