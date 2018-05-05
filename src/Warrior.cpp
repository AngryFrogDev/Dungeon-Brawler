#include "Warrior.h"
#include "mdCollision.h"
#include "mdProjectiles.h"
#include "mdAudio.h"
#include "mdEntities.h"
#include "mdParticleSystem.h"

Warrior::Warrior(character_deff character, int x_pos, bool _fliped, int skin) : Character() {

	lane = 1;
	skin_id = skin;
	
	//PROVISIONAL: Animations should be loaded from the xml

	//Animations
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

	standing_special1.PushBack({ 0		, 158 * 5, 195, 158 });
	standing_special1.PushBack({ 195    , 158 * 5, 195, 158 });
	standing_special1.PushBack({ 195 * 2, 158 * 5, 195, 158 });
	standing_special1.PushBack({ 195 * 3, 158 * 5, 195, 158 });
	standing_special1.PushBack({ 195 * 4, 158 * 5, 195, 158 });
	standing_special1.PushBack({ 195 * 5, 158 * 5, 195, 158 }, ACTIVE);
	standing_special1.PushBack({ 195 * 6, 158 * 5, 195, 158 });

	standing_special1.loop = false;
	standing_special1.speed = character.st_s1.animation_speed;

	standing_special2.PushBack({ 0, 948, 195, 158 });
	standing_special2.PushBack({ 0, 948, 195, 158 });
	standing_special2.PushBack({ 0, 948, 195, 158 });
	standing_special2.PushBack({ 195, 948, 195, 158 });
	standing_special2.PushBack({ 390, 948, 195, 158 });
	standing_special2.PushBack({ 585, 948, 195, 158 }, ACTIVE);
	standing_special2.PushBack({ 780, 948, 195, 158 });
	standing_special2.PushBack({ 975, 948, 195, 158 });
	standing_special2.PushBack({ 1170, 948, 195, 158 });
	standing_special2.PushBack({ 1365, 948, 195, 158 });
	standing_special2.PushBack({ 1560, 948, 195, 158 });
	standing_special2.PushBack({ 1755, 948, 195, 158 });
	standing_special2.PushBack({ 1950, 948, 195, 158 });
	standing_special2.PushBack({ 2145, 948, 195, 158 });
	standing_special2.PushBack({ 0, 1106, 195, 158 });
	standing_special2.PushBack({ 195, 1106, 195, 158 });
	standing_special2.PushBack({ 0, 948, 195, 158 });
	standing_special2.PushBack({ 195, 948, 195, 158 });
	standing_special2.PushBack({ 390, 948, 195, 158 });
	standing_special2.PushBack({ 585, 948, 195, 158 });
	standing_special2.PushBack({ 780, 948, 195, 158 });
	standing_special2.PushBack({ 975, 948, 195, 158 });
	standing_special2.PushBack({ 1170, 948, 195, 158 });
	standing_special2.PushBack({ 1365, 948, 195, 158 });
	standing_special2.PushBack({ 1560, 948, 195, 158 });
	standing_special2.PushBack({ 1755, 948, 195, 158 });
	standing_special2.PushBack({ 1950, 948, 195, 158 });
	standing_special2.PushBack({ 2145, 948, 195, 158 });
	standing_special2.PushBack({ 0, 1106, 195, 158 });
	standing_special2.PushBack({ 195, 1106, 195, 158 });

	standing_special2.loop = false;
	standing_special2.speed = character.st_s2.animation_speed;

	crouching_special1.PushBack({ 0, 1738, 195, 158 });
	crouching_special1.PushBack({ 195, 1738, 195, 158 });
	crouching_special1.PushBack({ 390, 1738, 195, 158 });
	crouching_special1.PushBack({ 585, 1738, 195, 158 }, ACTIVE);
	crouching_special1.PushBack({ 780, 1738, 195, 158 });
	crouching_special1.PushBack({ 975, 1738, 195, 158 });
	crouching_special1.PushBack({ 1170, 1738, 195, 158 });
	crouching_special1.PushBack({ 1365, 1738, 195, 158 });
	crouching_special1.PushBack({ 1560, 1738, 195, 158 });
	crouching_special1.PushBack({ 1755, 1738, 195, 158 });
	crouching_special1.PushBack({ 1755, 1738, 195, 158 });

	crouching_special1.loop = false;
	crouching_special1.speed = character.cr_s1.animation_speed;
	
	crouching_special2.PushBack({ 0, 632, 195, 158 });
	crouching_special2.PushBack({ 195, 632, 195, 158 });
	crouching_special2.PushBack({ 390, 632, 195, 158 });
	crouching_special2.PushBack({ 585, 632, 195, 158 }, ACTIVE);
	crouching_special2.PushBack({ 780, 632, 195, 158 });
	crouching_special2.PushBack({ 975, 632, 195, 158 });
	crouching_special2.PushBack({ 1170, 632, 195, 158 });
	crouching_special2.PushBack({ 1365, 632, 195, 158 });
	crouching_special2.PushBack({ 1560, 632, 195, 158 });
	crouching_special2.PushBack({ 1755, 632, 195, 158 });
	crouching_special2.PushBack({ 1755, 632, 195, 158 });
	crouching_special2.PushBack({ 1950, 632, 195, 158 });
	crouching_special2.PushBack({ 1950, 632, 195, 158 });
	
	crouching_special2.loop = false;
	crouching_special2.speed = character.cr_s2.animation_speed;

	jumping_special1.PushBack({ 0, 3476, 195, 158 });
	jumping_special1.PushBack({ 195, 3476, 195, 158, }, ACTIVE);

	jumping_special1.loop = false;
	jumping_special1.speed = character.jm_s1.animation_speed;
	jumping_special1.angle = 30;

	jumping_special2.PushBack({ 0, 3476, 195, 158 });
	jumping_special2.PushBack({ 195, 3476, 195, 158, }, ACTIVE);

	jumping_special2.loop = false;
	jumping_special2.speed = character.jm_s2.animation_speed;
	jumping_special2.angle = 30;

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
	non_flip_attacks = character.non_flip_attacks;
	shadow_rect = { 452, 3719, 68, 14 };
	shadow_offset = 105;
	// Constructor inicialization
	fliped = _fliped;
	logic_position.x = x_pos;
	type = CHAR_TYPE::WARRIOR;

	// WARRIOR EXCLUSIVE VARS
	spin_speed = character.spin_speed;
	improved_spin_speed = character.improved_spin_speed;
	improved_spin_recovery = character.improved_spin_recovery;
	jm_s1_angle = character.jm_s1_angle;
	jm_s1_speed = character.jm_s1_speed;
	dive_kick_max_height = character.dive_kick_max_height; 
	jm_s2_angle = character.jm_s2_angle;
	jm_s2_speed = character.jm_s2_speed;
	dive_kick_object_mult = character.dive_kick_object_mult;
	projectile_duration = character.projectile_duration;
	projectile_speed = character.projectile_speed;
	swordyuken_invencivility = character.swordyuken_invencivility;
	swordyuken_jump_power = character.swordyuken_jump_power;
	super_invencibility = 300;
	// PROVISIONAL: Should be loaded from xml
	super_attack_list.push_back(ST_L);
	super_attack_list.push_back(ST_L);
	super_attack_list.push_back(ST_H);
	super_attack_list.push_back(CR_H);
	super_attack_list.push_back(CR_S2);

	super_last_attack = CR_S2;

	super_advance_speed = 5;
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
	spin_object = false; 
	dive_kick_object = false;
	// Others
	bottom_lane = 800;
	upper_lane = 450;
	lateral_limit = 50;


	// PROVISIONAL: This should belong to entities, if not fx are loaded twice
	s_jump = App->audio->loadSFX("SFX/jump.wav");
	s_light_sword_block = App->audio->loadSFX("SFX/light_sword_block.wav");
	s_heavy_sword_block = App->audio->loadSFX("SFX/heavy_sword_block.wav");
	s_light_sword_whiff = App->audio->loadSFX("SFX/light_sword_whiff.wav");
	s_heavy_sword_whiff = App->audio->loadSFX("SFX/heavy_sword_whiff.wav");
	s_light_sword_impact = App->audio->loadSFX("SFX/light_sword_impact.wav");
	s_heavy_sword_impact = App->audio->loadSFX("SFX/heavy_sword_impact.wav");
	s_standing_special_1 = App->audio->loadSFX("SFX/standing_special_1.wav");
	s_standing_special_2 = App->audio->loadSFX("SFX/standing_special_2.wav");
	s_jumping_special_1 = App->audio->loadSFX("SFX/jumping_special_1.wav");;
	s_crouching_special_1 = App->audio->loadSFX("SFX/crouching_special_1.wav");;
	s_crouching_special_2 = App->audio->loadSFX("SFX/crouching_special_2.wav");;
	s_man_death = App->audio->loadSFX("SFX/man_death.wav");
	s_super = App->audio->loadSFX("SFX/super.wav");


	current_animation = &idle;

	// Collider creation
	hurtbox = App->collision->AddCollider({ 0, 0, standing_hurtbox_size.x, standing_hurtbox_size.y }, HURTBOX, -1, CHAR_ATT_TYPE::NO_ATT, (Module*)App->entities, (Character*)this);
	pushbox = App->collision->AddCollider({ 0, 0, standing_hurtbox_size.x, standing_hurtbox_size.y / 2 }, PUSHBOX, -1, CHAR_ATT_TYPE::NO_ATT, (Module*)App->entities, (Character*)this);
}


