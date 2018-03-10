#include "Application.h"
#include "ProjDefs.h"
#include "PerfTimer.h"
#include "Timer.h"

Application::Application(int argc, char* args[]) {

}

Application::~Application() {
	for (std::list<Module*>::iterator it = modules.begin(); it != modules.end(); ++it)
		RELEASE(*it);

	modules.clear();
}

bool Application::awake() {
	bool ret = true;

	for (std::list<Module*>::iterator it = modules.begin(); it != modules.end(); ++it)
		ret = (*it)->awake();

	startup_time.start();

	return ret;
}

bool Application::update() {
	bool ret = true;
	++frame_count;
	float dt = frame_time.readSec();
	frame_time.start();

	for (std::list<Module*>::iterator it = modules.begin(); it != modules.end(); ++it)
		ret = (*it)->isActive() ? ret = (*it)->preUpdate() : true;

	for (std::list<Module*>::iterator it = modules.begin(); it != modules.end(); ++it)
		ret = (*it)->isActive() ? ret = (*it)->update(dt) : true;

	for (std::list<Module*>::iterator it = modules.begin(); it != modules.end(); ++it)
		ret = (*it)->isActive() ? ret = (*it)->postUpdate() : true;

	return ret;
}

bool Application::cleanUp() {
	bool ret = true;

	for (std::list<Module*>::iterator it = modules.end(); it != modules.begin(); --it)
		ret = (*it)->cleanUp();

	return ret;
}

void Application::addModule(Module * module)
{
	module->init();
	modules.push_back(module);
}
