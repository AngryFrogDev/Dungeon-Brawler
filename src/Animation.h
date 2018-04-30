#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "SDL/include/SDL_rect.h"
#define MAX_FRAMES 500

enum fighting_state {

	STARTUP,
	ACTIVE,
};
struct fighting_frame {

	fighting_frame()
	{

	}

	fighting_frame(SDL_Rect _rect, fighting_state _state)
	{
		rect = _rect;
		state = _state;
	}
	SDL_Rect rect;
	fighting_state state;

};

class Animation {
public:
	bool loop = true;
	bool paused = false;
	float speed = 1.0f;
	float angle = 0;
	fighting_frame frames[MAX_FRAMES];

private:
	float current_frame = 0.0f;
	int last_frame = 0;
	int loops = 0;


public:

	Animation()
	{}

	Animation(const Animation& anim) : loop(anim.loop), speed(anim.speed), last_frame(anim.last_frame)
	{
		SDL_memcpy(&frames, anim.frames, sizeof(frames));
	}

	void PushBack(const SDL_Rect& rect, fighting_state state = STARTUP)
	{
		if (last_frame < MAX_FRAMES)
		{ 
			fighting_frame frame(rect, state);
			frames[last_frame++] = frame;
		}
	}

	SDL_Rect& GetCurrentFrame()
	{
		if(!paused)
			current_frame += speed;
		if (current_frame >= last_frame)
		{
			current_frame = (loop) ? 0.0f : last_frame - 1;
			loops++;
		}
		return frames[(int)current_frame].rect;
	}

	fighting_state GetState()
	{
		if (!paused)
			current_frame += speed;
		if (current_frame >= last_frame) {
			current_frame = (loop) ? 0.0f : last_frame - 1;
			loops++;
		}
		return frames[(int)current_frame].state;
	}

	bool Finished() const
	{
		return loops > 0;
	}

	void Reset()
	{
		current_frame = 0.0f;
		loops = 0;
	}
};


#endif