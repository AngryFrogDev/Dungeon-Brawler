#include "Paladin.h"
#include "mdCollision.h"
#include "mdProjectiles.h"
#include "mdParticleSystem.h"
#include "mdAudio.h"
#include "mdEntities.h"
#include "Player.h"
#include "ParticleEmitter.h"


Paladin::Paladin(character_deff character, int x_pos, bool _fliped, int skin): Character(character, x_pos, _fliped, skin) {

	idle.PushBack({ 0,0,195,158 });
	idle.PushBack({ 195,0,195,158 });
	idle.PushBack({ 195 * 2,0,195,158 });
	idle.PushBack({ 195 * 3,0,195,158 });
	idle.PushBack({ 195 * 4,0,195,158 });

	idle.loop = true;
	idle.speed = 0.2;

	walk_forward.PushBack({ 0,158,195,158 });
	walk_forward.PushBack({ 195,158,195,158 });
	walk_forward.PushBack({ 195 * 2,158,195,158 });
	walk_forward.PushBack({ 195 * 3,158,195,158 });
	walk_forward.PushBack({ 195 * 4,158,195,158 });
	walk_forward.PushBack({ 195 * 5,158,195,158 });
	walk_forward.PushBack({ 195 * 6,158,195,158 });
	walk_forward.PushBack({ 195 * 7,158,195,158 });
	walk_forward.PushBack({ 195 * 8,158,195,158 });
	walk_forward.PushBack({ 195 * 9,158,195,158 });
	walk_forward.PushBack({ 195 * 10,158,195,158 });
	walk_forward.PushBack({ 195 * 11,158,195,158 });
	walk_forward.PushBack({ 195 * 12,158,195,158 });

	walk_forward.loop = true;
	walk_forward.speed = 0.2;

	walk_back.PushBack({ 195 * 12,158,195,158 });
	walk_back.PushBack({ 195 * 11,158,195,158 });
	walk_back.PushBack({ 195 * 10,158,195,158 });
	walk_back.PushBack({ 195 * 9,158,195,158 });
	walk_back.PushBack({ 195 * 8,158,195,158 });
	walk_back.PushBack({ 195 * 7,158,195,158 });
	walk_back.PushBack({ 195 * 6,158,195,158 });
	walk_back.PushBack({ 195 * 5,158,195,158 });
	walk_back.PushBack({ 195 * 4,158,195,158 });
	walk_back.PushBack({ 195 * 3,158,195,158 });
	walk_back.PushBack({ 195 * 2,158,195,158 });
	walk_back.PushBack({ 195,158,195,158 });
	walk_back.PushBack({ 0,158,195,158 });

	walk_back.loop = true;
	walk_back.speed = 0.2;

	light_attack.PushBack({ 0,158 * 3,195, 158 });
	light_attack.PushBack({ 195,158 * 3,195, 158 });
	light_attack.PushBack({ 195 * 2,158 * 3,195, 158 });
	light_attack.PushBack({ 195 * 3,158 * 3,195, 158 });
	light_attack.PushBack({ 195 * 4,158 * 3,195, 158 });
	light_attack.PushBack({ 195 * 5,158 * 3,195, 158 }, ACTIVE);

	light_attack.loop = false;
	light_attack.speed = character.st_l.animation_speed;

	heavy_attack.PushBack({ 0,158 * 4,195, 158 });
	heavy_attack.PushBack({ 195,158 * 4,195, 158 });
	heavy_attack.PushBack({ 195 * 2,158 * 4,195, 158 });
	heavy_attack.PushBack({ 195 * 3,158 * 4,195, 158 });
	heavy_attack.PushBack({ 195 * 4,158 * 4,195, 158 }, ACTIVE);


	heavy_attack.loop = false;
	heavy_attack.speed = character.st_h.animation_speed;

	crouch.PushBack({ 195 	,158 * 5,195,158 });

	crouch.loop = false;
	crouch.speed = 0.2;

	crouching_light.PushBack({ 0,158 * 16,195,158 });
	crouching_light.PushBack({ 195,158 * 16,195,158 });
	crouching_light.PushBack({ 195 * 2,158 * 16,195,158 }, ACTIVE);

	crouching_light.loop = false;
	crouching_light.speed = character.cr_l.animation_speed;

	crouching_heavy.PushBack({ 0,158 * 9,195,158 });
	crouching_heavy.PushBack({ 195,158 * 9,195,158 });
	crouching_heavy.PushBack({ 195 * 2,158 * 9,195,158 });
	crouching_heavy.PushBack({ 195 * 3,158 * 9,195,158 }, ACTIVE);


	crouching_heavy.loop = false;
	crouching_heavy.speed = character.cr_h.animation_speed;

	jump.PushBack({ 0, 158 * 2, 195, 158 });
	jump.PushBack({ 195, 158 * 2, 195, 158 });
	jump.PushBack({ 195 * 2, 158 * 2, 195, 158 });
	jump.PushBack({ 195 * 3, 158 * 2, 195, 158 });
	jump.PushBack({ 195 * 4, 158 * 2, 195, 158 });
	jump.PushBack({ 195 * 5, 158 * 2, 195, 158 });
	jump.PushBack({ 195 * 6, 158 * 2, 195, 158 });
	jump.PushBack({ 195 * 7, 158 * 2, 195, 158 });
	jump.PushBack({ 195 * 8, 158 * 2, 195, 158 });
	jump.PushBack({ 195 * 9, 158 * 2, 195, 158 });
	jump.PushBack({ 195 * 10, 158 * 2, 195, 158 });
	jump.PushBack({ 195 * 11, 158 * 2, 195, 158 });

	jump.loop = false;
	jump.speed = 0.2;

	jumping_light.PushBack({ 0      ,158 * 8,195,158 });
	jumping_light.PushBack({ 195    ,158 * 8,195,158 }, ACTIVE);
	jumping_light.PushBack({ 195 * 2,158 * 8,195,158 });

	jumping_light.loop = false;
	jumping_light.speed = character.jm_l.animation_speed;

	jumping_heavy.PushBack({ 0      ,158 * 7,195,158 });
	jumping_heavy.PushBack({ 195    ,158 * 7,195,158 }, ACTIVE);

	jumping_heavy.loop = false;
	jumping_heavy.speed = character.jm_h.animation_speed;

	standing_hit.PushBack({ 0, 158 * 17, 195, 158 }); 
	standing_hit.PushBack({ 195, 158 * 17, 195, 158 });
	standing_hit.PushBack({ 195 * 2, 158 * 17, 195, 158 });

	standing_hit.loop = true; 
	standing_hit.speed = 0.2;

	standing_block.PushBack({ 195 * 2, 158 * 3, 195, 158 });

	standing_block.loop = false;
	standing_block.speed = 0.2;

	crouching_block.PushBack({ 0, 158 * 6, 195, 158 });

	crouching_block.loop = false;
	crouching_block.speed = 0.2;

	knockdown.PushBack({ 0      , 158 * 21, 195, 158 });
	knockdown.PushBack({ 0      , 158 * 21, 195, 158 });
	knockdown.PushBack({ 0      , 158 * 21, 195, 158 });
	knockdown.PushBack({ 0      , 158 * 21, 195, 158 });
	knockdown.PushBack({ 0      , 158 * 21, 195, 158 }); // It works for me
	knockdown.PushBack({ 195    , 158 * 21, 195, 158 });
	knockdown.PushBack({ 195 * 2, 158 * 21, 195, 158 });
	knockdown.PushBack({ 195 * 3, 158 * 21, 195, 158 });

	knockdown.loop = false;
	knockdown.speed = 0.1;

	dead.PushBack({ 0      , 158 * 21, 195, 158 });

	dead.loop = true;
	dead.speed = 0.1;

	crouching_special2.PushBack({ 195 * 3, 158 * 10, 195, 158 });

	crouching_special1.PushBack({ 195 * 0, 158 * 15, 195, 158 });
	crouching_special1.PushBack({ 195 * 1, 158 * 15, 195, 158 }, ACTIVE);

	crouching_special1.loop = false;
	crouching_special1.speed = character.cr_s1.animation_speed;


	standing_special1.PushBack({ 0		, 158 * 10, 195, 158 });
	standing_special1.PushBack({ 195    , 158 * 10, 195, 158 });
	standing_special1.PushBack({ 195 * 2, 158 * 10, 195, 158 });
	standing_special1.PushBack({ 195 * 3, 158 * 10, 195, 158 });
	standing_special1.PushBack({ 195 * 4, 158 * 10, 195, 158 }, ACTIVE);

	standing_special1.loop = false;
	standing_special1.speed = character.st_s1.animation_speed;

	standing_special2.PushBack({ 0		, 158 * 11, 195, 158 });
	standing_special2.PushBack({ 195		, 158 * 11, 195, 158 });
	standing_special2.PushBack({ 195 * 2		, 158 * 11, 195, 158 }, ACTIVE);
	standing_special2.PushBack({ 195 * 3		, 158 * 11, 195, 158 });
	standing_special2.PushBack({ 195 * 4		, 158 * 11, 195, 158 });
	standing_special2.PushBack({ 195 * 5		, 158 * 11, 195, 158 });


	standing_special2.loop = false;
	standing_special2.speed = character.st_s2.animation_speed;

	jumping_special2.PushBack({0        , 158 * 12, 195, 158 }, ACTIVE);
	jumping_special2.PushBack({195 , 158 * 12, 195, 158 }, ACTIVE);

	jumping_special2.loop = false;
	jumping_special2.speed = character.jm_s1.animation_speed;

	super_anim.PushBack({ 0		, 158 * 14, 195, 158 });
	super_anim.PushBack({ 195		, 158 * 14, 195, 158 });
	super_anim.PushBack({ 195 * 2		, 158 * 14, 195, 158 });
	super_anim.PushBack({ 195 * 3		, 158 * 14, 195, 158 });
	super_anim.PushBack({ 195 * 4		, 158 * 14, 195, 158 });
	super_anim.PushBack({ 195 * 5		, 158 * 14, 195, 158 }, ACTIVE);


	super_anim.loop = false;
	super_anim.speed = character.super.animation_speed;


	//PALADIN EXCLUSIVE BARS
	// XML inicialization
	parry_reaction = light_attack;
	parry_reaction.speed = character.cr_s2.animation_speed;
	parry_duration = character.parry_duration;
	miss_parry_recovery = character.miss_parry_recovery;
	parry_healing = character.parry_healing;

	parry_reaction_invencivility = character.parry_reaction_invencivility;

	st_s2_speed = character.st_s2_speed;
	st_s2_invencivility = character.st_s2_invencivility;

	hammer_duration = character.hammer_duration;
	hammer_speed = character.hammer_speed;

	slam_speed = character.slam_speed;
	slam_max_height = character.slam_max_height;

	air_hammer_speed = character.air_hammer_speed;
	air_hammer_duration = character.air_hammer_duration;

	super_healing = 50;
	
	// Runtime inicialization
	parry_start = 0;
	parry_reacting = false;
}


