#include "Warrior.h"
#include "mdCollision.h"
#include "mdProjectiles.h"
#include "mdAudio.h"

Warrior::Warrior(int x_pos, bool _fliped, int lane) : Character() {

	this->lane = lane;

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
	light_attack.speed = 0.2;

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
	heavy_attack.speed = 0.2;

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
	crouching_light.speed = 0.2;

	crouching_heavy.PushBack({ 0,158 * 8,195,158 });
	crouching_heavy.PushBack({ 195,158 * 8,195,158 });
	crouching_heavy.PushBack({ 195 * 2,158 * 8,195,158 });
	crouching_heavy.PushBack({ 195 * 3,158 * 8,195,158 });
	crouching_heavy.PushBack({ 195 * 4,158 * 8,195,158 }, ACTIVE);
	crouching_heavy.PushBack({ 195 * 5,158 * 8,195,158 });
	crouching_heavy.PushBack({ 195 * 6,158 * 8 ,195,158 });

	crouching_heavy.loop = false;
	crouching_heavy.speed = 0.2;

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
	jumping_light.speed = 0.2;

	jumping_heavy.PushBack({ 0      ,158 * 16,195,158 });
	jumping_heavy.PushBack({ 195    ,158 * 16,195,158 }, ACTIVE);

	jumping_heavy.loop = false;
	jumping_heavy.speed = 0.2;

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
	standing_special1.speed = 0.2;

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
	standing_special2.speed = 0.3;

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
	crouching_special1.speed = 0.15;
	
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
	crouching_special2.speed = 0.5;

	jumping_special1.PushBack({ 0, 3476, 195, 158 });
	jumping_special1.PushBack({ 195, 3476, 195, 158, }, ACTIVE);

	jumping_special1.loop = false;
	jumping_special1.speed = 0.3;
	jumping_special1.angle = 30;

	jumping_special2.PushBack({ 0, 3476, 195, 158 });
	jumping_special2.PushBack({ 195, 3476, 195, 158, }, ACTIVE);

	jumping_special2.loop = false;
	jumping_special2.speed = 0.3;
	jumping_special2.angle = 30;

	// Basic attack definitions

	st_l.pos_rel_char = {110,35}; 
	st_l.hitbox = { 0,0,250,20 };
	st_l.active_time = 100;
	st_l.hitstun = 300;
	st_l.blockstun = 100;
	st_l.pushhit = 3;     
	st_l.pushblock = 2;
	st_l.damage = 5;
	st_l.knockdown = false;
	st_l.juggle_speed.x = 5;
	st_l.juggle_speed.y = 5;
	st_l.block_type = BLOCK_TYPE::MID;
	st_l.type = ST_L;
	st_l.recovery = 100;
	
	st_h.pos_rel_char = { 210,20 };
	st_h.hitbox = { 0,0,150, 50 };
	st_h.active_time = 200;
	st_h.hitstun = 500;
	st_h.blockstun = 200;
	st_h.pushhit = 5;
	st_h.pushblock = 3;
	st_h.damage = 15;
	st_h.knockdown = false;
	st_h.juggle_speed.x = 10;
	st_h.juggle_speed.y = 20;
	st_h.block_type = BLOCK_TYPE::MID;
	st_h.recovery = 300;
	st_h.type = ST_H;

	cr_l.pos_rel_char = { 110,50 };
	cr_l.hitbox = { 0,0,70, 30 };
	cr_l.active_time = 70;
	cr_l.hitstun = 200;
	cr_l.blockstun = 50;
	cr_l.pushhit = 2;
	cr_l.pushblock = 2;
	cr_l.damage = 3;
	cr_l.knockdown = false;
	cr_l.juggle_speed.x = 0;
	cr_l.juggle_speed.y = 0;
	cr_l.block_type = BLOCK_TYPE::LOW;
	cr_l.type = CR_L;
	cr_l.recovery = 50;

	cr_h.pos_rel_char = { 190,80 };
	cr_h.hitbox = { 0,0,180, 50 };
	cr_h.active_time = 200;
	cr_h.hitstun = 500;
	cr_h.blockstun = 200;
	cr_h.pushhit = -1; // -1 means that is a knockdown attack
	cr_h.pushblock = 2;
	cr_h.damage = 15;
	cr_h.knockdown = true;
	cr_h.juggle_speed.x = 5;
	cr_h.juggle_speed.y = 20;
	cr_h.block_type = BLOCK_TYPE::LOW;
	cr_h.type = CR_H;
	cr_h.recovery = 350;

	jm_l.pos_rel_char = { 150,70 };
	jm_l.hitbox = { 0,0,140,20 };
	jm_l.active_time = -1;
	jm_l.hitstun = 200;
	jm_l.blockstun = 150;
	jm_l.pushhit = 1;
	jm_l.pushblock = 1;
	jm_l.damage = 6;
	jm_l.knockdown = false;
	jm_l.juggle_speed.x = 5;
	jm_l.juggle_speed.y = 20;
	jm_l.block_type = BLOCK_TYPE::OVERHEAD;
	jm_l.type = JM_L;
	jm_l.recovery = 30;

	jm_h.pos_rel_char = { 42,80 };
	jm_h.hitbox = { 0,0,120,100 };
	jm_h.active_time = -1;
	jm_h.hitstun = 500;
	jm_h.blockstun = 450;
	jm_h.pushhit = 1;
	jm_h.pushblock = 1;
	jm_h.damage = 10;
	jm_h.knockdown = false;
	jm_h.juggle_speed.x = 0;
	jm_h.juggle_speed.y = 10;
	jm_h.block_type = BLOCK_TYPE::OVERHEAD;
	jm_h.type = JM_H;
	jm_h.recovery = 100;

	st_s1.pos_rel_char = { 0, 0 };
	st_s1.hitbox = { 0,0,130,30 };
	st_s1.active_time = -1;
	st_s1.hitstun = 200;
	st_s1.blockstun = 150;
	st_s1.pushhit = 2;
	st_s1.pushblock = 2;
	st_s1.damage = 10;
	st_s1.knockdown = false;
	st_s1.juggle_speed.x = 10;
	st_s1.juggle_speed.y = 15;
	st_s1.block_type = BLOCK_TYPE::MID;
	st_s1.recovery = 300;
	st_s1.type = ST_S1;

	st_s2.pos_rel_char = { 0, 0 };
	st_s2.hitbox = { 0,0,420,100 };
	st_s2.active_time = -1;
	st_s2.hitstun = -1;
	st_s2.blockstun = 300;
	st_s2.pushhit = 2;
	st_s2.pushblock = 2;
	st_s2.damage = 20;
	st_s2.knockdown = true;
	st_s2.juggle_speed.x = 3;
	st_s2.juggle_speed.y = 3;
	st_s2.block_type = BLOCK_TYPE::MID;
	st_s2.recovery = 500;
	st_s2.type = ST_S2;

	cr_s1.pos_rel_char = { 200,50 };
	cr_s1.hitbox = { 0,0,150,150 };
	cr_s1.active_time = -1;
	cr_s1.hitstun = -1;
	cr_s1.blockstun = 300;
	cr_s1.pushhit = -1;
	cr_s1.pushblock = 2;
	cr_s1.damage = 10;
	cr_s1.knockdown = true;
	cr_s1.juggle_speed.x = 2;
	cr_s1.juggle_speed.y = 25;
	cr_s1.block_type = BLOCK_TYPE::MID;
	cr_s1.recovery = 150;
	cr_s1.type = CR_S1;

	cr_s2.pos_rel_char = { 130,0 };
	cr_s2.hitbox = { 0,0,140,300 };
	cr_s2.active_time = -1;
	cr_s2.hitstun = -1;
	cr_s2.blockstun = 300;
	cr_s2.pushhit = -1;
	cr_s2.pushblock = 2;
	cr_s2.damage = 17;
	cr_s2.knockdown = true;
	cr_s2.juggle_speed.x = 10;
	cr_s2.juggle_speed.y = 20;
	cr_s2.block_type = BLOCK_TYPE::MID;
	cr_s2.type = CR_S2;
	cr_s2.recovery = 600;

	jm_s1.pos_rel_char = { 60,50 };
	jm_s1.hitbox = { 0,0,80, 100 };
	jm_s1.active_time = -1;
	jm_s1.hitstun = 500;
	jm_s1.blockstun = 450;
	jm_s1.pushhit = 1;
	jm_s1.pushblock = 1;
	jm_s1.damage = 10;
	jm_s1.knockdown = false;
	jm_s1.juggle_speed.x = 0;
	jm_s1.juggle_speed.y = 0;
	jm_s1.block_type = BLOCK_TYPE::OVERHEAD;
	jm_s1.recovery = 100;
	jm_s1.type = JM_S1;

	jm_s2.pos_rel_char = { 60,50 };
	jm_s2.hitbox = { 0,0,80, 100 };
	jm_s2.active_time = -1;
	jm_s2.hitstun = 500;
	jm_s2.blockstun = 450;
	jm_s2.pushhit = 1;
	jm_s2.pushblock = 1;
	jm_s2.damage = 10;
	jm_s2.knockdown = false;
	jm_s2.juggle_speed.x = 0;
	jm_s2.juggle_speed.y = 0;
	jm_s2.block_type = BLOCK_TYPE::OVERHEAD;
	jm_s2.recovery = 100;
	jm_s2.type = JM_S2;

	super.pos_rel_char = { 0,0 };
	super.hitbox = { 0,0 };
	super.active_time = -1;
	super.hitstun = 700;
	super.blockstun = 450;
	super.pushhit = 1;
	super.pushblock = 1;
	super.damage = 7; // It will be 4 hits + cr_s2, so it will be a total of 45 damage
	super.knockdown = false;
	super.juggle_speed.x = 0;
	super.juggle_speed.y = 10;
	super.block_type = BLOCK_TYPE::MID;
	super.recovery = 0;
	super.type = SUPER;

	super_attack_list.push_back(ST_L);
	super_attack_list.push_back(ST_L);
	super_attack_list.push_back(ST_H);
	super_attack_list.push_back(CR_H);
	super_attack_list.push_back(CR_S2);

	super_last_attack = CR_S2;

	super_advance_speed = 5;

	// Other variable initialization
	grounded = false;
	instanciated_hitbox = false;
	hit = false;
	crouching_hurtbox = false;
	fliped = _fliped;  
	death = false;
	super_window = 30;
	cancelability_window = 30;
	
	max_life = 100;
	current_life = max_life;

	max_super_gauge = 100;
	current_super_gauge = 0;
	super_gauge_gain_hit = 20;
	super_gauge_gain_block = 10;
	super_gauge_gain_strike = 10;

	jump_power.y = 25;
	jump_power.x = 5;

	velocity.y = 0;
	velocity.x = 0;

	current_state = CHAR_STATE::IDLE;
	
	gravity = 1;
	bottom_lane = 800;
	upper_lane = 450;

	logic_position.x = x_pos;
	logic_position.y = 1000;

	starting_position.x = x_pos;
	starting_position.y = -1000;

	standing_hurtbox_size.x = 100;
	standing_hurtbox_size.y = 200;
	crouching_hurtbox_offset = 40;

	invencibility_on_wakeup = 30;

	draw_size.x = 195;
	draw_size.y = 158;

	type = CHAR_TYPE::WARRIOR;
	walk_speed = 4;

	state_first_tick = false;

	scale = 3;
	hurtbox = App->collision->AddCollider({0, 0, standing_hurtbox_size.x, standing_hurtbox_size.y }, HURTBOX, -1, CHAR_ATT_TYPE::NO_ATT, (Module*)App->entities, (Character*)this);
	pushbox = App->collision->AddCollider({0, 0, standing_hurtbox_size.x, standing_hurtbox_size.y/2 }, PUSHBOX, -1, CHAR_ATT_TYPE::NO_ATT, (Module*)App->entities, (Character*)this);

	// WARRIOR EXCLUSIVE VARS
	spin_speed = 6;

	diveKickHeight = 220;
	diveKicking = false;
	jm_s1_angle = 20;
	jm_s1_speed.x = 10;
	jm_s1_speed.y = 10;

	jm_s2_angle = 40;
	jm_s2_speed.x = 5;
	jm_s2_speed.y = 10;

	projectile_duration = 2000;
	projectile_speed = 15;
	projectile_scale = 3;

	swordyuken_invencivility = 300;
	swordyuken_jump_power = 20;

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
}


