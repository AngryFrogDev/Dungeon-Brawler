#ifndef __TIMER__
#define __TIMER__

#include "ProjDefs.h"

class Timer
{
public:

	// Constructors
	Timer();
	~Timer(){}

	void start();
	uint32 read() const;
	float readSec() const;

	void stop();

	//Pause or unpause timer
	void pauseUnpause();

	bool isPaused();
	bool isActive();


private:
	uint32	started_at;
	bool	paused = false;
	uint32  paused_at = 0;
	bool	active = false;
};

#endif //__TIMER__