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

	for (int i = 1; i < 11; i++)
		walk_forward.PushBack({ i * x_space,height * 1,width,height });

	walk_forward.loop = true;
	walk_forward.speed = 0.2;

	for (int i = 11; i > 0; i--)
		walk_back.PushBack({ i * x_space,height * 1 ,width,height });

	walk_back.loop = true;
	walk_back.speed = 0.2;


	//STANDING

	for (int i = 0; i < 6; i++) {
		if (i == 4)
			light_attack.PushBack({ i * x_space, height * 5,width,height }, ACTIVE);
		else
			light_attack.PushBack({ i * x_space, height * 5,width,height });
	}

	light_attack.loop = true;
	light_attack.speed = 0.2;

	for (int i = 0; i < 12; i++) {
		if (i == 8)
			heavy_attack.PushBack({ i * x_space, height * 4,width,height }, ACTIVE);
				else
			heavy_attack.PushBack({ i * x_space, height * 4,width,height });
	}
	heavy_attack.loop = true;
	heavy_attack.speed = 0.2;

	//CROUCHING
	for (int i = 0; i < 2; i++) {
		crouch.PushBack({ i * x_space, height * 3,width,height });
	}

	crouch.loop = false;
	crouch.speed = 0.2;

	for (int i = 0; i < 4; i++) {
		if (i == 2)
			crouching_light.PushBack({ i * x_space, height * 6,width,height }, ACTIVE);
		else
			crouching_light.PushBack({ i * x_space, height * 6,width,height });
	}

	crouching_light.loop = false;
	crouching_light.speed = 0.2;

	for (int i = 0; i < 10; i++) {
		if (i == 4)
			crouching_heavy.PushBack({ i * x_space, height * 17,width,height }, ACTIVE);
		else
			crouching_heavy.PushBack({ i * x_space, height * 17,width,height });
	}

	crouching_heavy.loop = false;
	crouching_heavy.speed = character.cr_h.animation_speed;



	//JUMPING
	for (int i = 0; i < 10; i++)
		jump.PushBack({ i * x_space, height * 2,width,height });

	jump.loop = false;
	jump.speed = 0.2;

	for (int i = 0; i < 4; i++) {
		if (i == 3)
			jumping_light.PushBack({ i * x_space, height * 9,width,height },ACTIVE);
		else
			jumping_light.PushBack({ i * x_space, height * 9,width,height });
	}

	jumping_light.loop = false;
	jumping_light.speed = 0.2;

	for (int i = 0; i < 3; i++) {
		if ( i == 2)
			jumping_heavy.PushBack({ i * x_space, height * 8,width,height },ACTIVE);
		else
			jumping_heavy.PushBack({ i * x_space, height * 8,width,height });
	}

	jumping_heavy.loop = false;
	jumping_heavy.speed = 0.2;

	//HIT

	for (int i = 0; i < 4; i++)
		standing_hit.PushBack({ i * x_space, height * 10,width,height });

	standing_hit.loop = true;
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

	standing_special2.PushBack({ 0 * x_space, height * 15,width,height });
	standing_special2.PushBack({ 1 * x_space, height * 15,width,height });
	standing_special2.PushBack({ 2 * x_space, height * 15,width,height });
	standing_special2.PushBack({ 3 * x_space, height * 15,width,height }, ACTIVE);

	standing_special2.loop = false;
	standing_special2.speed = character.st_s2.animation_speed;

	//JUMPING SPECIALS 

	for (int i = 0; i < 4; i++)
		jumping_special2.PushBack({ i * x_space, height * 13,width,height });

	jumping_special2.loop = false;
	jumping_special2.speed = 0.2;


	jumping_special1.PushBack({ 0, height * 19,width,height },ACTIVE);

	
	jumping_special1.loop = false;
	jumping_special1.speed = 0.2;

	//CROUCHING SPECIALS 
	//int i = 7; i > 0; i--
	//int i = 0; i < 8; i++
	for (int i = 7; i > 0; i--)
		crouching_special1.PushBack({ i * x_space, height * 16,width,height });

	crouching_special1.loop = false;
	crouching_special1.speed = 0.2;

	for (int i = 0; i < 3; i++) {
		if (i == 2)
			crouching_special2.PushBack({ i * x_space, height * 7,width,height }, ACTIVE);
		else
			crouching_special2.PushBack({ i * x_space, height * 7,width,height });
	}
	crouching_special2.PushBack({ 2 * x_space, height * 7,width,height });
	crouching_special2.PushBack({ 2 * x_space, height * 7,width,height });
	crouching_special2.PushBack({ 2 * x_space, height * 7,width,height }); // This is for slide to last longer

	crouching_special2.loop = false;
	crouching_special2.speed = character.cr_s2.animation_speed;


	taunt.PushBack({ 0 * width,height * 20, width, height });
	taunt.PushBack({ 1 * width,height * 20, width, height });
	taunt.PushBack({ 2 * width,height * 20, width, height });
	taunt.PushBack({ 3 * width,height * 20, width, height });
	taunt.PushBack({ 4 * width,height * 20, width, height });
	taunt.PushBack({ 5 * width,height * 20, width, height });
	taunt.PushBack({ 6 * width,height * 20, width, height });
	taunt.PushBack({ 7 * width,height * 20, width, height });
	taunt.PushBack({ 8 * width,height * 20, width, height });
	taunt.PushBack({ 9 * width,height * 20, width, height });
	taunt.PushBack({ 10 * width,height * 20, width, height });
	taunt.PushBack({ 11 * width,height * 20, width, height });
	taunt.PushBack({ 12 * width,height * 20, width, height });
	taunt.PushBack({ 0 * width,height * 21, width, height });
	taunt.PushBack({ 1 * width,height * 21, width, height });
	taunt.PushBack({ 2 * width,height * 21, width, height });
	taunt.PushBack({ 3 * width,height * 21, width, height });
	taunt.PushBack({ 4 * width,height * 21, width, height });
	taunt.PushBack({ 5 * width,height * 21, width, height });
	taunt.PushBack({ 6 * width,height * 21, width, height });
	taunt.PushBack({ 7 * width,height * 21, width, height });
	taunt.PushBack({ 8 * width,height * 21, width, height });

	taunt.speed = 0.4;
	taunt.loop = false;

	// REKKA HARDCODED STUFF
	rekka_attack_list.push_back(ST_S2);
	rekka_attack_list.push_back(CR_H);
	rekka_attack_list.push_back(ST_H);

	rekka_advance_speed = 3;
	rekka_last_attack = ST_H;
	rekka_cancelability_window = 15;

	skin_id = 0; // Currently this has no more skins

	fillRecoveriesArray();
}