Warrior::~Warrior() {

}

void Warrior::standingSpecial1() 	{
		if (current_animation->GetState() == ACTIVE && !instanciated_hitbox) {
			collider* projectile_collider = App->collision->AddCollider({ logic_position.x, logic_position.y, st_s1.hitbox.w,st_s1.hitbox.h }, COLLIDER_TYPE::PROJECTILE_HITBOX, projectile_duration, CHAR_ATT_TYPE::ST_S1, (Module*)App->entities, this);
			hitboxes.push_back(projectile_collider);
			iPoint speed;
			if (!fliped)
				speed.x = projectile_speed;
			else
				speed.x = -projectile_speed;
			speed.y = 0;
			App->projectiles->addProjectile(WARRIOR_KNIFE, { logic_position.x, logic_position.y }, speed, projectile_collider, -1, fliped , scale);
			askRecovery(st_s1.recovery);
			projectile = true;
		}
}
void Warrior::standingSpecial2()	{
	hurtbox->type = PROJECTILE_INVENCIBLE_HURTBOX;
	if(!fliped)
		logic_position.x += spin_speed; 
	else
		logic_position.x -= spin_speed;

	if (current_animation->Finished()) {
		instanciated_hitbox = false;
		hurtbox->type = HURTBOX;
		collider* hitbox = getCurrentAttackHitbox();
		if (hitbox != nullptr) { // Just for safety
			deleteAttackHitbox(ST_S2);
		}
		askRecovery(st_s2.recovery);
	}
	else if (current_animation->GetState() == ACTIVE && !instanciated_hitbox) 
		instanciateHitbox(ST_S2);

	collider* hitbox = getCurrentAttackHitbox();
	if (hitbox != nullptr)
		hitbox->SetPos(calculateDrawPosition(st_s2.pos_rel_char.x, st_s2.hitbox.w, true), calculateDrawPosition(st_s2.pos_rel_char.y, st_s2.hitbox.h, false));
}