Warrior::~Warrior() {

}

void Warrior::standingSpecial1(const bool(&inputs)[MAX_INPUTS]){

		if (current_animation->GetState() == ACTIVE) {
			collider* projectile_collider = App->collision->AddCollider({ (int)logic_position.x, (int)logic_position.y, st_s1.hitbox.w,st_s1.hitbox.h }, COLLIDER_TYPE::PROJECTILE_HITBOX, projectile_duration, CHAR_ATT_TYPE::ST_S1, (Module*)App->entities, this);
			hitboxes.push_back(projectile_collider);
			iPoint speed;
			if (!fliped)
				speed.x = projectile_speed;
			else
				speed.x = -projectile_speed;
			speed.y = 0;

			App->projectiles->addProjectile(WARRIOR_KNIFE, { calculateDrawPosition(0, st_s1.hitbox.w, true), calculateDrawPosition(0, st_s1.hitbox.h, false) }, speed, projectile_collider, -1, fliped, scale, nullptr);
			askRecovery(st_s1.recovery);
		}
}

void Warrior::standingSpecial2(const bool(&inputs)[MAX_INPUTS])	{
	hurtbox->type = PROJECTILE_INVENCIBLE_HURTBOX;

	if(!spin_object){
		if(!fliped)
			logic_position.x += spin_speed; 
		else
			logic_position.x -= spin_speed;
	}
	else {
		if (inputs[LEFT])
			logic_position.x -= improved_spin_speed;
		else if (inputs[RIGHT])
			logic_position.x += improved_spin_speed;
		else if (inputs[DOWN] && inputs[SPECIAL_2]) {
			instanciated_hitbox = false;
			hurtbox->type = HURTBOX;
			collider* hitbox = getCurrentAttackHitbox();
			if (hitbox != nullptr) { // Just for safety
				deleteAttackHitbox(ST_S2);
			}
			updateState(ATTACKING, CR_S2);
		}
	}

	if (current_animation->Finished()) {
		instanciated_hitbox = false;
		hurtbox->type = HURTBOX;
		collider* hitbox = getCurrentAttackHitbox();
		if (hitbox != nullptr) { // Just for safety
			deleteAttackHitbox(ST_S2);
		}
		if(!spin_object)
			askRecovery(st_s2.recovery);
		else
			askRecovery(improved_spin_recovery);
	}
	else if (current_animation->GetState() == ACTIVE && !instanciated_hitbox) 
		instanciateHitbox(ST_S2);

	collider* hitbox = getCurrentAttackHitbox();
	if (hitbox != nullptr)
		hitbox->SetPos(calculateDrawPosition(st_s2.pos_rel_char.x, st_s2.hitbox.w, true), calculateDrawPosition(st_s2.pos_rel_char.y, st_s2.hitbox.h, false));
}