void Rogue::standingSpecial1(const bool(&inputs)[MAX_INPUTS])
{
	if (current_animation->GetState() == ACTIVE) {
		collider* projectile_collider = App->collision->AddCollider({ (int)logic_position.x, (int)logic_position.y, st_s1.hitbox.w,st_s1.hitbox.h }, COLLIDER_TYPE::PROJECTILE_HITBOX, projectile_duration, st_s1, this);
		hitboxes.push_back(projectile_collider);
		iPoint speed;
		if (!fliped)
			speed.x = projectile_speed;
		else
			speed.x = -projectile_speed;
		speed.y = -30;

		App->projectiles->addProjectile(ROGUE_DAGGER, { calculateDrawPosition(0, st_s1.hitbox.w, true), calculateDrawPosition(0, st_s1.hitbox.h, false) }, speed, projectile_collider, -1, fliped, scale, nullptr, { 0,0 },20.0f);
		askRecovery(st_s1.recovery);
	}
}

void Rogue::crouchingSpecial1()
{
	int emitter_x_offset = 70; // because she moves fast, we'll create the emitter forward //PROVISIONAL load in xml

	if (current_roll_frames == 0) {
		pushbox->active = false;
		makeInvencibleFor(600);
		if (!fliped)
			App->particle_system->createEmitter({ (float)logic_position.x,(float)logic_position.y + 50 }, "particles/smoke-bomb.xml");
		else
			App->particle_system->createEmitter({ (float)logic_position.x,(float)logic_position.y + 50 }, "particles/smoke-bomb.xml");
	}

	if (current_roll_frames == (4*(max_roll_frames / 5))) {
		if (!fliped)
			App->particle_system->createEmitter({ (float)logic_position.x + emitter_x_offset,(float)logic_position.y + 50 }, "particles/smoke-bomb.xml");
		else
			App->particle_system->createEmitter({ (float)logic_position.x - emitter_x_offset,(float)logic_position.y + 50 }, "particles/smoke-bomb.xml");
	}

	if (current_roll_frames < max_roll_frames) {
		current_roll_frames++;

		fPoint speed = { 0,0 };

		if (fliped)
			speed.x = -roll_speed;
		else
			speed.x = roll_speed;

		logic_position.x += speed.x;
	}
	else {
		//App->particle_system->createEmitter({ (float)logic_position.x,(float)logic_position.y }, "particles/smoke-bomb.xml");
		askRecovery(cr_s1.recovery);
		current_roll_frames = 0;
		pushbox->active = true;
	}
}

