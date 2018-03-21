#include "Application.h"
#include "mdInput.h"
#include "mdRender.h"
#include "mdCollision.h"
#include "DebLog.h"

mdCollision::mdCollision()
{
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
		colliders[i] = nullptr;

	AddCollider({ 100,100,100,100 }, COLLIDER_NONE, this);
		

}

// Destructor
mdCollision::~mdCollision()
{}

bool mdCollision::preUpdate()
{
	// Remove all colliders scheduled for deletion
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr && colliders[i]->to_delete == true)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}

	return true;
}

bool mdCollision::update(float dt)
{
	collider* c1;
	collider* c2;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		// skip empty colliders
		if (colliders[i] == nullptr)
			continue;

		c1 = colliders[i];

		// avoid checking collisions already checked
		for (uint k = i + 1; k < MAX_COLLIDERS; ++k)
		{
			// skip empty colliders
			if (colliders[k] == nullptr)
				continue;

			c2 = colliders[k];

			if (c1->checkCollision(c2->rect) == true)
			{
			
				if (matrix[c1->type][c2->type] && c1->callback)
					c1->callback->onCollision(c1, c2);
			
			
				if (matrix[c2->type][c1->type] && c2->callback)
						c2->callback->onCollision(c2, c1);
			
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

	if (debug == false)
		return;

	Uint8 alpha = 80;
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
			continue;

		switch (colliders[i]->type)
		{
		case COLLIDER_NONE: // white
			App->render->drawQuad(colliders[i]->rect, 255, 255, 255, alpha,true);
			break;
		
		}
	}
}

// Called before quitting
bool mdCollision::cleanUp()
{
	LOG("Freeing all colliders");

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}

	return true;
}

collider* mdCollision::AddCollider(SDL_Rect rect, COLLIDER_TYPE type, Module* callback)
{
	collider* ret = nullptr;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
		{
			ret = colliders[i] = new collider(rect, type, callback);
			break;
		}
	}

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