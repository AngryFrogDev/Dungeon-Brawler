#include "Mage.h"
#include "mdCollision.h"
#include "mdProjectiles.h"
#include "mdParticleSystem.h"
#include "mdAudio.h"
#include "mdEntities.h"


Mage::Mage(character_deff character, int x_pos, bool _fliped, int lane) : Character() {

	this->lane = lane;

	//PROVISIONAL: Animations should be loaded from the xml

	//Animations
	idle.PushBack({ 0,0,195,158 });
	idle.PushBack({ 195,0,195,158 });
	idle.PushBack({ 195 * 2,0,195,158 });
	idle.PushBack({ 195 * 3,0,195,158 });
	idle.PushBack({ 195 * 4,0,195,158 });
	idle.PushBack({ 195 * 5,0,195,158 });
	idle.PushBack({ 195 * 6,0,195,158 });

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

	crouch.PushBack({ 195 * 1	,158 * 4,195,158 });
	crouch.PushBack({ 195 * 2	,158 * 4,195,158 });

	crouch.loop = false;
	crouch.speed = 0.2;

	light_attack.PushBack({ 0,158 * 10,195, 158 });
	light_attack.PushBack({ 195,158 * 10,195, 158 });
	light_attack.PushBack({ 195 * 2,158 * 10,195, 158 });
	light_attack.PushBack({ 195 * 3,158 * 10,195, 158 }, ACTIVE);
	light_attack.PushBack({ 195 * 4,158 * 10,195, 158 });
	light_attack.PushBack({ 195 * 5,158 * 10,195, 158 });

	light_attack.loop = false;
	light_attack.speed = character.st_l.animation_speed;


	crouching_light.PushBack({ 0,158 * 12,195, 158 });
	crouching_light.PushBack({ 195,158 * 12,195, 158 });
	crouching_light.PushBack({ 195 * 2,158 * 12,195, 158 }, ACTIVE);
	crouching_light.PushBack({ 195 * 3,158 * 12,195, 158 });

	crouching_light.loop = false;
	crouching_light.speed = character.cr_l.animation_speed;


	heavy_attack.PushBack({ 0,158 * 3,195, 158 });
	heavy_attack.PushBack({ 195,158 * 3,195, 158 });
	heavy_attack.PushBack({ 195 * 2,158 * 3,195, 158 });
	heavy_attack.PushBack({ 195 * 3,158 * 3,195, 158 }, ACTIVE);
	heavy_attack.PushBack({ 195 * 4,158 * 3,195, 158 });
	heavy_attack.PushBack({ 195 * 5,158 * 3,195, 158 });
	heavy_attack.PushBack({ 195 * 6,158 * 3,195, 158 });
	heavy_attack.PushBack({ 195 * 7,158 * 3,195, 158 });

	heavy_attack.loop = false;
	heavy_attack.speed = character.st_h.animation_speed; //Maybe this attack is too powerfull and could be crouched

	crouching_heavy.PushBack({ 0,158 * 5,195,158 });
	crouching_heavy.PushBack({ 195,158 * 5,195,158 });
	crouching_heavy.PushBack({ 195 * 2,158 * 5,195,158 });
	crouching_heavy.PushBack({ 195 * 3,158 * 5,195,158 });
	crouching_heavy.PushBack({ 195 * 4,158 * 5,195,158 }, ACTIVE);
	crouching_heavy.PushBack({ 195 * 5,158 * 5,195,158 });
	crouching_heavy.PushBack({ 195 * 6,158 * 5,195,158 });
	crouching_heavy.PushBack({ 195 * 7,158 * 5,195,158 });
	crouching_heavy.PushBack({ 195 * 8,158 * 5,195,158 });

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

	jump.loop = false;
	jump.speed = 0.2;

	jumping_light.PushBack({ 0      ,158 * 9,195,158 });
	jumping_light.PushBack({ 195    ,158 * 9,195,158 });
	jumping_light.PushBack({ 195 * 2,158 * 9,195,158 }, ACTIVE);

	jumping_light.loop = false;
	jumping_light.speed = character.jm_l.animation_speed;

	jumping_heavy.PushBack({ 0      ,158 * 8,195,158 });
	jumping_heavy.PushBack({ 195    ,158 * 8,195,158 });
	jumping_heavy.PushBack({ 195 * 2,158 * 8,195,158 }, ACTIVE);

	jumping_heavy.loop = false;
	jumping_heavy.speed = character.jm_h.animation_speed;

	standing_hit.PushBack({ 0, 158 * 11, 195, 158 });
	standing_hit.PushBack({ 195, 158 * 11, 195, 158 });

	standing_hit.loop = true;
	standing_hit.speed = 0.2;

	standing_block.PushBack({ 195 * 4, 158 * 3, 195, 158 });

	standing_block.loop = false;
	standing_block.speed = 0.2;

	crouching_block.PushBack({ 195*3, 158 * 5, 195, 158 });

	crouching_block.loop = false;
	crouching_block.speed = 0.2;

	knockdown.PushBack({ 0      , 158 * 6, 195, 158 });
	knockdown.PushBack({ 0      , 158 * 6, 195, 158 });
	knockdown.PushBack({ 0      , 158 * 6, 195, 158 });
	knockdown.PushBack({ 0      , 158 * 6, 195, 158 });
	knockdown.PushBack({ 0      , 158 * 6, 195, 158 }); // It works for me
	knockdown.PushBack({ 195    , 158 * 6, 195, 158 });
	knockdown.PushBack({ 195 * 2, 158 * 6, 195, 158 });
	knockdown.PushBack({ 195 * 3, 158 * 6, 195, 158 });

	knockdown.loop = false;
	knockdown.speed = 0.1;

	dead.PushBack({ 0      , 158 * 6, 195, 158 });

	dead.loop = true;
	dead.speed = 0.1;

	standing_special1.PushBack({ 195    , 158 * 14, 195, 158 });
	standing_special1.PushBack({ 195 * 2, 158 * 14, 195, 158 });
	standing_special1.PushBack({ 0		, 158 * 14, 195, 158 }, ACTIVE);
	standing_special1.PushBack({ 195    , 158 * 14, 195, 158 });
	standing_special1.PushBack({ 195 * 2, 158 * 14, 195, 158 });


	standing_special1.loop = false;
	standing_special1.speed = character.st_s1.animation_speed;

	standing_special2.PushBack({ 195 * 8 , 158 * 13, 195, 158 });
	standing_special2.PushBack({ 195 * 9 , 158 * 13, 195, 158 });
	standing_special2.PushBack({ 195 * 10, 158 * 13, 195, 158 });
	standing_special2.PushBack({ 195 * 11, 158 * 13, 195, 158 }, ACTIVE);
	standing_special2.PushBack({ 195 * 12, 158 * 13, 195, 158 });


	standing_special2.loop = false;
	standing_special2.speed = character.st_s2.animation_speed;

	jumping_special1.PushBack({ 195 * 7,158 * 5,195,158 }, ACTIVE);

	jumping_special1.loop = false;
	jumping_special1.speed = character.jm_s1.animation_speed;
	jumping_special1.angle = -30;
	
	crouching_special2.PushBack({ 195 * 0 , 158 * 15, 195, 158 });

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
	type = CHAR_TYPE::MAGE;

	//MAGE EXCLUSIVE VARS
	//Load from xml
	fireball_speed = 10;
	fireball_duration = 2000; // in milliseconds
	fireball_emitter_offset.x = 50;
	fireball_emitter_offset.y = 0;

	jm_s1_angle = 30;
	jm_s1_max_height = 600;
	jm_s1_backfire.x = -7;
	jm_s1_backfire.y = -10;
	air_fireball_speed.x = 10;
	air_fireball_speed.y = 5;

	double_jump_power.x = 5;
	double_jump_power.y = 15;
	//Runtime inicialization
	double_jump = false;


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
	bottom_lane = 800;
	upper_lane = 450;
	lateral_limit = 50;
	lane = 0;


	// PROVISIONAL: This should belong to entities, if not fx are loaded twice
	//s_jump = App->audio->loadSFX("SFX/jump.wav");
	//s_light_sword_block = App->audio->loadSFX("SFX/light_sword_block.wav");
	//s_heavy_sword_block = App->audio->loadSFX("SFX/heavy_sword_block.wav");
	//s_light_sword_whiff = App->audio->loadSFX("SFX/light_sword_whiff.wav");
	//s_heavy_sword_whiff = App->audio->loadSFX("SFX/heavy_sword_whiff.wav");
	//s_light_sword_impact = App->audio->loadSFX("SFX/light_sword_impact.wav");
	//s_heavy_sword_impact = App->audio->loadSFX("SFX/heavy_sword_impact.wav");
	//s_standing_special_1 = App->audio->loadSFX("SFX/standing_special_1.wav");
	//s_standing_special_2 = App->audio->loadSFX("SFX/standing_special_2.wav");
	//s_jumping_special_1 = App->audio->loadSFX("SFX/jumping_special_1.wav");;
	//s_crouching_special_1 = App->audio->loadSFX("SFX/crouching_special_1.wav");;
	//s_crouching_special_2 = App->audio->loadSFX("SFX/crouching_special_2.wav");;
	//s_man_death = App->audio->loadSFX("SFX/man_death.wav");
	//s_super = App->audio->loadSFX("SFX/super.wav");


	current_animation = &idle;

	// Collider creation
	hurtbox = App->collision->AddCollider({ 0, 0, standing_hurtbox_size.x, standing_hurtbox_size.y }, HURTBOX, -1, CHAR_ATT_TYPE::NO_ATT, (Module*)App->entities, (Character*)this);
	pushbox = App->collision->AddCollider({ 0, 0, standing_hurtbox_size.x, standing_hurtbox_size.y / 2 }, PUSHBOX, -1, CHAR_ATT_TYPE::NO_ATT, (Module*)App->entities, (Character*)this);

}


