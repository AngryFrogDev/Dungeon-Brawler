#include "Rogue.h"
#include "mdCollision.h"
#include "mdProjectiles.h"
#include "mdParticleSystem.h"
#include "mdAudio.h"
#include "mdEntities.h"
#include "Player.h"
#include "ParticleEmitter.h"



Rogue::Rogue(character_deff character, int x_pos, bool _fliped, int skin) : Character(character, x_pos, _fliped, skin) {

	int x_space = 195;
	int width = 195;
	int height = 158;


	//MOVEMENT
	for (int i = 0; i < 6; i++) 
		idle.PushBack({ i * x_space,height * 0,width,height });

	idle.loop = true;
	idle.speed = 0.2;

	for (int i = 0; i < 13; i++)
		walk_forward.PushBack({ i * x_space,height * 1,width,height });

	walk_forward.loop = true;
	walk_forward.speed = 0.2;

	for (int i = 12; i > 0; i--)
		walk_back.PushBack({ i * x_space,height * 1 ,width,height });

	walk_back.loop = true;
	walk_back.speed = 0.2;

	for (int i = 0; i < 6; i++)
		light_attack.PushBack({ i * x_space, height * 5,width,height });

	//STANDING
	light_attack.loop = true;
	light_attack.speed = 0.2;

	for (int i = 0; i < 12; i++)
		heavy_attack.PushBack({ i * x_space, height * 4,width,height });

	heavy_attack.loop = true;
	heavy_attack.speed = 0.2;

	//CROUCHING
	for (int i = 0; i < 2; i++)
		crouch.PushBack({ i * x_space, height * 3,width,height });

	crouch.loop = false;
	crouch.speed = 0.2;

	for (int i = 0; i < 4; i++)
		crouching_light.PushBack({ i * x_space, height * 6,width,height });

	crouching_light.loop = false;
	crouching_light.speed = 0.2;

	for (int i = 0; i < 3; i++)
		crouching_heavy.PushBack({ i * x_space, height * 7,width,height });

	crouching_heavy.loop = false;
	crouching_heavy.speed = 0.2;

	for (int i = 0; i < 10; i++)
		jump.PushBack({ i * x_space, height * 2,width,height });

	//JUMPING
	jump.loop = false;
	jump.speed = 0.2;

	for (int i = 0; i < 4; i++)
		jumping_light.PushBack({ i * x_space, height * 9,width,height });

	jumping_light.loop = false;
	jumping_light.speed = 0.2;

	for (int i = 0; i < 3; i++)
		jumping_heavy.PushBack({ i * x_space, height * 8,width,height });

	jumping_heavy.loop = false;
	jumping_heavy.speed = 0.2;

	//HIT

	for (int i = 0; i < 4; i++)
		standing_hit.PushBack({ i * x_space, height * 10,width,height });

	standing_hit.loop = false;
	standing_hit.speed = 0.2;

	for (int i = 0; i < 4; i++)
		knockdown.PushBack({ i * x_space, height * 11,width,height });

	knockdown.loop = false;
	knockdown.speed = 0.2;

	//BLOCK


	standing_block.PushBack({ 10 * x_space, height * 15,width,height });

	standing_block.loop = false;
	standing_block.speed = 0.2;

	for (int i = 0; i < 4; i++)
		crouching_block.PushBack({ i * x_space, height * 10,width,height });

	crouching_block.loop = false;
	crouching_block.speed = 0.2;

	//STANDING SPECIALS

	standing_special1.PushBack({ 0 * x_space, height * 18,width,height });
	standing_special1.PushBack({ 1 * x_space, height * 18,width,height });
	standing_special1.PushBack({ 2 * x_space, height * 18,width,height });
	standing_special1.PushBack({ 3 * x_space, height * 18,width,height },ACTIVE);

	standing_special1.loop = false;
	standing_special1.speed = 0.2;


	type = CHAR_TYPE::ROGUE;
	skin_id = 0;

}

void Rogue::standingSpecial1(const bool(&inputs)[MAX_INPUTS])
{
	if (current_animation->GetState() == ACTIVE) {
		collider* projectile_collider = App->collision->AddCollider({ (int)logic_position.x, (int)logic_position.y, st_s1.hitbox.w,st_s1.hitbox.h }, COLLIDER_TYPE::PROJECTILE_HITBOX, projectile_duration, CHAR_ATT_TYPE::ST_S1, (Module*)App->entities, this);
		hitboxes.push_back(projectile_collider);
		iPoint speed;
		if (!fliped)
			speed.x = projectile_speed;
		else
			speed.x = -projectile_speed;
		speed.y = -30;

		App->projectiles->addProjectile(ROGUE_DAGGER, { calculateDrawPosition(0, st_s1.hitbox.w, true), calculateDrawPosition(0, st_s1.hitbox.h, false) }, speed, projectile_collider, -1, fliped, scale, nullptr);
		askRecovery(st_s1.recovery);
	}
}


Rogue::~Rogue()
{
}
