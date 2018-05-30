#include "mdProjectiles.h"
#include "Application.h"
#include "mdRender.h"
#include "mdTextures.h"
#include "mdCollision.h"
#include "mdParticleSystem.h"
#include "ParticleEmitter.h"
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

	paladin_hammer.PushBack({65, 0, 21, 20 });
	paladin_hammer.speed = 0.2;
	paladin_hammer.loop = false;

	rogue_arrow.PushBack({ 1,18,38,8 });
	rogue_arrow.speed = 0.2;
	rogue_arrow.loop = false;
	return true;
}

bool mdProjectiles::preUpdate() {

	std::list<projectile*> projectiles_to_delete;
	// Remove all projectiles scheduled for deletion

	//Iterate list looking for the projectiles to delete
	for (std::list<projectile*>::iterator it = projectiles.begin(); it != projectiles.end(); ++it) {
		projectile* p = *it;
		if (p->collider->to_delete) { // Projectiles are deleted with their colliders
			projectiles_to_delete.push_back(p);
		}
	}

	// Remove the projectiles
	for (std::list<projectile*>::iterator it = projectiles_to_delete.begin(); it != projectiles_to_delete.end(); ++it) {
		projectile* p = *it;
		projectiles.remove(p);
		if(p->emitter)
			p->emitter->active = false;
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

	return true;
}

bool mdProjectiles::cleanUp() {
	LOG("Freeing all projectiles");

	for (std::list<projectile*>::iterator it = projectiles.begin(); it != projectiles.end(); ++it) {
		projectile* p = *it;
		if(p->emitter)
			p->emitter->active = false;
		delete p;
	}

	projectiles.clear();
	return true;
}

projectile* mdProjectiles::addProjectile(PROJECTILE_TYPE type,iPoint position, iPoint speed,collider* collider, int life, bool fliped, int scale, ParticleEmitter* emitter, iPoint emitter_offset, float angular_velocity) {

	projectile* new_projectile = nullptr;
	switch (type) {
		case WARRIOR_KNIFE:
			new_projectile = new projectile(&warrior_knife,position, speed, collider,life, fliped, scale, WARRIOR_KNIFE, emitter, emitter_offset);
			break;
		case MAGE_FIREBALL:
			new_projectile = new projectile(nullptr, position, speed, collider, life, fliped, scale, MAGE_FIREBALL, emitter, emitter_offset);
			break;
		case MAGE_METEORIT:
			new_projectile = new projectile(nullptr, position, speed, collider, life, fliped, scale, MAGE_METEORIT, emitter, emitter_offset);
			break;
		case ROGUE_DAGGER:
			new_projectile = new projectile(&warrior_knife, position, speed, collider, life, fliped, scale, ROGUE_DAGGER, emitter, emitter_offset);
			break;
		case PALADIN_HAMMER:
			new_projectile = new projectile(&paladin_hammer, position, speed, collider, life, fliped, scale, PALADIN_HAMMER, emitter, emitter_offset);
			break;
		case ROGUE_ARROW:
			new_projectile = new projectile(&rogue_arrow, position, speed, collider, life, fliped, scale, ROGUE_ARROW, emitter, emitter_offset);
			break;
		case PALADIN_AIR_HAMMER:
			new_projectile = new projectile(&paladin_hammer, position, speed, collider, life, fliped, scale, PALADIN_AIR_HAMMER, emitter, emitter_offset);
			break;

	}	
	new_projectile->angular_velocity = angular_velocity;
	projectiles.push_back(new_projectile);
	return new_projectile;
}

int mdProjectiles::lookForProjectileType(PROJECTILE_TYPE type, Character* character ) {

	int counter = 0;
	for (std::list<projectile*>::iterator it = projectiles.begin(); it != projectiles.end(); ++it) {
		projectile* p = *it;
		if (p->type == type && p->collider->character == character) { 
			counter++;
		}
	}
	return counter;
}

void projectile::update() {

	if (collider != nullptr) {
		collider->SetPos(position.x, position.y);

		if (emitter) {
			emitter->start_pos.x = (float)position.x + collider->rect.w/2 + emitter_offset.x;
			emitter->start_pos.y = (float)position.y + collider->rect.h/2 + emitter_offset.y;
			//App->render->drawQuad(100, { (int)emitter->start_pos.x - 10, (int)emitter->start_pos.y - 10, 20,20 }, 255, 255, 255, 255, true, true);
		}
	}
	if (type == ROGUE_DAGGER || type == PALADIN_HAMMER) {
		speed.y += gravity;
	}

	angle += angular_velocity;

	position += speed;
}
void projectile::draw(SDL_Texture* graphics) {
	if(animation)
		App->render->drawSprite(4, graphics, position.x, position.y, &animation->GetCurrentFrame(),scale, fliped,1.0f,angle); 
}