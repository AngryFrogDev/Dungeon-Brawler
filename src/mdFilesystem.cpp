#include "mdFilesystem.h"
#include "DebLog.h"
#include "ProjDefs.h"

#include "SDL/include/SDL.h"
#include "PhysFS/include/physfs.h"

#pragma comment (lib, "PhysFS/libx86/physfs.lib")

mdFilesystem::mdFilesystem() : Module() {
	name = "filesystem";

	//The base path needs to be initiated by PhysFS for modules to load config
	base_path = SDL_GetBasePath();
	PHYSFS_init(base_path);

	//We include the root file to our paths by default
	addPath(".");
}

mdFilesystem::~mdFilesystem() {
	PHYSFS_deinit();
}

bool mdFilesystem::awake(const pugi::xml_node& md_config) {
	LOG("mdFilesystem : Loading filesystem");
	bool ret = true;

	//Adding all paths inside configuration in order
	for (pugi::xml_node path = md_config.child("path"); path; path.next_sibling("path"))
		addPath(path.child_value());

	//Getting the write directory from SDL
	write_path = SDL_GetPrefPath("Angry Frog", "Dungeon Brawler"); //TODO: Might want to not hardcode this

	if (PHYSFS_setWriteDir(write_path) == 0) {
		LOG("mdFilesystem : Error while creating the write dir: %s", PHYSFS_getLastError());
		ret = false;
	}
	else {
		LOG("mdFilesystm: Writing directory created - %s", write_path);
		ret = addPath(write_path, "save/");
	}

	return ret;
}

bool mdFilesystem::cleanUp() {
	return true;
}

bool mdFilesystem::addPath(const char* path, const char* mount_point) {
	bool ret = true;

	if (PHYSFS_mount(path, mount_point, 1) == 0) {
		LOG("mdFilesystem : Error while adding the path %s - %s", path, PHYSFS_getLastError());
		ret = false;
	}

	return ret;
}

bool mdFilesystem::fileExists(const char* file) const {
	return PHYSFS_exists(file);
}

bool mdFilesystem::isDirectory(const char* name) const {
	return PHYSFS_isDirectory(name);
}

bool mdFilesystem::changeWriteDir(const char* dir) const {
	bool ret = true;

	if (PHYSFS_setWriteDir(dir) == 0) {
		LOG("mdFilesystem : Error while changing write directory - %s", PHYSFS_getLastError());
		ret = false;
	}
	else
		LOG("mdFilesystem : Write directory changed to %s", dir);

	return ret;
}

unsigned int mdFilesystem::load(const char * file, char ** buffer) const {
	unsigned int ret = 0;

	PHYSFS_file* fs_file = PHYSFS_openRead(file);
	if (fs_file != NULL) {
		int size = PHYSFS_fileLength(fs_file);
		if (size > 0) {
			*buffer = new char[size];
			if (size != PHYSFS_readBytes(fs_file, *buffer, size)) {
				LOG("mdFilesystem : Error while reading file %s - &s", file, PHYSFS_getLastError());
				RELEASE(buffer);
			}
			else
				ret = size;
		}

		if (PHYSFS_close(fs_file) == 0)
			LOG("mdFilesystem : Error while closing file %s - %s", file, PHYSFS_getLastError());
	}
	else
		LOG("mdFilesystem : Error while opening file %s - %s", file, PHYSFS_getLastError());

	return ret;
}

unsigned int mdFilesystem::save(const char* file, char* buffer, unsigned int size) const {
	unsigned int ret = 0;

	PHYSFS_file* fs_file = PHYSFS_openWrite(file);
	if (fs_file != NULL) {
		if (size != PHYSFS_writeBytes(fs_file, (const void*)buffer, size))
			LOG("mdFilesystem : Error while writing to file %s - %s", file, PHYSFS_getLastError());
		else
			ret = size;

		if (PHYSFS_close(fs_file) == 0)
			LOG("mdFilesystem : Error while closing file %s - %s", file, PHYSFS_getLastError());
	}
	else
		LOG("mdFilesystem : Error while opening file %s - %s", file, PHYSFS_getLastError());

	return ret;
}
