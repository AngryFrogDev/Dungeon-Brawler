#include "Paladin.h"
#include "mdCollision.h"
#include "mdProjectiles.h"
#include "mdParticleSystem.h"
#include "mdAudio.h"
#include "mdEntities.h"
#include "Player.h"
#include "ParticleEmitter.h"


Paladin::Paladin(character_deff character, int x_pos, bool _fliped, int skin) {

	skin_id = skin;

	idle.PushBack({ 0,0,195,158 });
	idle.PushBack({ 195,0,195,158 });
	idle.PushBack({ 195 * 2,0,195,158 });
	idle.PushBack({ 195 * 3,0,195,158 });

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

	walk_forward.loop = true;
	walk_forward.speed = 0.2;

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

	light_attack.PushBack({ 0,158 * 13,195, 158 });
	light_attack.PushBack({ 195,158 * 13,195, 158 });
	light_attack.PushBack({ 195 * 2,158 * 13,195, 158 });
	light_attack.PushBack({ 195 * 3,158 * 13,195, 158 }, ACTIVE);
	light_attack.PushBack({ 195 * 4,158 * 13,195, 158 });
	light_attack.PushBack({ 195 * 5,158 * 13,195, 158 });

	light_attack.loop = false;
	light_attack.speed = character.st_l.animation_speed;

	heavy_attack.PushBack({ 0,158 * 12,195, 158 });
	heavy_attack.PushBack({ 195,158 * 12,195, 158 });
	heavy_attack.PushBack({ 195 * 2,158 * 12,195, 158 });
	heavy_attack.PushBack({ 195 * 3,158 * 12,195, 158 });
	heavy_attack.PushBack({ 195 * 4,158 * 12,195, 158 });
	heavy_attack.PushBack({ 195 * 5,158 * 12,195, 158 });
	heavy_attack.PushBack({ 195 * 6,158 * 12,195, 158 });
	heavy_attack.PushBack({ 195 * 7,158 * 12,195, 158 }, ACTIVE);
	heavy_attack.PushBack({ 195 * 8,158 * 12,195, 158 });
	heavy_attack.PushBack({ 195 * 9,158 * 12,195, 158 });

	heavy_attack.loop = false;
	heavy_attack.speed = character.st_h.animation_speed;

	crouch.PushBack({ 195 * 2	,158 * 19,195,158 });

	crouch.loop = false;
	crouch.speed = 0.2;

	crouching_light.PushBack({ 0,158 * 20,195,158 });
	crouching_light.PushBack({ 195,158 * 20,195,158 });
	crouching_light.PushBack({ 195 * 2,158 * 20,195,158 });
	crouching_light.PushBack({ 195 * 3,158 * 20,195,158 });
	crouching_light.PushBack({ 195 * 4,158 * 20,195,158 }, ACTIVE);
	crouching_light.PushBack({ 195 * 5,158 * 20,195,158 });
	crouching_light.PushBack({ 195 * 6,158 * 20,195,158 });

	crouching_light.loop = false;
	crouching_light.speed = character.cr_l.animation_speed;

	crouching_heavy.PushBack({ 0,158 * 8,195,158 });
	crouching_heavy.PushBack({ 195,158 * 8,195,158 });
	crouching_heavy.PushBack({ 195 * 2,158 * 8,195,158 });
	crouching_heavy.PushBack({ 195 * 3,158 * 8,195,158 });
	crouching_heavy.PushBack({ 195 * 4,158 * 8,195,158 }, ACTIVE);
	crouching_heavy.PushBack({ 195 * 5,158 * 8,195,158 });
	crouching_heavy.PushBack({ 195 * 6,158 * 8 ,195,158 });

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

	jump.loop = false;
	jump.speed = 0.2;

	jumping_light.PushBack({ 0      ,158 * 15,195,158 });
	jumping_light.PushBack({ 195    ,158 * 15,195,158 });
	jumping_light.PushBack({ 195 * 2,158 * 15,195,158 }, ACTIVE);

	jumping_light.loop = false;
	jumping_light.speed = character.jm_l.animation_speed;

	jumping_heavy.PushBack({ 0      ,158 * 16,195,158 });
	jumping_heavy.PushBack({ 195    ,158 * 16,195,158 }, ACTIVE);

	jumping_heavy.loop = false;
	jumping_heavy.speed = character.jm_h.animation_speed;

	standing_hit.PushBack({ 0, 158 * 14, 195, 158 });
	standing_hit.PushBack({ 195, 158 * 14, 195, 158 });

	standing_hit.loop = true;
	standing_hit.speed = 0.2;

	standing_block.PushBack({ 0, 158 * 23, 195, 158 });

	standing_block.loop = false;
	standing_block.speed = 0.2;

	crouching_block.PushBack({ 195, 158 * 23, 195, 158 });

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
	dead.loop = false;
	dead.speed = 0.1;



	// XML inicialization
	draw_size.x = 195;
	draw_size.y = 158;
	max_life = character.max_life;
	max_super_gauge = character.max_super_gauge;
	super_window = character.super_window;
	cancelability_window = character.cancelability_window;
	super_gauge_gain_hit = character.super_gauge_gain_hit;
	super_gauge_gain_block = character.super_gauge_gain_block;
	super_gauge_gain_strike = character.super_gauge_gain_strike;
	crouching_hurtbox_offset = character.crouching_hurtbox_offset;
	standing_hurtbox_size = character.standing_hurtbox_size;
	jump_power = character.jump_power;
	walk_speed = character.walk_speed;
	gravity = character.gravity;
	invencibility_on_wakeup = character.invencibility_on_wakeup;
	scale = character.scale;
	shadow_rect = { 452, 3719, 68, 14 };
	shadow_offset = 105;
	// Constructor inicialization
	fliped = _fliped;
	logic_position.x = x_pos;
	type = CHAR_TYPE::PALADIN;


	//PALADIN EXCLUSIVE BARS
	// XML inicialization
	parry_reaction = light_attack;
	parry_reaction.speed = character.cr_s2.animation_speed;
	parry_duration = 400;
	miss_parry_recovery = 500;
	parry_healing = 20;

	parry_reaction_invencivility = 200;
	// Runtime inicialization
	parry_start = 0;
	parry_reacting = false;
	// Basic attack definitions

	st_l = character.st_l;
	st_h = character.st_h;
	cr_l = character.cr_l;
	cr_h = character.cr_h;
	jm_l = character.jm_l;
	jm_h = character.jm_h;
	st_s1 = character.st_s1;
	st_s2 = character.st_s2;
	cr_s1 = character.cr_s1;
	cr_s2 = character.cr_s2;
	jm_s1 = character.jm_s1;
	jm_s2 = character.jm_s2;
	super = character.super;


	// Runtime inicialization
	grounded = false;
	instanciated_hitbox = false;
	hit = false;
	crouching_hurtbox = false;
	death = false;
	current_life = max_life;
	current_super_gauge = 0;
	velocity.y = 0;
	velocity.x = 0;
	current_state = CHAR_STATE::IDLE;
	logic_position.y = 1000;
	starting_position.x = logic_position.x;
	starting_position.y = -1000;
	state_first_tick = false;
	// Others
	ground_position = 800;
	lateral_limit = 50;



	current_animation = &idle;

	// Collider creation
	hurtbox = App->collision->AddCollider({ 0, 0, standing_hurtbox_size.x, standing_hurtbox_size.y }, HURTBOX, -1, CHAR_ATT_TYPE::NO_ATT, (Module*)App->entities, (Character*)this);
	pushbox = App->collision->AddCollider({ 0, 0, standing_hurtbox_size.x, standing_hurtbox_size.y / 2 }, PUSHBOX, -1, CHAR_ATT_TYPE::NO_ATT, (Module*)App->entities, (Character*)this);
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
			instanciateHitbox(CR_S2);
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