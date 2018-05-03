#ifndef __PARTICLE__
#define __PARTICLE__

#include "p2Point.h"
#include "mdTextures.h"
#include "Animation.h"
#include "mdRender.h"



struct Color {
	Uint8 r;
	Uint8 g;
	Uint8 b;
	Uint8 alpha;
};
struct ParticleInfo {
	int lifetime;

	float initial_scale;
	float final_scale;

	Color initial_color;
	Color final_color;

	fPoint pos;

	float vel_mag;

	float angle;

	float initial_spin;
	float final_spin;

	SDL_BlendMode blend;

	std::string texture_path;

	int draw_priority;

	bool use_gravity = true;
};



class Particle
{
public:
	Particle();

	~Particle();

	void init(ParticleInfo info);

	void animate();

	bool inUse() const;

	void Draw();

private:

	float calculateRatio(float final, float initial = 0, float variation = 0) const;

	float degToRad(float deg);

	void config(ParticleInfo info);

	void updateColors();

	void loadTexture(const char* path);

private:

	int			lifetime;
	int			framesLeft_;

	fPoint		pos;
	iPoint		initial_size; //To keep it centered
	fPoint		vel;

	float lifetime_ratio = 1;

	float initial_scale = 1;
	float scale = 1;
	float final_scale = 0.0f;

	Color initial_color;
	Color current_color;
	Color final_color;

	SDL_BlendMode blend;

	float initial_spin;
	float current_spin;
	float final_spin;

	int draw_priority = 1;

	SDL_Texture* texture = nullptr;

	bool use_gravity = true;

};


#endif
