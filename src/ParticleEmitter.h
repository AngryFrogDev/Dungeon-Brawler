#pragma once

#include "p2Point.h"
#include "mdTextures.h"
#include "Animation.h"
#include "Timer.h"

#include "p2Point.h"
#include "mdParticleSystem.h"

enum emmiterType { FOUNTAIN, EXPLOSION };

ParticleInfo;

class ParticleEmitter
{
public:
	ParticleEmitter(fPoint pos, std::string config_path);
	~ParticleEmitter();

	Particle* createParticle();
	void update(float dt);
	float addOrSubstractRand(float atribute, int maxVariation) const;


private:
	bool loadConfig(pugi::xml_document& config_file, pugi::xml_node& config_node, std::string path);
	void configureParticle(ParticleInfo &info);

public:
	bool active = true;

	emmiterType type;

	fPoint start_pos;

	int lifetime;
	int framesLeft_;

	Timer emission_timer;
	float period;
	float frequency;

	float lifetime_ratio = 1;

	float initial_scale = 1;
	float scale = 1;
	float final_scale = 0.0f;

	int max_emissions;
	int current_emissions;

private:
	pugi::xml_document config_file;
	pugi::xml_node config;

};

