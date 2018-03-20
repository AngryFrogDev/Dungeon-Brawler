#ifndef __MDAUDIO__
#define __MDAUDIO__

#include <list>
#include "Module.h"
#include "SDL_mixer\include\SDL_mixer.h"

class mdAudio : public Module {
public:
	mdAudio();
	~mdAudio();

	bool awake(const pugi::xml_node& md_config);
	bool preUpdate();
	bool cleanUp();

	Mix_Chunk* loadSFX(const char* path);
	Mix_Music* loadMusic(const char* path);
	void freeSFX(Mix_Chunk* sfx);
	void freeMusic(Mix_Music* music);

	void playSFX(Mix_Chunk* sfx) const;
	void sfxVolume(int percent = 100) const;
	void playMusic(Mix_Music* music, int fade_in = 5000) const;
	void musicVolume(int percent = 100) const;
	void stopMusic(int fade_out = 2000) const;

private:
	std::list<Mix_Chunk*> sfxs;
	std::list<Mix_Music*> musics;

	mutable Mix_Music* music_playing = nullptr;
	unsigned int channels;
};

#endif