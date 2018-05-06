#include "Rogue.h"
#include "mdCollision.h"
#include "mdProjectiles.h"
#include "mdParticleSystem.h"
#include "mdAudio.h"
#include "mdEntities.h"
#include "Player.h"
#include "ParticleEmitter.h"



Rogue::Rogue(character_deff character, int x_pos, bool _fliped, int skin) : Character(character, x_pos, _fliped, skin) {

	idle.PushBack({ 0,158,195,158 });
	idle.PushBack({ 195,158,195,158 });
	idle.PushBack({ 195 * 2,158,195,158 });
	idle.PushBack({ 195 * 3,158,195,158 });
	idle.PushBack({ 195 * 4,158,195,158 });
	idle.PushBack({ 195 * 5,158,195,158 });

	idle.loop = true;
	idle.speed = 0.2;

	type = CHAR_TYPE::ROGUE;
	skin_id = 0;

}


Rogue::~Rogue()
{
}
