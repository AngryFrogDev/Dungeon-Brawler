#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "SDL/include/SDL_rect.h"
#define MAX_FRAMES 500

enum fighting_state {

	STARTUP,
	ACTIVE,
	RECOVERY
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
	bool pingpong = false;
	float speed = 1.0f;
	fighting_frame frames[MAX_FRAMES];
	int name;

private:
	float current_frame = 0.0f;
	int last_frame = 0;
	int loops = 0;
	enum pingpong
	{
		forward,
		backward
	} direction = forward;

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
		switch (direction)
		{
		case pingpong::forward:
		{
			current_frame += speed;
			if (current_frame >= last_frame)
			{
				current_frame = (loop || pingpong) ? 0.0f : last_frame - 1;
				direction = pingpong ? pingpong::backward : pingpong::forward;
				loops++;
			}
		}
		break;
		case pingpong::backward:
		{
			current_frame -= speed;
			if (current_frame <= 0.0f)
			{
				current_frame = 0.0f;
				direction = pingpong::forward;
				loops++;
			}
		}
		break;
		}

		return frames[(int)current_frame].rect;
	}

	fighting_state GetState()
	{
		switch (direction)
		{
		case pingpong::forward:
		{
			current_frame += speed;
			if (current_frame >= last_frame)
			{
				current_frame = (loop || pingpong) ? 0.0f : last_frame - 1;
				direction = pingpong ? pingpong::backward : pingpong::forward;
				loops++;
			}
		}
		break;
		case pingpong::backward:
		{
			current_frame -= speed;
			if (current_frame <= 0.0f)
			{
				current_frame = 0.0f;
				direction = pingpong::forward;
				loops++;
			}
		}
		break;
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