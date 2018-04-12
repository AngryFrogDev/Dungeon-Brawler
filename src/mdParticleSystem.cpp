#include "mdParticleSystem.h"
#include "DebLog.h"
#include "Application.h"
#include "mdWindow.h"
#include "ParticleEmitter.h"


bool mdParticleSystem::update(float dt)
{
	debug_particle_timer.start();
	updateParticles();
	updateEmitters();
	time_to_update = debug_particle_timer.read();
	return true;
}

mdParticleSystem::mdParticleSystem()
{
}


mdParticleSystem::~mdParticleSystem()
{
}

Particle* mdParticleSystem::create(ParticleInfo info)
{
	// Find an available particle.
	for (int i = 0; i < POOL_SIZE; i++)
	{
		if (!particles_[i].inUse())
		{
			particles_[i].init(info);
			return &particles_[i];
		}
	}
}

void mdParticleSystem::createEmitter(fPoint pos, std::string config_path)
{
	ParticleEmitter* emitter = nullptr;

	emitter = new ParticleEmitter(pos, config_path);
	particle_emitters.push_back(emitter);
}

void mdParticleSystem::updateParticles()
{
	particles_count = 0;
	for (int i = 0; i < POOL_SIZE; i++)
	{
		if (!particles_[i].inUse())
			continue;

		particles_[i].animate();

		if (!particles_[i].inUse())
			continue;

		particles_[i].Draw();
		particles_count++;
	}
}

void mdParticleSystem::updateEmitters()
{
	std::list<ParticleEmitter*>::iterator it = particle_emitters.begin();
	while (it != particle_emitters.end())
	{
		if (!(*it)->active)
		{
			particle_emitters.erase(it++);  // alternatively, i = items.erase(i);
		}
		else
		{
			(*it)->update(0);
			++it;
		}
	}
}