void Warrior::crouchingSpecial1() {

	if (!fliped)
		logic_position.x += spin_speed; 
	else
		logic_position.x -= spin_speed; 

	if (current_animation->Finished()) {
		instanciated_hitbox = false;
		collider* hitbox = getCurrentAttackHitbox();
		if (hitbox != nullptr) { // Just for safety
			deleteAttackHitbox(CR_S1);
		}
		askRecovery(cr_s1.recovery);
		hurtbox->rect.h = standing_hurtbox_size.y;
	}
	else if (current_animation->GetState() == ACTIVE && !instanciated_hitbox)
		instanciateHitbox(CR_S1);

	collider* hitbox = getCurrentAttackHitbox();
	if (hitbox != nullptr)
		hitbox->SetPos(calculateDrawPosition(cr_s1.pos_rel_char.x, cr_s1.hitbox.w, true), calculateDrawPosition(cr_s1.pos_rel_char.y, cr_s1.hitbox.h, false));



}

void Warrior::crouchingSpecial2()	{ // Should have recovery

	if (!state_first_tick) {
		updateAnimation(crouching_special2);
		velocity.y -= swordyuken_jump_power;
		grounded = false;
		makeInvencibleFor(swordyuken_invencivility);
		state_first_tick = true;
	}
	if (velocity.y >= 0) { 
		setCrouchingHurtbox(false);
		instanciated_hitbox = false;
		collider* hitbox = getCurrentAttackHitbox();
		if (hitbox != nullptr) {
			deleteAttackHitbox(CR_S2);
		}
	}
	if (grounded)
		askRecovery(cr_s2.recovery);

	if (current_animation->GetState() == ACTIVE && !instanciated_hitbox)
		instanciateHitbox(CR_S2);

	collider* hitbox = getCurrentAttackHitbox();
	if (hitbox != nullptr)
		hitbox->SetPos(calculateDrawPosition(cr_s2.pos_rel_char.x, cr_s2.hitbox.w, true), calculateDrawPosition(cr_s2.pos_rel_char.y, cr_s2.hitbox.h, false));

}