Paladin::~Paladin() {
}

void Paladin::crouchingSpecial2() {
	if (!state_first_tick) {
		updateAnimation(crouching_special2);
		state_first_tick = true;
		parry_start = SDL_GetTicks();
	}
	if (parry_duration < (SDL_GetTicks() - parry_start) && parry_reacting == false) {
		askRecovery(miss_parry_recovery);
	}

	if (parry_reacting) {
		if (current_animation->GetState() == ACTIVE && !instanciated_hitbox)
			instanciateHitbox(cr_s2);
		if (current_animation->Finished()){
			parry_reacting = false;
			instanciated_hitbox = false;
			askRecovery(cr_s2.recovery);
		}
	}
	if (hit) {
		hit = false;
		current_life += parry_healing; // Maybe this should check that life is not exceded beyond the maximum
		if (current_life > max_life)
			current_life = max_life;
		updateAnimation(parry_reaction);
		makeInvencibleFor(parry_reaction_invencivility);
		parry_reacting = true;
	}
}

void Paladin::crouchingSpecial1() {
	if (current_animation->Finished()) {
		askRecovery(cr_s1.recovery);
		instanciated_hitbox = false;
	}
	else if (current_animation->GetState() == ACTIVE && !instanciated_hitbox) {
		instanciateHitbox(cr_s1);
	}
}