void Rogue::jumpingSpecial1(const bool(&inputs)[MAX_INPUTS])
{
	if (!state_first_tick) {
		updateAnimation(jumping_special1);
		if (!fliped) {
			velocity.x = -crossbow_recoil;
			jumping_special1.angle = crossbow_angle;
		}
		else {
			velocity.x = crossbow_recoil;
			jumping_special1.angle = -crossbow_angle;
		};
		state_first_tick = true;
	}
	if (current_animation->GetState() == ACTIVE && !instanciated_hitbox) {
		collider* projectile_collider = App->collision->AddCollider({ (int)logic_position.x, (int)logic_position.y, jm_s1.hitbox.w,jm_s1.hitbox.h }, COLLIDER_TYPE::PROJECTILE_HITBOX, 1000, jm_s1, this);
		hitboxes.push_back(projectile_collider);
		iPoint speed = { 0,0 };
		iPoint offset = { 0,0 };
		if (!fliped) {
			speed.x = crossbow_speed.x;
			offset.x = jm_s1.pos_rel_char.x;
		}
		else {
			speed.x = -crossbow_speed.x;
			offset.x = -jm_s1.pos_rel_char.x;
		}
		speed.y = crossbow_speed.y;
		offset.y = jm_s1.pos_rel_char.y;

		App->projectiles->addProjectile(ROGUE_ARROW, { calculateDrawPosition(0,jm_s1.hitbox.w,true) + offset.x, calculateDrawPosition(0,jm_s1.hitbox.h,false) + offset.y }, speed, projectile_collider, -1, fliped, scale, nullptr);
		instanciated_hitbox = true;
	}

	if (grounded) {
		instanciated_hitbox = false;
		askRecovery(jm_s1.recovery);
	}
}

void Rogue::jumpingSpecial2(const bool(&inputs)[MAX_INPUTS])
{
	if (!state_first_tick) {
		state_first_tick = true;
	}
	if (current_dash_frames == 0) {
		airdash_emitter = App->particle_system->createEmitter({ (float)logic_position.x,(float)logic_position.y }, "particles/air-dash.xml");
		fPoint speed = { 0,0 };

		if (inputs[RIGHT]) {
			speed.x = dash_speed;
		}
		else if (inputs[LEFT]) {
			speed.x = -dash_speed;
		}
		else if (fliped)
			speed.x = -dash_speed;
		else 
			speed.x = dash_speed;
		speed.y = -5;
		velocity = speed;
	}



	if (current_dash_frames < max_dash_frames) {
		current_dash_frames++;
	}
	else {
		updateState(JUMPING);
		current_dash_frames = 0;
		has_airdash = false;
	}

}

void Rogue::characterSpecificUpdates()
{
	if (airdash_emitter) {
		airdash_emitter->start_pos.x = (float)logic_position.x;
		airdash_emitter->start_pos.y = (float)logic_position.y;
	}

	if (logic_position.y >= ground_position)
		has_airdash = true;

	if (on_super) {
		if (current_super_frames < max_super_frames) {
			current_super_frames++;
		}
		else {
			on_super = false;
			resetRecoveries();
			current_super_frames = 0;
			current_super_gauge = 0;
			super_emitter->active = false;
			walk_speed = walk_speed / 2;
			jump_power.x = jump_power.x / 2;
		}
	}

	if (super_emitter)
		super_emitter->start_pos = { logic_position.x -30,logic_position.y +20};
}