void Warrior::jumpingSpecial1(const bool(&inputs)[MAX_INPUTS]) {
	if (!state_first_tick) {
		updateAnimation(jumping_special1);
		state_first_tick = true;
	}
	if (!grounded) {
		iPoint speed = { 0,0 };
		if (dive_kick_object) {
			speed.create(jm_s1_speed.x * dive_kick_object_mult, jm_s1_speed.y * dive_kick_object_mult);
		}
		else
			speed = jm_s1_speed;

		if (!fliped) {
			velocity.x = speed.x; 
			jumping_special1.angle = jm_s1_angle;
		}
		else {
			velocity.x = -speed.x; 
			jumping_special1.angle = -jm_s1_angle;
		}
		velocity.y = speed.y;

		if (current_animation->GetState() == ACTIVE && !instanciated_hitbox)
			instanciateHitbox(JM_S1);
	}
	collider* hitbox = getCurrentAttackHitbox();
	if (hitbox != nullptr)
		hitbox->SetPos(calculateDrawPosition(jm_s1.pos_rel_char.x, jm_s1.hitbox.w, true), calculateDrawPosition(jm_s1.pos_rel_char.y, jm_s1.hitbox.h, false));


	if(dive_kick_object){
		if (inputs[LIGHT_ATTACK]) {
			collider* hitbox = getCurrentAttackHitbox();
			if (hitbox != nullptr) { // Just for safety
				deleteAttackHitbox(JM_S1);
			}
			instanciated_hitbox = false;
			updateState(ATTACKING, JM_L);
		}
		else if(inputs[HEAVY_ATTACK]){
			collider* hitbox = getCurrentAttackHitbox();
			if (hitbox != nullptr) { // Just for safety
				deleteAttackHitbox(JM_S1);
			}
			instanciated_hitbox = false;
			updateState(ATTACKING, JM_H);
		}
	}

	if (grounded) {
		instanciated_hitbox = false;
		collider* hitbox = getCurrentAttackHitbox();
		if (hitbox != nullptr) { // Just for safety
			deleteAttackHitbox(JM_S1);
		}
		askRecovery(jm_s1.recovery);
	}
}

