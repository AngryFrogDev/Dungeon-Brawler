#ifndef __APPLICATION__
#define __APPLICATION__

#include <list>
#include "Module.h"
#include "ProjDefs.h"
#include "Timer.h"

class PerfTimer;
// Modules
class mdWindow;

class Application
{
public:

	// Constructor
	Application(int argc, char* args[]);

	// Destructor
	virtual ~Application();

	// Called before render is available
	bool awake();

	// Called each loop iteration
	bool update();

	// Called before quitting
	bool cleanUp();

	// Add a new module to handle
	void addModule(Module* module);

private:
	std::list<Module*> modules;
	uint64				frame_count = 0;
	Timer				startup_time;
	Timer				frame_time;
	float	dt = 0.0f;

public:
	// Modules
	mdWindow* window;
};

extern Application* App;

#endif //__APPLICATION__