void Rogue::fillRecoveriesArray()
{
	//	basic_attack_deff st_l, st_h, cr_l, cr_h, jm_l, jm_h, st_s1, st_s2, cr_s1, cr_s2, jm_s1, jm_s2, super;
	recoveries_array[0] = &st_l.recovery;
	recoveries_array[1] = &st_h.recovery;
	recoveries_array[2] = &cr_l.recovery;
	recoveries_array[3] = &cr_h.recovery;
	recoveries_array[4] = &jm_l.recovery;
	recoveries_array[5] = &jm_h.recovery;
	recoveries_array[6] = &st_s1.recovery;
	recoveries_array[7] = &st_s2.recovery;
	recoveries_array[8] = &cr_s1.recovery;
	recoveries_array[9] = &cr_s2.recovery;
	recoveries_array[10] = &jm_s1.recovery;
	recoveries_array[11] = &jm_s2.recovery;

	for (int i = 0; i < 12; i++) {
		original_recoveries_array[i] = *recoveries_array[i];
	}
}

void Rogue::setAllRecoveriesTo(int value)
{
	for (int i = 0; i < 12; i++) 
		*recoveries_array[i] = value;
}

void Rogue::resetRecoveries()
{
	for (int i = 0; i < 12; i++) 
		*recoveries_array[i] = original_recoveries_array[i];
}

void Rogue::crouchingSpecial2() {
	if (!state_first_tick) {
		updateAnimation(crouching_special2);
		state_first_tick = true;
	}
	if (!fliped)
		logic_position.x += slide_speed;
	else
		logic_position.x -= slide_speed;

	if (current_animation->Finished()) {
		instanciated_hitbox = false;
		collider* hitbox = getCurrentAttackHitbox();
		if (hitbox != nullptr) { // Just for safety
			deleteAttackHitbox(CR_S2);
		}
		askRecovery(cr_s2.recovery);
		hurtbox->rect.h = standing_hurtbox_size.y;
	}
	else if (current_animation->GetState() == ACTIVE && !instanciated_hitbox)
		instanciateHitbox(cr_s2);

	collider* hitbox = getCurrentAttackHitbox();
	if (hitbox != nullptr)
		hitbox->SetPos(calculateDrawPosition(cr_s2.pos_rel_char.x, cr_s2.hitbox.w, true), calculateDrawPosition(cr_s2.pos_rel_char.y, cr_s2.hitbox.h, false));
}

void Rogue::standingSpecial2(const bool(&inputs)[MAX_INPUTS]) {
	if (!state_first_tick) {
		rekka_iterator = rekka_attack_list.begin();
		state_first_tick = true;
		updateAnimationOnBasicAttack(*rekka_iterator);
	}

	if (fliped)
		logic_position.x -= rekka_advance_speed;
	else
		logic_position.x += rekka_advance_speed;

	if (current_animation->GetState() == ACTIVE && !instanciated_hitbox) {
		basic_attack_deff current_attack = getAttackData(*rekka_iterator);
		if (*rekka_iterator != rekka_last_attack)
			current_attack.knockdown = false;
		else
			current_attack.knockdown = true;
		instanciateHitbox(current_attack);
		instanciated_hitbox = false;
		if (*rekka_iterator == rekka_last_attack){
			askRecovery(current_attack.recovery);
		}
		else if (lookInBuffer(SPECIAL_2, rekka_cancelability_window)) {
			rekka_iterator++;
			updateAnimationOnBasicAttack(*rekka_iterator);
		}
		else
			askRecovery(current_attack.recovery);
	
	}

}
void Rogue::doSuper() {
	if (!state_first_tick) {
		on_super = true;
		setAllRecoveriesTo(1);
		updateAnimation(taunt);
		if (current_super_frames == 0) {
			super_emitter = App->particle_system->createEmitter({ (float)logic_position.x,(float)logic_position.y }, "particles/rogue-super.xml");
			walk_speed *= 2;
			jump_power.x *= 2;
		}
		state_first_tick = true;
	}
	if (current_animation->Finished()) {
		updateState(IDLE);
	}
}

bool Rogue::jumpingSpecial2Condition() {
	return has_airdash;
}

bool Rogue::standingSpecial1Condition() {
	return App->projectiles->lookForProjectileType(ROGUE_DAGGER, (Character*)this) == 0;
}

void Rogue::updateAnimationOnBasicAttack(CHAR_ATT_TYPE type) {
	switch (type) {
	case ST_S2:
		updateAnimation(standing_special2);
		break;
	case CR_H:
		updateAnimation(crouching_heavy);
		break;
	case ST_H:
		updateAnimation(heavy_attack);
		break;
	}
}

void Rogue::specificCharacterReset() {
	has_airdash = true;
	current_dash_frames = 0;
	current_roll_frames = 0;
}

Rogue::~Rogue()
{
}
