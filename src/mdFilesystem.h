#ifndef __MDFILESYSTEM__
#define __MDFILESYSTEM__

#include "Module.h"

class mdFilesystem : public Module {
public:
	mdFilesystem();
	virtual ~mdFilesystem();

	bool awake(const pugi::xml_node& md_config);
	bool cleanUp();

	bool addPath(const char* path, const char* mount_point = NULL);
	bool fileExists(const char* file) const;
	bool isDirectory(const char* name) const;
	bool changeWriteDir(const char* dir) const;

	unsigned int load(const char* file, char** buffer) const;

	unsigned int save(const char* file,  char* buffer, unsigned int size) const;

private:
	char* base_path = NULL;
	char* write_path = NULL;
};

#endif