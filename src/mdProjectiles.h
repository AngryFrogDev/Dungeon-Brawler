#pragma once
#include "Module.h"
#include <list>
#include "p2Point.h"
#include "Animation.h"

#include "SDL/include/SDL.h"
#include "SDL_image/include/SDL_image.h"
#pragma comment( lib, "SDL_image/libx86/SDL2_image.lib" )
struct collider;
struct SDL_Texture;


enum PROJECTILE_TYPE {
	DEFF_PROJECTILE,
	WARRIOR_KNIFE
};

struct projectile {
	projectile(Animation animation, iPoint position,iPoint speed, collider* collider, int life,bool fliped, int scale): 
		animation(animation), position(position),speed(speed), collider(collider), life(life),fliped(fliped), scale(scale){
		born = SDL_GetTicks();
	}
	void update();
	void draw(SDL_Texture* graphics);
	
	iPoint position;
	iPoint speed;
	collider* collider;
	Animation animation;
	int born; // in milliseconds
	int life; // in milliseconds
	bool fliped;
	bool to_delete = false;
	int scale;
};

class mdProjectiles :
	public Module {
public:
	mdProjectiles();
	~mdProjectiles();

	bool awake(const pugi::xml_node& md_config);
	bool preUpdate();
	bool update(float dt);
	bool cleanUp();

	projectile* addProjectile(PROJECTILE_TYPE type, iPoint position, iPoint speed, collider* collider, int life,bool fliped, int scale);

	SDL_Texture* graphics;
	Animation warrior_knife;

	std::list<projectile*> projectiles;
};

