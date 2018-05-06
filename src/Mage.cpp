#include "Mage.h"
#include "mdCollision.h"
#include "mdProjectiles.h"
#include "mdParticleSystem.h"
#include "mdAudio.h"
#include "mdEntities.h"
#include "Player.h"
#include "ParticleEmitter.h"

Mage::Mage(character_deff character, int x_pos, bool _fliped, int skin) : Character(character) {

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
	
	crouching_special1.PushBack({ 195 * 0 , 158 * 16, 195, 158 });
	crouching_special1.PushBack({ 195 * 1 , 158 * 16, 195, 158 });
	crouching_special1.PushBack({ 195 * 2 , 158 * 16, 195, 158 });
	crouching_special1.PushBack({ 195 * 3 , 158 * 16, 195, 158 }, ACTIVE);
	crouching_special1.PushBack({ 195 * 4 , 158 * 16, 195, 158 });
	crouching_special1.PushBack({ 195 * 5 , 158 * 16 , 195, 158 });
	crouching_special1.PushBack({ 195 * 6 , 158 * 16 , 195, 158 });


	crouching_special1.loop = false;
	crouching_special1.speed = character.cr_s1.animation_speed;

	crouching_special2.PushBack({ 195 * 0 , 158 * 15, 195, 158 });
	crouching_special2.PushBack({ 195 * 1 , 158 * 15, 195, 158 });
	crouching_special2.PushBack({ 195 * 2 , 158 * 15, 195, 158 });
	crouching_special2.PushBack({ 195 * 3 , 158 * 15, 195, 158 }, ACTIVE);
	crouching_special2.PushBack({ 195 * 4 , 158 * 15, 195, 158 });
	crouching_special2.PushBack({ 195 * 5 , 158 * 15, 195, 158 });
	 
	crouching_special2.loop = false;
	crouching_special2.speed = character.cr_s2.animation_speed; 

	super_anim.PushBack({ 195 * 0 , 158 * 13 ,195, 158 });
	super_anim.PushBack({ 195 * 1 , 158 * 13 ,195, 158 });
	super_anim.PushBack({ 195 * 2 , 158 * 13 ,195, 158 });
	super_anim.PushBack({ 195 * 3 , 158 * 13 ,195, 158 });
	super_anim.PushBack({ 195 * 4 , 158 * 13 ,195, 158 }, ACTIVE);
	super_anim.PushBack({ 195 * 5 , 158 * 13 ,195, 158 });
	super_anim.PushBack({ 195 * 6 , 158 * 13 ,195, 158 });
	super_anim.PushBack({ 195 * 7 , 158 * 13 ,195, 158 });

	super_anim.loop = false;
	super_anim.speed = character.super.animation_speed;
	// Constructor inicialization
	fliped = _fliped;
	logic_position.x = x_pos;
	skin_id = skin;
	type = CHAR_TYPE::MAGE;

	//MAGE EXCLUSIVE VARS
	//Load from xml
	fireball_speed = character.fireball_speed;
	fireball_duration = character.fireball_duration; // in milliseconds
	fireball_emitter_offset = character.fireball_emitter_offset;
	fireball_size_grow = 60;
	fireball_damage_boost = 10;

	air_fireball_angle = character.air_fireball_angle;
	air_fireball_max_height = character.air_fireball_max_height;
	air_fireball_backfire = character.air_fireball_backfire;
	air_fireball_speed = character.air_fireball_speed;

	double_jump_power = character.double_jump_power;

	arcane_symbol.PushBack({649, 3717, 67, 17});
	arcane_symbol.PushBack({845, 3717, 67, 17 });
	arcane_symbol.PushBack({1039, 3717, 67, 17 });
	arcane_symbol.PushBack({ 845, 3717, 67, 17 });

	arcane_symbol.loop = true;
	arcane_symbol.speed = 0.2;

	meteorits_life = character.meteorits_life;
	meteorits = character.meteorits;
	first_meteorit_height = character.first_meteorit_height;
	meteorits_offset = character.meteorits_offset;
	meteorits_speed = character.meteorits_speed;
	meteorits_rows = character.meteorits_rows;
	meteorits_rows_offset = character.meteorits_rows_offset;

	//Runtime inicialization
	double_jump = false;
	mine_placed = false;
	meteorits_spawned = false;
	mine_position = { 0,0 };
	fireball_level = 0;
	fireball_max_charge = false;
	charge_fireball_item = false; 
	double_fireball_item = false;


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

}


Mage::~Mage() {
}

