#include "Warrior.h"
#include "mdCollision.h"

Warrior::Warrior(int x_pos, bool _fliped): Character() {
	type = CHAR_TYPE::WARRIOR;
	walk_speed = 4;

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
	walk_forward.PushBack({ 195*2,158,195,158 });
	walk_forward.PushBack({ 195*3,158,195,158 });
	walk_forward.PushBack({ 195*4,158,195,158 });
	walk_forward.PushBack({ 195*5,158,195,158 });
	walk_forward.PushBack({ 195*6,158,195,158 });
	walk_forward.PushBack({ 195*7,158,195,158 });
	walk_forward.PushBack({ 195*8,158,195,158 });
	walk_forward.PushBack({ 195*9,158,195,158 });
	walk_forward.PushBack({ 195*10,158,195,158 });
	walk_forward.PushBack({ 195*11,158,195,158 });

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
	light_attack.speed = 0.3;

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
	heavy_attack.speed = 0.3;

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
	crouching_light.speed = 0.5;

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

	// Basic attack definitions

	st_l.pos_rel_char = {210,35}; 
	st_l.hitbox = { 0,0,100,20 };
	st_l.active_time = 100;
	st_l.hitstun = 300;
	
	st_h.pos_rel_char = { 210,20 };
	st_h.hitbox = { 0,0,150, 50 };
	st_h.active_time = 200;
	st_h.hitstun = 500;

	cr_l.pos_rel_char = { 110,50 };
	cr_l.hitbox = { 0,0,70, 30 };
	cr_l.active_time = 70;
	cr_l.hitstun = 200;

	cr_h.pos_rel_char = { 190,80 };
	cr_h.hitbox = { 0,0,180, 50 };
	cr_h.active_time = 200;
	cr_h.hitstun = 500;

	jm_l.pos_rel_char = { 150,70 };
	jm_l.hitbox = { 0,0,140,20 };
	jm_l.active_time = -1;
	jm_l.hitstun = 200;

	jm_h.pos_rel_char = { 42,80 };
	jm_h.hitbox = { 0,0,120,100 };
	jm_h.active_time = -1;
	jm_h.hitstun = 500;

	// Other variable initialization
	grounded = true;
	instanciated_hitbox = false;

	jump_power.y = 25;
	jump_power.x = 5;

	velocity.y = 0;
	velocity.x = 0;

	current_state = CHAR_STATE::IDLE;
	fliped = _fliped;  
	gravity = 1;
	bottom_lane = 500;

	logic_position.x = x_pos;
	logic_position.y = 500;

	standing_hurtbox_size.x = 100;
	standing_hurtbox_size.y = 200;

	draw_size.x = 195;
	draw_size.y = 158;

	scale = 3;
	hurtbox = App->collision->AddCollider({ logic_position.x - standing_hurtbox_size.x/2, logic_position.y - standing_hurtbox_size.y/2, standing_hurtbox_size.x, standing_hurtbox_size.y }, HURTBOX, -1,(Module*)App->entities,(Character*)this);

}


Warrior::~Warrior() {

}