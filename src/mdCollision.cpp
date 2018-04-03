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
				if (c1->character != c2->character && c1->character->lane == c2->character->lane) {
					c1->character->onCollision(c1,c2);
					c2->character->onCollision(c2, c1);
				}

			}
		}
	}	

	// Check life
	for (std::list<collider*>::iterator it = colliders.begin(); it != colliders.end(); ++it) {
		collider* c = *it;
		if (c->life != -1 && SDL_GetTicks() - c->born > c->life)/*PROVISIONAL: Maybe it should use a timer*/ {
			c->to_delete = true;
		}
	}

	DebugDraw();
	return true;
}





void mdCollision::DebugDraw()
{
	if ((App->input->getKey(SDL_SCANCODE_F1) == KEY_DOWN))
		debug = !debug;

	if (debug == false)
		return;

	Uint8 alpha = 80;
	for (std::list<collider*>::iterator it = colliders.begin(); it != colliders.end(); ++it) {
		collider* c = *it;
		if (!c->active)
			continue;
		switch (c->type)
		{
			case COLLIDER_NONE: // white
				App->render->drawQuad(c->rect, 255, 255, 255, alpha,true);
				break;
			case HITBOX:
				App->render->drawQuad(c->rect, 255, 0, 0, alpha, true);
				break;
			case HURTBOX:
				App->render->drawQuad(c->rect, 0, 0, 255, alpha, true);
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


collider* mdCollision::AddCollider(SDL_Rect rect, COLLIDER_TYPE type,int life,CHAR_ATT_TYPE attack_type, Module* callback, Character* character)
{
	collider* ret = new collider(rect, type, life,attack_type, callback,character);

	colliders.push_back(ret);

	return ret;
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