#include "Warrior.h"
#include "mdCollision.h"
#include "mdProjectiles.h"

Warrior::Warrior(int x_pos, bool _fliped, int lane): Character() {

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
	standing_special1.PushBack({ 195 * 5, 158 * 5, 195, 158 });
	standing_special1.PushBack({ 195 * 6, 158 * 5, 195, 158 }, ACTIVE);

	standing_special1.loop = false;
	standing_special1.speed = 0.1;

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

	standing_special2.loop = false;
	standing_special2.speed = 0.3;

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

	// Other variable initialization
	grounded = false;
	instanciated_hitbox = false;
	hit = false;
	crouching_hurtbox = false;
	fliped = _fliped;  
	
	max_life = 100;
	current_life = max_life;

	jump_power.y = 25;
	jump_power.x = 5;

	velocity.y = 0;
	velocity.x = 0;

	current_state = CHAR_STATE::JUMPING;
	
	gravity = 1;
	bottom_lane = 500;
	upper_lane = 150;

	logic_position.x = x_pos;
	logic_position.y = -1000;

	standing_hurtbox_size.x = 100;
	standing_hurtbox_size.y = 200;
	crouching_hurtbox_offset = 40;

	invencibility_on_wakeup = 30;

	draw_size.x = 195;
	draw_size.y = 158;

	type = CHAR_TYPE::WARRIOR;
	walk_speed = 4;

	scale = 3;
	hurtbox = App->collision->AddCollider({ logic_position.x - standing_hurtbox_size.x/2, logic_position.y - standing_hurtbox_size.y/2, standing_hurtbox_size.x, standing_hurtbox_size.y }, HURTBOX, -1,CHAR_ATT_TYPE::NO_ATT,(Module*)App->entities,(Character*)this);

	// WARRIOR EXCLUSIVE VARS
	spin_speed = 15;
	projectile_duration = 2000;
	projectile_speed = 15;
	projectile_scale = 3;

}


Warrior::~Warrior() {

}

void Warrior::standingSpecial1() 	{
		if (current_animation->GetState() == ACTIVE && !instanciated_hitbox) {
			collider* projectile_collider = App->collision->AddCollider({ logic_position.x, logic_position.y, st_s1.hitbox.w,st_s1.hitbox.h }, COLLIDER_TYPE::HITBOX, projectile_duration, CHAR_ATT_TYPE::ST_S1, (Module*)App->entities, this);
			iPoint speed;
			if (!fliped)
				speed.x = projectile_speed;
			else
				speed.x = -projectile_speed;
			speed.y = 0;
			App->projectiles->addProjectile(WARRIOR_KNIFE, { logic_position.x, logic_position.y }, speed, projectile_collider, -1, fliped , scale);
			current_state = IDLE;
			projectile = true;
		}
}
void Warrior::standingSpecial2()	{
	if(!fliped)
		logic_position.x += spin_speed; 
	else
		logic_position.x -= spin_speed;

	if (current_animation->Finished()) {
		current_state = IDLE;
		instanciated_hitbox = false;
		if (hitbox != nullptr) 			{
			hitbox->to_delete = true;
			hitbox = nullptr;
		}
	}
	else if (current_animation->GetState() == ACTIVE && !instanciated_hitbox)
		instanciateHitbox(ST_S2);

	if (hitbox != nullptr)
		hitbox->SetPos(calculateDrawPosition(st_s2.pos_rel_char.x, st_s2.hitbox.w, true), calculateDrawPosition(st_s2.pos_rel_char.y, st_s2.hitbox.h, false));
}