void Paladin::standingSpecial2(const bool(&inputs)[MAX_INPUTS]) {
	if (!state_first_tick) {
		updateAnimation(standing_special2);
		state_first_tick = true;
		makeInvencibleFor(st_s2_invencivility);
	}

	if (!fliped)
		logic_position.x += st_s2_speed;
	else
		logic_position.x -= st_s2_speed;

	if (current_animation->Finished()) {
		instanciated_hitbox = false;
		collider* hitbox = getCurrentAttackHitbox();
		if (hitbox != nullptr) { // Just for safety
			deleteAttackHitbox(ST_S2);
		}
		askRecovery(st_s2.recovery);
		hurtbox->rect.h = standing_hurtbox_size.y;
	}
	else if (current_animation->GetState() == ACTIVE && !instanciated_hitbox)
		instanciateHitbox(st_s2);

	collider* hitbox = getCurrentAttackHitbox();
	if (hitbox != nullptr)
		hitbox->SetPos(calculateDrawPosition(st_s2.pos_rel_char.x, st_s2.hitbox.w, true), calculateDrawPosition(st_s2.pos_rel_char.y, st_s2.hitbox.h, false));
}

void Paladin::standingSpecial1(const bool(&inputs)[MAX_INPUTS]) {
	if (current_animation->GetState() == ACTIVE) {
		collider* projectile_collider = App->collision->AddCollider({ (int)logic_position.x, (int)logic_position.y, st_s1.hitbox.w,st_s1.hitbox.h }, COLLIDER_TYPE::PROJECTILE_HITBOX, hammer_duration, st_s1, this);
		hitboxes.push_back(projectile_collider);
		iPoint speed;
		if (!fliped)
			speed.x = hammer_speed.x;
		else
			speed.x = -hammer_speed.x;
		speed.y = hammer_speed.y;

		App->projectiles->addProjectile(PALADIN_HAMMER, { calculateDrawPosition(0, st_s1.hitbox.w, true), calculateDrawPosition(0, st_s1.hitbox.h, false) }, speed, projectile_collider, -1, fliped, scale, nullptr, { 0,0 }, 20.0f);
		askRecovery(st_s1.recovery);
	}
}