void Warrior::jumpingSpecial2(const bool(&inputs)[MAX_INPUTS]) {
	if (!state_first_tick) {
		updateAnimation(jumping_special2);
		state_first_tick = true;
	}
	if (!grounded) {
		iPoint speed = { 0,0 };
		if (dive_kick_object) {
			speed.create(jm_s2_speed.x * dive_kick_object_mult, jm_s2_speed.y * dive_kick_object_mult);
		}
		else
			speed = jm_s2_speed;

		if (!fliped) {
			velocity.x = speed.x; 
			jumping_special2.angle = jm_s2_angle;
		}
		else {
			velocity.x = -speed.x; 
			jumping_special2.angle = -jm_s2_angle;
		}
		velocity.y = speed.y; 

		if (current_animation->GetState() == ACTIVE && !instanciated_hitbox)
			instanciateHitbox(JM_S2);
	}
	collider* hitbox = getCurrentAttackHitbox();
	if (hitbox != nullptr)
		hitbox->SetPos(calculateDrawPosition(jm_s1.pos_rel_char.x, jm_s1.hitbox.w, true), calculateDrawPosition(jm_s1.pos_rel_char.y, jm_s1.hitbox.h, false));

	if (dive_kick_object) {
		if (inputs[LIGHT_ATTACK]) {
			collider* hitbox = getCurrentAttackHitbox();
			if (hitbox != nullptr) { // Just for safety
				deleteAttackHitbox(JM_S2);
			}
			instanciated_hitbox = false;
			updateState(ATTACKING, JM_L);
		}
		else if (inputs[HEAVY_ATTACK]) {
			collider* hitbox = getCurrentAttackHitbox();
			if (hitbox != nullptr) { // Just for safety
				deleteAttackHitbox(JM_S2);
			}
			instanciated_hitbox = false;
			updateState(ATTACKING, JM_H);
		}
	}
	if(grounded) {
		instanciated_hitbox = false;
		collider* hitbox = getCurrentAttackHitbox();
		if (hitbox != nullptr) { // Just for safety
			deleteAttackHitbox(JM_S2);
		}
		askRecovery(jm_s1.recovery);
	}
}

void Warrior::doSuper() {

	if (!state_first_tick) {
		super_iterator = super_attack_list.begin();
		updateAnimationOnBasicAttack(*super_iterator);
		state_first_tick = true;
		current_super_gauge = 0;
		makeInvencibleFor(super_invencibility);
	}
	if (fliped)
		logic_position.x -= super_advance_speed;
	else
		logic_position.x += super_advance_speed;

	if (current_animation->GetState() == ACTIVE && !instanciated_hitbox) {
		instanciateHitbox(*super_iterator);
		super_iterator++;
		instanciated_hitbox = false;
		if (*super_iterator != super_last_attack)
			updateAnimationOnBasicAttack(*super_iterator);
		else
			updateState(ATTACKING, *super_iterator);
	}
}
void Warrior::updateAnimationOnBasicAttack(CHAR_ATT_TYPE type) {
	switch (type) {
	case ST_L:
		updateAnimation(light_attack);
		current_animation->Reset();
		break;
	case ST_H:
		updateAnimation(heavy_attack);
		break;
	case CR_L:
		updateAnimation(crouching_light);
		break;
	case CR_H:
		updateAnimation(crouching_heavy);
		break;
	}
	
}
void Warrior::giveItem(ITEMS type) {
	switch (type) {
	case SPECIAL_ITEM_1:
		spin_object = true;
		break;
	case SPECIAL_ITEM_2:
		dive_kick_object = true;
		break;
	}
}


void Warrior::takeAllItems() {
	dive_kick_object = false;
	spin_object = false;
}
bool Warrior::jumpingSpecial1Condition() {
	return logic_position.y <= dive_kick_max_height;
}
bool Warrior::jumpingSpecial2Condition() {
	return logic_position.y <= dive_kick_max_height;
}
bool Warrior::standingSpecial1Condition() {
	return App->projectiles->lookForProjectileType(WARRIOR_KNIFE, (Character*)this) == 0;
}