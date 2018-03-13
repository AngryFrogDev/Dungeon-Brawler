#ifndef __MODULE__
#define __MODULE__

#include <string>
#include "pugixml\pugixml.hpp"

class Module
{
public:

	Module() : active(false)
	{}

	virtual ~Module()
	{}

	void init() {
		active = true;
	}

	//TODO: Add XML support
	// Called before render is available
	virtual bool awake(pugi::xml_node& md_config) {
		return true;
	}

	// Called each loop iteration
	virtual bool preUpdate() {
		return true;
	}

	// Called each loop iteration
	virtual bool update(float dt) {
		return true;
	}

	// Called each loop iteration
	virtual bool postUpdate() {
		return true;
	}

	// Called before quitting
	virtual bool cleanUp() {
		return true;
	}

	virtual bool load() {
		return true;
	}

	virtual bool save() const {
		return true;
	}

	bool isActive() const {
		return active;
	}

public:

	std::string	name;
	bool	active;

};

#endif // __MODULE__