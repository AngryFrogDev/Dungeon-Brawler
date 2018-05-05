#include "Rogue.h"
#include "mdCollision.h"
#include "mdProjectiles.h"
#include "mdParticleSystem.h"
#include "mdAudio.h"
#include "mdEntities.h"
#include "Player.h"
#include "ParticleEmitter.h"



Rogue::Rogue(character_deff character, int x_pos, bool _fliped, int skin)
{
	idle.PushBack({ 0,0,195,158 });
	idle.PushBack({ 195,0,195,158 });
	idle.PushBack({ 195 * 2,0,195,158 });
	idle.PushBack({ 195 * 3,0,195,158 });
	idle.PushBack({ 195 * 4,0,195,158 });
	idle.PushBack({ 195 * 5,0,195,158 });
	idle.PushBack({ 195 * 6,0,195,158 });

	idle.loop = true;
	idle.speed = 0.2;

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
	type = CHAR_TYPE::ROGUE;

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


Rogue::~Rogue()
{
}
