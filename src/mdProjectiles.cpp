#include "mdProjectiles.h"
#include "Application.h"
#include "mdRender.h"
#include "mdTextures.h"
#include "mdCollision.h"
#include "DebLog.h"


mdProjectiles::mdProjectiles() {
}


mdProjectiles::~mdProjectiles() {
}


bool mdProjectiles::awake(const pugi::xml_node& md_config) {

	graphics = App->textures->load("Assets/particles.png");
	warrior_knife.PushBack({ 0,0,37,11 });

	warrior_knife.speed = 0.2;
	warrior_knife.loop = false;
	return true;
}

bool mdProjectiles::preUpdate() {

	std::list<projectile*> projectiles_to_delete;
	// Remove all projectiles scheduled for deletion

	//Iterate list looking for the projectiles
	for (std::list<projectile*>::iterator it = projectiles.begin(); it != projectiles.end(); ++it) {
		projectile* p = *it;
		if (p->collider->to_delete) { // Projectiles are deleted with their colliders
			projectiles_to_delete.push_back(p);
		}
	}

	// Remove the projectiles
	for (std::list<projectile*>::iterator it = projectiles_to_delete.begin(); it != projectiles_to_delete.end(); ++it) {
		projectile* p = *it;
		p->collider->character->setProjectile(false);
		projectiles.remove(p);
		delete p;
	}

	projectiles_to_delete.clear();
	return true;
}

bool mdProjectiles::update(float dt) {

	// Logical update
	for (std::list<projectile*>::iterator it = projectiles.begin(); it != projectiles.end(); ++it) {
		projectile* p = *it;
		p->update();
	}
	// Drawing
	for (std::list<projectile*>::iterator it = projectiles.begin(); it != projectiles.end(); ++it) {
		projectile* p = *it;
		p->draw(graphics);
	}
	// Check life
	for (std::list<projectile*>::iterator it = projectiles.begin(); it != projectiles.end(); ++it) {
		projectile* p = *it;
		if (p->life != -1 && SDL_GetTicks() - p->born > p->life)/*PROVISIONAL: Maybe it should use a timer*/ {
			p->to_delete = true;
		}
	}

	return true;
}

bool mdProjectiles::cleanUp() {
	LOG("Freeing all projectiles");

	for (std::list<projectile*>::iterator it = projectiles.begin(); it != projectiles.end(); ++it) {
		projectile* p = *it;
		delete p;
	}

	projectiles.clear();
	return true;
}

projectile* mdProjectiles::addProjectile(PROJECTILE_TYPE type,iPoint position, iPoint speed,collider* collider,int life, bool fliped, int scale) {

	projectile* new_projectile;
	switch (type) {
		case WARRIOR_KNIFE:
			new_projectile = new projectile(warrior_knife,position, speed, collider,life, fliped, scale); 
	}	

	projectiles.push_back(new_projectile);
	return new_projectile;
}

void projectile::update() {
	position += speed;
	if (collider != nullptr) {
		collider->SetPos(position.x, position.y);
	}
}
void projectile::draw(SDL_Texture* graphics) {
	App->render->blit(3, graphics, position.x, position.y, &animation.GetCurrentFrame(),scale, fliped); 
}