void Paladin::jumpingSpecial2(const bool(&inputs)[MAX_INPUTS]) {
	if (!state_first_tick) {
		updateAnimation(jumping_special2);
		state_first_tick = true;
	}

	if (!grounded) {
		if (!fliped) {
			velocity.x = slam_speed.x;
		}
		else {
			velocity.x = -slam_speed.x;
		}
		velocity.y = slam_speed.y;
	}
	else{
		instanciateHitbox(jm_s2);
		instanciated_hitbox = false;
		askRecovery(jm_s2.recovery);
		App->particle_system->createEmitter({ (float)logic_position.x,(float)logic_position.y +150 }, "particles/dust-explosion.xml");
		App->particle_system->createEmitter({ (float)logic_position.x,(float)logic_position.y +100 }, "particles/dust-cloud.xml");
		App->particle_system->createEmitter({ (float)logic_position.x,(float)logic_position.y + 100 }, "particles/dust-cloud-front.xml");
	}


}

void Paladin::jumpingSpecial1(const bool(&inputs)[MAX_INPUTS]) {
	if (!state_first_tick) {
		updateAnimation(standing_special1);
		state_first_tick = true;
	}
	if (current_animation->GetState() == ACTIVE && !instanciated_hitbox) {
		collider* projectile_collider = App->collision->AddCollider({ (int)logic_position.x, (int)logic_position.y, jm_s1.hitbox.w, jm_s1.hitbox.h }, COLLIDER_TYPE::PROJECTILE_HITBOX, air_hammer_duration, jm_s1, this);
		hitboxes.push_back(projectile_collider);
		iPoint speed = { 0,0 };
		iPoint offset = { 0,0 };
		if (!fliped) {
			speed.x = air_hammer_speed;
			offset.x = jm_s1.pos_rel_char.x;
		}
		else {
			speed.x = -air_hammer_speed;
			offset.x = -jm_s1.pos_rel_char.x;
		}
		offset.y = jm_s1.pos_rel_char.y;
		ParticleEmitter* light_ball = App->particle_system->createEmitter({ (float)logic_position.x,(float)logic_position.y + 150 }, "particles/ligh-ball.xml");
		App->projectiles->addProjectile(PALADIN_AIR_HAMMER, { calculateDrawPosition(0,jm_s1.hitbox.w,true) + offset.x, calculateDrawPosition(0,jm_s1.hitbox.h,false) + offset.y }, speed, projectile_collider, -1, fliped, scale, light_ball, { 0,0 }, 30.0f);
		instanciated_hitbox = true;
	}

	if (grounded) {
		instanciated_hitbox = false;
		askRecovery(jm_s1.recovery);
	}
}

void Paladin::doSuper() {
	if (!state_first_tick) {
		updateAnimation(super_anim);
		state_first_tick = true;
		current_super_gauge = 0;
	}
	if (current_animation->Finished()) {
		updateState(IDLE);
		current_life += super_healing;
		if (current_life > max_life)
			current_life = max_life;
	}
}
bool Paladin::standingSpecial1Condition() {
	return App->projectiles->lookForProjectileType(PALADIN_HAMMER, (Character*)this) == 0;
}

bool Paladin::jumpingSpecial2Condition() {
	return logic_position.y <= slam_max_height;
}

bool Paladin::jumpingSpecial1Condition() {
	return App->projectiles->lookForProjectileType(PALADIN_AIR_HAMMER, (Character*)this) == 0;
}