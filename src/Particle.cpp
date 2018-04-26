#include "Particle.h"
#include "Application.h"
#include "mdRender.h"



Particle::Particle()
{
	framesLeft_ = 0;

}


Particle::~Particle()
{
}

void Particle::init(ParticleInfo info)
{
	config(info);

	loadTexture(info.texture_path.c_str());

	SDL_assert(texture);

	SDL_QueryTexture(texture, NULL, NULL, &initial_size.x, &initial_size.y);

	current_color = initial_color;

	scale = initial_scale;

}

void Particle::animate()
{
	//Here we calculate how the attributes should be

	framesLeft_--;

	if (!inUse()) {
		App->textures->unload(texture);
		return; 
	}

	lifetime_ratio = float(framesLeft_) / float(lifetime); 

	scale = calculateRatio(final_scale, initial_scale);

	current_spin = calculateRatio(final_spin, initial_spin);

	updateColors();

	float gravity = 0.3;

	vel.y += gravity;

	pos.x += vel.x; //Simple linear motion
	pos.y += vel.y;
}

bool Particle::inUse() const
{
	return framesLeft_ > 0;
}

void Particle::Draw()
{
	//Here shit is used

	iPoint position_to_draw;

	position_to_draw.x = pos.x - ((initial_size.x / 2)*scale);
	position_to_draw.y = pos.y - ((initial_size.y / 2)*scale);

	SDL_SetTextureColorMod(texture, current_color.r, current_color.g, current_color.b);

	SDL_SetTextureAlphaMod(texture, current_color.alpha);

	SDL_SetTextureBlendMode(texture, blend);

	App->render->drawSprite(draw_priority, texture, position_to_draw.x, position_to_draw.y, NULL, scale,NULL,1,current_spin,NULL,NULL,true);
}

float Particle::calculateRatio(float final, float inital, float variation) const
{
	return (final - inital) * (1-lifetime_ratio) + inital;
}

float Particle::degToRad(float deg)
{
	return deg * (M_PI / 180);
}

void Particle::updateColors()
{
	current_color.r = calculateRatio(final_color.r, initial_color.r);
	current_color.g = calculateRatio(final_color.g, initial_color.g);
	current_color.b = calculateRatio(final_color.b, initial_color.b);
	current_color.alpha = calculateRatio(final_color.alpha, initial_color.alpha);
}

void Particle::config(ParticleInfo info)
{
	pos = info.pos;
	lifetime = framesLeft_ = info.lifetime;

	initial_color = info.initial_color;
	current_color = initial_color;
	final_color = info.final_color;

	initial_scale = info.initial_scale;
	scale = initial_scale;
	final_scale = info.final_scale;

	float angle = degToRad(info.angle);
	vel.x = info.vel_mag * cos(angle);
	vel.y = -info.vel_mag * sin(angle);

	initial_color = info.initial_color;
	current_color = info.initial_color;
	final_color = info.final_color;

	initial_spin = info.initial_spin;
	current_spin = initial_spin;
	final_spin = info.final_spin;

	blend = info.blend;

	draw_priority = info.draw_priority;
}

void Particle::loadTexture(const char* path)
{
	texture = App->textures->load(path);
}