void Mage::standingSpecial1(const bool(&inputs)[MAX_INPUTS]) {
	if (current_animation->GetState() == ACTIVE) {
		// Item code
		if (inputs[SPECIAL_1] && !fireball_max_charge && charge_fireball_item && !instanciated_hitbox) {
			fireball_level += 0.016; // Time duration of a frame at 60 fps
			standing_special1.paused = true;
			if (fireball_level >= 2)
				fireball_max_charge = true;
			if (hit)
				standing_special1.paused = false;
		}
		else if (!instanciated_hitbox) {
			standing_special1.paused = false;
			st_s1.hitbox.w = initial_fireball.hitbox.w + fireball_size_grow*(int)fireball_level;
			st_s1.hitbox.h = initial_fireball.hitbox.h + fireball_size_grow*(int)fireball_level;
			st_s1.damage = initial_fireball.damage + fireball_damage_boost*(int)fireball_level;
		//Item code
			collider* projectile_collider = App->collision->AddCollider({ (int)logic_position.x, (int)logic_position.y, st_s1.hitbox.w,st_s1.hitbox.h }, COLLIDER_TYPE::PROJECTILE_HITBOX, fireball_duration, CHAR_ATT_TYPE::ST_S1, (Module*)App->entities, this);
			hitboxes.push_back(projectile_collider);
			iPoint speed, offset, emitter_offset = { 0,0 };
			if (!fliped) {
				speed.x = fireball_speed;
				offset.x = st_s1.pos_rel_char.x;
				emitter_offset.x = fireball_emitter_offset.x;
			}
			else {
				speed.x = -fireball_speed;
				offset.x = -st_s1.pos_rel_char.x;
				emitter_offset.x = -fireball_emitter_offset.x;
			}
			offset.y = st_s1.pos_rel_char.y;
			speed.y = 0;
			ParticleEmitter* emitter = nullptr;
			switch ((int)fireball_level) {
				case 0:
					emitter = App->particle_system->createEmitter({ (float)logic_position.x,(float)logic_position.y }, "particles/fire-ball.xml");
					break;
				case 1:
					emitter = App->particle_system->createEmitter({ (float)logic_position.x,(float)logic_position.y }, "particles/fire-ball-2.xml");
					break;
				case 2:
					emitter = emitter = App->particle_system->createEmitter({ (float)logic_position.x,(float)logic_position.y }, "particles/fire-ball-3.xml");
					break;
			}

			App->projectiles->addProjectile(MAGE_FIREBALL, { calculateDrawPosition(0,st_s1.hitbox.w,true) + offset.x, calculateDrawPosition(0,st_s1.hitbox.h,false) + offset.y }, speed, projectile_collider, -1, fliped, scale, emitter, emitter_offset);
			instanciated_hitbox = true;
		}
	}
	if(current_animation->Finished()){
		askRecovery(st_s1.recovery);
		instanciated_hitbox = false;
		fireball_level = 0;
		fireball_max_charge = 0;
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
			velocity.x = air_fireball_backfire.x;
			jumping_special1.angle = air_fireball_angle;
		}
		else {
			velocity.x = -air_fireball_backfire.x;
			jumping_special1.angle = -air_fireball_angle;
		}
		velocity.y = air_fireball_backfire.y;
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
			emitter_offset.x = 50;
		}
		else {
			speed.x = -air_fireball_speed.x;
			offset.x = -jm_s1.pos_rel_char.x;
			emitter_offset.x = -50; //Air fireball emmiter offset
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
		speed.x = double_jump_power.x;
	}
	else if (inputs[LEFT]) {
		speed.x = -double_jump_power.x;
	}
	speed.y = -double_jump_power.y;

	velocity = speed;
	current_animation->Reset();
	updateState(JUMPING);
	
}

void Mage::crouchingSpecial1() {
	if (current_animation->Finished()) {
		askRecovery(cr_s1.recovery);
		instanciated_hitbox = false;
	}
	else if (current_animation->GetState() == ACTIVE && !instanciated_hitbox) {
		instanciateHitbox(CR_S1);
	}
}