Mage::~Mage() {
}

void Mage::standingSpecial1() {
	if (current_animation->GetState() == ACTIVE && !instanciated_hitbox) {
		collider* projectile_collider = App->collision->AddCollider({ (int)logic_position.x, (int)logic_position.y, st_s1.hitbox.w,st_s1.hitbox.h }, COLLIDER_TYPE::PROJECTILE_HITBOX, fireball_duration, CHAR_ATT_TYPE::ST_S1, (Module*)App->entities, this);
		hitboxes.push_back(projectile_collider);
		iPoint speed = { 0,0 };
		iPoint offset = { 0,0 };
		iPoint emitter_offset = { 0,0 };
		if (!fliped) {
			speed.x = fireball_speed;
			offset.x = st_s1.pos_rel_char.x;
			emitter_offset.x = fireball_emitter_offset.x;
		}
		else{
			speed.x = -fireball_speed;
			offset.x = -st_s1.pos_rel_char.x;
			emitter_offset.x = -fireball_emitter_offset.x;
		}
		offset.y = st_s1.pos_rel_char.y;

		ParticleEmitter* emitter = App->particle_system->createEmitter({ (float)logic_position.x,(float)logic_position.y }, "particles/fire-ball.xml");
		App->projectiles->addProjectile(MAGE_FIREBALL, { calculateDrawPosition(0,st_s1.hitbox.w,true) + offset.x, calculateDrawPosition(0,st_s1.hitbox.h,false) + offset.y }, speed, projectile_collider, -1, fliped, scale, emitter,emitter_offset);
		instanciated_hitbox = true;
	}
	if(current_animation->Finished()){
		askRecovery(st_s1.recovery);
		instanciated_hitbox = false;
	}
}

