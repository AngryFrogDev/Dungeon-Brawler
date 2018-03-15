#include "Application.h"
#include "ProjDefs.h"
#include "PerfTimer.h"
#include "Timer.h"
#include "DebLog.h"

//Modules
#include "mdWindow.h"
#include "mdFilesystem.h"
#include "mdInput.h"


Application::Application(int argc, char* args[]) {
	filesystem = new mdFilesystem;
	addModule(filesystem);
	window = new mdWindow;
	addModule(window);
	input = new mdInput;
	addModule(input);
}

Application::~Application() {
	for (std::list<Module*>::iterator it = modules.begin(); it != modules.end(); ++it)
		RELEASE(*it);

	modules.clear();
}

bool Application::awake() {
	bool ret = true;

	pugi::xml_document config_file;
	pugi::xml_node config;
	

	loadConfig(config_file, config);

	if (config.empty()) {
		ret = false;
	}
	else {
		pugi::xml_node app_config = config.child("application");
		
		for (std::list<Module*>::iterator it = modules.begin(); it != modules.end(); ++it)
			ret = (*it)->awake(config.child((*it)->name.c_str()));
	}
	startup_time.start();

	return ret;
}

bool Application::update() {
	bool ret = true;
	++frame_count;
	float dt = frame_time.readSec();
	frame_time.start();

	for (std::list<Module*>::iterator it = modules.begin(); it != modules.end() && ret; ++it)
		ret = (*it)->isActive() ? ret = (*it)->preUpdate() : true;

	for (std::list<Module*>::iterator it = modules.begin(); it != modules.end() && ret; ++it)
		ret = (*it)->isActive() ? ret = (*it)->update(dt) : true;

	for (std::list<Module*>::iterator it = modules.begin(); it != modules.end() && ret; ++it)
		ret = (*it)->isActive() ? ret = (*it)->postUpdate() : true;

	return ret;
}

bool Application::cleanUp() {
	bool ret = true;

	for (std::list<Module*>::reverse_iterator it = modules.rbegin(); it != modules.rend(); ++it)
		ret = (*it)->cleanUp();

	return ret;
}

void Application::addModule(Module * module)
{
	module->init();
	modules.push_back(module);
}

void Application::loadConfig(pugi::xml_document& config_file, pugi::xml_node& config_node) {
	char* buffer;
	int size = filesystem->load("config.xml", &buffer);
	pugi::xml_parse_result result = config_file.load_buffer(buffer, size);
	if (size != 0)
		RELEASE(buffer);

	if (result == NULL)
		LOG("Application : Could not load config.xml - %s", result.description());
	else
		config_node = config_file.child("config");
}