void Warrior::crouchingSpecial1() {
	if (!fliped)
		logic_position.x += spin_speed; // Provisional
	else
		logic_position.x -= spin_speed; // Provisional

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
		velocity.y -= swordyuken_jump_power;// jump_power.y / 1.25; // Provisional
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

void Warrior::jumpingSpecial1() {
	if (logic_position.y <= diveKickHeight && !diveKicking) 
		diveKicking = true;

	if (diveKicking) {
			updateAnimation(jumping_special1);
		if (!grounded) {
			if (!fliped) {
				velocity.x = jm_s1_speed.x; 
				jumping_special1.angle = jm_s1_angle;
			}
			else {
				velocity.x = -jm_s1_speed.x; 
				jumping_special1.angle = -jm_s1_angle;
			}
			velocity.y = jm_s1_speed.y;

			if (current_animation->GetState() == ACTIVE && !instanciated_hitbox)
				instanciateHitbox(JM_S1);
		}
		collider* hitbox = getCurrentAttackHitbox();
		if (hitbox != nullptr)
			hitbox->SetPos(calculateDrawPosition(jm_s1.pos_rel_char.x, jm_s1.hitbox.w, true), calculateDrawPosition(jm_s1.pos_rel_char.y, jm_s1.hitbox.h, false));
	}
	if (grounded) {
		diveKicking = false;
		instanciated_hitbox = false;
		collider* hitbox = getCurrentAttackHitbox();
		if (hitbox != nullptr) { // Just for safety
			deleteAttackHitbox(JM_S1);
		}
		askRecovery(jm_s1.recovery);
	}
}

void Warrior::jumpingSpecial2() {
	if (logic_position.y <= diveKickHeight && !diveKicking) 
		diveKicking = true;

	if (diveKicking) {
			updateAnimation(jumping_special2);
		if (!grounded) {
			if (!fliped) {
				velocity.x = jm_s2_speed.x; 
				jumping_special2.angle = jm_s2_angle;
			}
			else {
				velocity.x = -jm_s2_speed.x; 
				jumping_special2.angle = -jm_s2_angle;
			}
			velocity.y = jm_s2_speed.y; 

			if (current_animation->GetState() == ACTIVE && !instanciated_hitbox)
				instanciateHitbox(JM_S2);
		}
		collider* hitbox = getCurrentAttackHitbox();
		if (hitbox != nullptr)
			hitbox->SetPos(calculateDrawPosition(jm_s1.pos_rel_char.x, jm_s1.hitbox.w, true), calculateDrawPosition(jm_s1.pos_rel_char.y, jm_s1.hitbox.h, false));
	}
	if(grounded) {
		diveKicking = false;
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