#include "Application.h"
#include "mdInput.h"
#include "mdRender.h"
#include "mdCollision.h"
#include "DebLog.h"

mdCollision::mdCollision() {

	collider_one = AddCollider({ 100,100,100,100 }, COLLIDER_NONE, this);
		
	collider_two = AddCollider({ 500,100,100,100 }, COLLIDER_NONE, this);

}

// Destructor
mdCollision::~mdCollision(){
}

bool mdCollision::preUpdate()
{
	//// Remove all colliders scheduled for deletion
	for (std::list<collider*>::iterator it = colliders.begin(); it != colliders.end();++it) {
		collider* c = *it;

		if (c->to_delete) {
			colliders.remove(c);
			delete c;
		}
	}

	return true;
}

bool mdCollision::update(float dt)
{
	collider* c1;
	collider* c2;


	for (std::list<collider*>::iterator it1 = colliders.begin(); it1 != colliders.end(); ++it1) {


		auto tmp = it1;
		tmp++;
		for (std::list<collider*>::iterator it2 = tmp; it2 != colliders.end(); ++it2) {
			c1 = *it1;
			c2 = *it2;

			if (c1->checkCollision(c2->rect) == true) {

				if (true)
					c1->callback->onCollision(c1, c2);
				if (true)
					c2->callback->onCollision(c2, c1);
			}
		}
	}	
	// PROVISIONAL: Test
	if (App->input->getKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) {
		collider_one->rect.x+= 5;
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
		switch (c->type)
		{
			case COLLIDER_NONE: // white
				App->render->drawQuad(c->rect, 255, 255, 255, alpha,true);
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
		colliders.remove(c);
		delete c;

	}

	return true;
}

void mdCollision::onCollision(collider*c1, collider* c2) 	{

	LOG("COLLISION!");
}


collider* mdCollision::AddCollider(SDL_Rect rect, COLLIDER_TYPE type, Module* callback)
{
	collider* ret = new collider(rect, type, callback);

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