#include "Application.h"
#include "ProjDefs.h"
#include "PerfTimer.h"
#include "Timer.h"
#include "DebLog.h"

//Modules
#include "mdWindow.h"
#include "mdFilesystem.h"
#include "mdInput.h"
#include "mdRender.h"
#include "mdTextures.h"
#include "mdEntities.h"
#include "mdAudio.h"


Application::Application(int argc, char* args[]) {
	filesystem = new mdFilesystem;
	window = new mdWindow;
	render = new mdRender;
	input = new mdInput;
	textures = new mdTextures;
	entities = new mdEntities;
	audio = new mdAudio;

	addModule(filesystem);
	addModule(window);
	addModule(input);
	addModule(textures);
	addModule(render);
	addModule(audio);
	addModule(entities);
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

	if (ret) ret = finishUpdate();

	return ret;
}

bool Application::finishUpdate() {
	bool ret = true;

	if (last_sec_frame_time.read() > 1000) {
		last_sec_frame_time.start();
		prev_last_sec_frame_count = last_sec_frame_count;
		last_sec_frame_count = 0;
	}

	float avg_fps = float(frame_count) / startup_time.readSec();
	float seconds_since_startup = startup_time.readSec();
	uint32 last_frame_ms = frame_time.read();
	uint32 frames_on_last_update = prev_last_sec_frame_count;

	static char title[256];
	sprintf_s(title, 256, "Av.FPS: %.2f Last Frame Ms: %02u Last sec frames: %i  Time since startup: %.3f Frame Count: %lu ",
		avg_fps, last_frame_ms, frames_on_last_update, seconds_since_startup, frame_count);
	App->window->setWindowTitle(title);

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
