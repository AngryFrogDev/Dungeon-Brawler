#ifndef _MDPROJECTILES_
#define _MDPROJECTILES_

#include "Module.h"
#include <list>
#include "p2Point.h"
#include "Animation.h"

#include "SDL/include/SDL.h"
#include "SDL_image/include/SDL_image.h"
#pragma comment( lib, "SDL_image/libx86/SDL2_image.lib" )
struct collider;
struct SDL_Texture;
class Character;
class ParticleEmitter;


enum PROJECTILE_TYPE {
	DEFF_PROJECTILE,
	WARRIOR_KNIFE,
	MAGE_FIREBALL,
	MAGE_METEORIT,
	ROGUE_DAGGER
};

struct projectile {
	projectile(Animation* animation, iPoint position,iPoint speed, collider* collider, int life,bool fliped, int scale, PROJECTILE_TYPE type, ParticleEmitter* emitter, iPoint emitter_offset):
		animation(animation), position(position),speed(speed), collider(collider), life(life),fliped(fliped), scale(scale), type(type), emitter(emitter), emitter_offset(emitter_offset) {

		born = SDL_GetTicks();
	}
	void update();
	void draw(SDL_Texture* graphics);
	
	iPoint position;
	iPoint speed;
	collider* collider;
	Animation* animation;
	int born; // in milliseconds
	int life; // in milliseconds
	bool fliped;
	int scale;
	PROJECTILE_TYPE type;
	ParticleEmitter* emitter = nullptr;
	iPoint emitter_offset;
	int gravity = 1;
	float angular_velocity;
	float angle = 0.0f;
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


	int lookForProjectileType(PROJECTILE_TYPE type, Character* character);

	projectile* addProjectile(PROJECTILE_TYPE type, iPoint position, iPoint speed, collider* collider, int life, bool fliped, int scale, ParticleEmitter* emitter = nullptr, iPoint emitter_offset = { 0,0 }, float angular_velocity = 0);


	SDL_Texture* graphics;
	Animation warrior_knife;

	std::list<projectile*> projectiles;


};
#endif