void Mage::crouchingSpecial2() {
	if (!state_first_tick) {
		updateAnimation(crouching_special2);
		state_first_tick = true;
	}
	if (!mine_placed) {
		if (current_animation->GetState() == ACTIVE && !instanciated_hitbox) {
			iPoint pos = { 0,0 };
			if (!fliped)
				pos.x = logic_position.x + cr_s2.pos_rel_char.x;
			else
				pos.x = logic_position.x - cr_s2.pos_rel_char.x;
			pos.y = logic_position.y + cr_s2.pos_rel_char.y;
			mine_position = pos;
			mine_placed = true;
			instanciated_hitbox = true;
		}
	}
	else {
		if (current_animation->GetState() == ACTIVE && !instanciated_hitbox) {
			hitboxes.push_back(App->collision->AddCollider({ mine_position.x - cr_s2.hitbox.w / 2 , mine_position.y - cr_s2.hitbox.h / 2 ,cr_s2.hitbox.w, cr_s2.hitbox.h }, HITBOX, cr_s2.active_time, CR_S2, App->entities, this));
			mine_placed = false;
			instanciated_hitbox = true;
		}
	}
	if (current_animation->Finished()) {
		instanciated_hitbox = false;
		askRecovery(cr_s2.recovery);
	}
}

void Mage::doSuper() {
	if (!state_first_tick) {
		updateAnimation(super_anim);
		state_first_tick = true;
		current_super_gauge = 0;
	}
	if (current_animation->GetState() == ACTIVE && !meteorits_spawned) {
		iPoint meteorits_current_offset = { 0,0 };
		int x_pos = 0; // Projectiles should spawn on the camera limit
		if (fliped) {
			meteorits_current_offset.x = -meteorits_offset.x;
			x_pos = App->render->camera.x + App->render->camera.w;
		}
		else {
			meteorits_current_offset.x = meteorits_offset.x;
			x_pos = App->render->camera.x;
		}

		meteorits_current_offset.y = meteorits_offset.y;

		for(int a = 0; a < meteorits_rows; a++){
			for (int i = 0; i < meteorits; i++) {
				iPoint meteorit_spawn_position = { (int)x_pos/*maybe player pos here?*/ + meteorits_current_offset.x*i, first_meteorit_height + meteorits_current_offset.y *i + meteorits_rows_offset*a };
				collider* projectile_collider = App->collision->AddCollider({ meteorit_spawn_position.x, (int)meteorit_spawn_position.y, jm_s1.hitbox.w,jm_s1.hitbox.h }, COLLIDER_TYPE::PROJECTILE_HITBOX, meteorits_life, CHAR_ATT_TYPE::JM_S1, (Module*)App->entities, this);
				hitboxes.push_back(projectile_collider);
				iPoint speed = { 0,0 };
				iPoint emitter_offset = { 0,0 };
				if (!fliped) {
					speed.x = meteorits_speed.x;
					emitter_offset.x = fireball_emitter_offset.x;
				}
				else {
					speed.x = -meteorits_speed.x;
					emitter_offset.x = -fireball_emitter_offset.x;
				}
				speed.y = meteorits_speed.y;
				speed.x += rand() % 1;
				speed.y += rand() % 5;

				ParticleEmitter* emitter = App->particle_system->createEmitter({ (float)meteorit_spawn_position.x,(float)meteorit_spawn_position.y }, "particles/fire-ball.xml");
				App->projectiles->addProjectile(MAGE_METEORIT, meteorit_spawn_position, speed, projectile_collider, -1, fliped, scale, emitter, emitter_offset);
			}
		}
		meteorits_spawned = true;
	}
	if (current_animation->Finished()) {
		askRecovery(super.recovery);
		meteorits_spawned = false;
	}

}

bool Mage::standingSpecial1Condition() {
	bool ret = true;
	if (!double_fireball_item)
		ret = App->projectiles->lookForProjectileType(MAGE_FIREBALL, (Character*)this) == 0;
	else 
		ret = App->projectiles->lookForProjectileType(MAGE_FIREBALL, (Character*)this) <= 1;

	return ret;
}

bool Mage::jumpingSpecial1Condition() {
	return (logic_position.y <= air_fireball_max_height && current_state != JM_S1);
}
bool Mage::jumpingSpecial2Condition() {
	return !double_jump;
}
void Mage::characterSpecificUpdates() {
	if (grounded)
		double_jump = false;

	if (mine_placed) 
		App->render->drawSprite(3, App->entities->mage_graphics, mine_position.x - 67 * 3 / 2, mine_position.y - 17 * 3 / 2, &arcane_symbol.GetCurrentFrame(), scale, fliped, 1, 0, 0, 0, true);
	
}

void Mage::giveItem(ITEMS type) {
	switch (type) {
	case SPECIAL_ITEM_1:
		charge_fireball_item = true;
		break;
	case SPECIAL_ITEM_2:
		double_fireball_item = true;
		break;
	}
}


void Mage::takeAllItems() {
	charge_fireball_item = false;
	double_fireball_item = false;
}