void Mage::standingSpecial2(const bool(&inputs)[MAX_INPUTS]) {
	if (current_animation->GetState() == ACTIVE ) {
		if(!instanciated_hitbox) {
			instanciated_hitbox = true;
			instanciateHitbox(ST_S2);
		}
		fPoint emitter_player_offset = { 0,0 };
		if (!fliped)
			emitter_player_offset.x = (float)st_s2.pos_rel_char.x;
		else
			emitter_player_offset.x = -(float)st_s2.pos_rel_char.x;

		emitter_player_offset.y = (float)st_s2.pos_rel_char.y;
		App->particle_system->createEmitter({ (float)logic_position.x + emitter_player_offset.x, (float)logic_position.y + emitter_player_offset.y + (float)st_s2.hitbox.h/2 }, "particles/fire-column.xml");
	}

	if (current_animation->Finished()) {
		instanciated_hitbox = false;
		collider* hitbox = getCurrentAttackHitbox();
		if (hitbox != nullptr) { // Just for safety
			deleteAttackHitbox(ST_S2);
		}
		askRecovery(st_s2.recovery);
	}
}

void Mage::jumpingSpecial1(const bool(&inputs)[MAX_INPUTS]) {

	if (!state_first_tick) {
		updateAnimation(jumping_special1);
		if (!fliped) {
			velocity.x = jm_s1_backfire.x;
			jumping_special1.angle = jm_s1_angle;
		}
		else {
			velocity.x = -jm_s1_backfire.x;
			jumping_special1.angle = -jm_s1_angle;
		}
		velocity.y = jm_s1_backfire.y;
		state_first_tick = true;
	}
	if (current_animation->GetState() == ACTIVE && !instanciated_hitbox) {
		collider* projectile_collider = App->collision->AddCollider({ (int)logic_position.x, (int)logic_position.y, jm_s1.hitbox.w,jm_s1.hitbox.h }, COLLIDER_TYPE::PROJECTILE_HITBOX, fireball_duration, CHAR_ATT_TYPE::JM_S1, (Module*)App->entities, this);
		hitboxes.push_back(projectile_collider);
		iPoint speed = { 0,0 };
		iPoint offset = { 0,0 };
		iPoint emitter_offset = { 0,0 };
		if (!fliped) {
			speed.x = air_fireball_speed.x;
			offset.x = jm_s1.pos_rel_char.x;
			emitter_offset.x = fireball_emitter_offset.x;
		}
		else {
			speed.x = -air_fireball_speed.x;
			offset.x = -jm_s1.pos_rel_char.x;
			emitter_offset.x = -fireball_emitter_offset.x;
		}
		speed.y = air_fireball_speed.y;
		offset.y = jm_s1.pos_rel_char.y;

		ParticleEmitter* emitter = App->particle_system->createEmitter({ (float)logic_position.x,(float)logic_position.y }, "particles/fire-ball.xml");
		App->projectiles->addProjectile(MAGE_FIREBALL, { calculateDrawPosition(0,jm_s1.hitbox.w,true) + offset.x, calculateDrawPosition(0,jm_s1.hitbox.h,false) + offset.y }, speed, projectile_collider, -1, fliped, scale, emitter, emitter_offset);
		instanciated_hitbox = true;
	}

	if (grounded) {
		instanciated_hitbox = false;
		askRecovery(jm_s1.recovery);
	}
}

void Mage::jumpingSpecial2(const bool(&inputs)[MAX_INPUTS]) {
	double_jump = true;
	fPoint speed = { 0,0 };
	if (inputs[RIGHT]) 		{
		if (!fliped)
			speed.x = -double_jump_power.x;
		else
			speed.x = double_jump_power.x;
	}
	else if (inputs[LEFT]) {
		if (!fliped)
			speed.x = double_jump_power.x;
		else
			speed.x = -double_jump_power.x;
	}
	speed.y = -double_jump_power.y;

	velocity = speed;
	current_animation->Reset();
	updateState(JUMPING);
	
}

bool Mage::standingSpecial1Condition() {
	return !App->projectiles->lookForProjectileType(MAGE_FIREBALL, (Character*)this);
}

bool Mage::jumpingSpecial1Condition() {
	return (logic_position.y <= jm_s1_max_height && current_state != JM_S1);
}
bool Mage::jumpingSpecial2Condition() {
	return !double_jump;
}
void Mage::characterSpecificUpdates() {
	if (grounded)
		double_jump = false;
}