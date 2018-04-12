#pragma once
#include "Module.h"
#include "mdTextures.h"
#include "p2Point.h"
#include "Particle.h"
#include "Timer.h"
#include <list>
class ParticleEmitter;

class mdParticleSystem :
	public Module
{
public:
	bool update(float dt) override;

	mdParticleSystem();
	~mdParticleSystem();

	Particle* create(ParticleInfo info);
	void createEmitter(fPoint pos, std::string config_path);

	void updateParticles();
	void updateEmitters();

public:
	int time_to_update;
	int particles_count = 0;

private:
	Timer debug_particle_timer;

	static const int POOL_SIZE = 1000;

	Particle particles_[POOL_SIZE];

	SDL_Texture* particle_textures = nullptr;

	std::list<ParticleEmitter*> particle_emitters;
};

