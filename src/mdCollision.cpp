#include "Application.h"
#include "mdInput.h"
#include "mdRender.h"
#include "mdCollision.h"
#include "DebLog.h"

mdCollision::mdCollision() {


}

// Destructor
mdCollision::~mdCollision(){
}

bool mdCollision::awake(const pugi::xml_node& md_config) {
	// PROVISIONAL: Should be loaded from xml
	hurtbox.push_back(HITBOX);
	hurtbox.push_back(PROJECTILE_HITBOX);
	
	projectile_invencible_hurtbox.push_back(HITBOX);

	pushbox.push_back(PUSHBOX);

	hitbox.push_back(HURTBOX);
	hitbox.push_back(PROJECTILE_HITBOX);

	projectile_hitbox.push_back(HURTBOX);

	debug = false;
	return true;
}
bool mdCollision::preUpdate()
{
	std::list<collider*> colliders_to_delete;
	// Remove all colliders scheduled for deletion

	//Iterate list looking for the colliders
	for (std::list<collider*>::iterator it = colliders.begin(); it != colliders.end();++it) {
		collider* c = *it;
		if (c->to_delete) {
			colliders_to_delete.push_back(c);
		}
	}

	// Remove the colliders
	for (std::list<collider*>::iterator it = colliders_to_delete.begin(); it != colliders_to_delete.end(); ++it) {
		collider* c = *it;
		colliders.remove(c);
		delete c;
	}

	colliders_to_delete.clear();

	return true;
}

bool mdCollision::update(float dt)
{
	collider* c1;
	collider* c2;

	// Check collision
	for (std::list<collider*>::iterator it1 = colliders.begin(); it1 != colliders.end(); ++it1) {
		auto tmp = it1;
		tmp++;
		for (std::list<collider*>::iterator it2 = tmp; it2 != colliders.end(); ++it2) {
			c1 = *it1;
			c2 = *it2;
			
			if (!c1->active || !c2->active)
				continue;

			if (c1->checkCollision(c2->rect) == true) {
				//Character check
				if (c1->character != c2->character/* && c1->character->lane == c2->character->lane*/) {
					if(isInteractive(c1->type,c2->type))
						c1->character->onCollision(c1,c2);
					if(isInteractive(c2->type, c1->type))
						c2->character->onCollision(c2, c1);
				}

			}
		}
	}	
	DebugDraw();
	return true;
}





void mdCollision::DebugDraw()
{
	if ((App->input->getKey(SDL_SCANCODE_F1) == KEY_DOWN))
		debug = !debug;

	if (!debug)
		return;

	Uint8 alpha = 80;
	for (std::list<collider*>::iterator it = colliders.begin(); it != colliders.end(); ++it) {
		collider* c = *it;
		if (!c->active)
			continue;
		switch (c->type)
		{
			case COLLIDER_NONE: // white
				App->render->drawQuad( 1, c->rect, 255, 255, 255, alpha,true,true);
				break;
			case HITBOX:
				App->render->drawQuad( 1, c->rect, 255, 0, 0, alpha, true, true);
				break;
			case HURTBOX:
				App->render->drawQuad( 1, c->rect, 0, 0, 255, alpha, true, true);
				break;
			case PUSHBOX:
				App->render->drawQuad( 1, c->rect, 255, 127, 80, alpha, true, true);
				break;
			case PROJECTILE_HITBOX:
				App->render->drawQuad( 1, c->rect, 255, 255, 255, alpha, true, true);
				break;
			case PROJECTILE_INVENCIBLE_HURTBOX:
				App->render->drawQuad( 1, c->rect, 0, 255, 0, alpha, true, true);
				break;
		}
	}
}

// Called before quitting
bool mdCollision::cleanUp()
{
	LOG("Freeing all colliders");

	for (std::list<collider*>::iterator it = colliders.begin(); it != colliders.end(); ++it) {
		collider* c = *it;
		delete c;
	}

	colliders.clear();
	return true;
}

void mdCollision::onCollision(collider*c1, collider* c2) {

	LOG("COLLISION!");
}


collider* mdCollision::AddCollider(SDL_Rect rect, COLLIDER_TYPE type, int life, basic_attack_deff attack_type, Character* character)
{
	collider* ret = new collider(rect, type, life,attack_type,character);

	colliders.push_back(ret);

	return ret;
}
bool mdCollision::isInteractive(COLLIDER_TYPE type1, COLLIDER_TYPE type2) {
	switch (type1) {
	case HURTBOX:
		return lookForColliderType(&hurtbox, type2);
		break;
	case PROJECTILE_INVENCIBLE_HURTBOX:
		return lookForColliderType(&projectile_invencible_hurtbox, type2);
		break;
	case HITBOX:
		return lookForColliderType(&hitbox, type2);
		break;
	case PUSHBOX:
		return lookForColliderType(&pushbox, type2);
		break;
	case PROJECTILE_HITBOX:
		return lookForColliderType(&projectile_hitbox, type2);
		break;
	}
}

bool mdCollision::lookForColliderType(std::list<COLLIDER_TYPE>* list, COLLIDER_TYPE type) {
	for (std::list<COLLIDER_TYPE>::iterator it = list->begin(); it != list->end(); ++it) {
		COLLIDER_TYPE evaluated = *it;
		if (evaluated == type)
			return true;
	}
	return false;
}
// -----------------------------------------------------

bool collider::checkCollision(const SDL_Rect& r) const
{
	if (r.y + r.h > rect.y && r.y < rect.y + rect.h && r.x + r.w > rect.x && r.x < rect.x + rect.w)
	{
		return true;
	}

	else
	{
		return false;
	}
}
