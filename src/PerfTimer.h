#ifndef __PERFTIMER__
#define __PERFTIMER__

#include "ProjDefs.h"

class PerfTimer
{
public:

	// Constructor
	PerfTimer();
	~PerfTimer(){}

	void start();
	double readMs() const;
	uint64 readTicks() const;

private:
	uint64	started_at;
	static uint64 frequency;
};

#endif //__PERFTIMER__