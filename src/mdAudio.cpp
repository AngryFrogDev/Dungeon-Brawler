#include "mdAudio.h"
#include "DebLog.h"
#pragma comment( lib, "SDL_mixer/libx86/SDL2_mixer.lib" )

mdAudio::mdAudio() : Module() {
	name = "audio";
}
mdAudio::~mdAudio() {}

bool mdAudio::awake(const pugi::xml_node & md_config) {
	bool ret = true;

	int flags = 0;
	if (md_config.child("flags").attribute("flac").as_bool(true))
		flags |= MIX_INIT_FLAC;
	if (md_config.child("flags").attribute("mod").as_bool(false))
		flags |= MIX_INIT_MOD;
	if (md_config.child("flags").attribute("mp3").as_bool(false))
		flags |= MIX_INIT_MP3;
	if (md_config.child("flags").attribute("ogg").as_bool(false))
		flags |= MIX_INIT_OGG;

	if (Mix_Init(flags)&flags != flags) {
		LOG("mdAudio : Failed to init required files support! %s", Mix_GetError());
		ret = false;
	}

	unsigned int chunksize = md_config.child("sfx").attribute("chunksize").as_uint(1024);
	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, chunksize) == -1) { //The 2 in this function is for stereo
		LOG("mdAudio : Failed to open audio! %s", Mix_GetError());
		ret = false;
	}

	channels = md_config.child("sfx").attribute("channels").as_uint(16);
	Mix_AllocateChannels(channels);

	return ret;
}

bool mdAudio::preUpdate() {
	bool ret = true;
	if (music_playing != nullptr && !Mix_PlayingMusic())
		music_playing = nullptr;
	return ret;
}

bool mdAudio::cleanUp() {
	bool ret = true;
	LOG("mdAudio : Freeing sound effects and music");

	Mix_AllocateChannels(0); //Will free and stop playing all channels to avoid problems.
	for (std::list<Mix_Chunk*>::iterator it = sfxs.begin(); it != sfxs.end(); ++it)
		Mix_FreeChunk(*it);
	sfxs.clear();

	Mix_HaltMusic(); //Stop playing music to avoid problems.
	for (std::list<Mix_Music*>::iterator it = musics.begin(); it != musics.end(); ++it)
		Mix_FreeMusic(*it);
	musics.clear();

	Mix_CloseAudio();

	return ret;
}

Mix_Chunk * mdAudio::loadSFX(const char * path) {
	Mix_Chunk* ret = nullptr;

	ret = Mix_LoadWAV(path);
	if (ret != NULL)
		sfxs.push_back(ret);
	else
		LOG("mdAudio : Failed to load wav from path \"%s\": %s", path, Mix_GetError());

	return ret;
}

Mix_Music * mdAudio::loadMusic(const char * path) {
	Mix_Music* ret = nullptr;

	ret = Mix_LoadMUS(path);
	if (ret != NULL)
		musics.push_back(ret);
	else
		LOG("mdAudio : Failed to load music from path \"%s\": %s", path, Mix_GetError());

	return ret;
}

void mdAudio::freeSFX(Mix_Chunk * sfx) {
	if (sfx != nullptr) {
		//Halt the channel playing the sfx if any
		for (int i = 0; i < channels; i++) {
			if (Mix_Playing(i) && sfx == Mix_GetChunk(i)) {
				Mix_HaltChannel(i); 
				break;
			}
		}
		for (std::list<Mix_Chunk*>::iterator it = sfxs.begin(); it != sfxs.end(); ++it) {
			if (*it == sfx) {
				sfxs.erase(it);
				Mix_FreeChunk(sfx);
				break;
			}
		}
	}
}

void mdAudio::freeMusic(Mix_Music * music) {
	if (music != nullptr) {
		//Halt the music if it is playing
		if (music == music_playing) {
			Mix_HaltMusic();
			music_playing = nullptr;
		}
		for (std::list<Mix_Music*>::iterator it = musics.begin(); it != musics.end(); ++it) {
			if (*it == music) {
				musics.erase(it);
				Mix_FreeMusic(music);
				break;
			}
		}
	}
}

void mdAudio::playSFX(Mix_Chunk * sfx) const {
	//More channels could be reserved during playtime if not enough
	if (sfx != nullptr) {
		if (Mix_PlayChannel(-1, sfx, 0) == -1)
			LOG("mdAudio : Could not play sfx: %s", Mix_GetError());
	}
	else
		LOG("mdAudio : Could not play sfx, sfx was null");
}

void mdAudio::sfxVolume(int new_volume) const {
	LOG("mdAudio : SFX volume changed from %d to %d", Mix_Volume(-1, new_volume), Mix_Volume(-1, -1));

}

void mdAudio::playMusic(Mix_Music * music, int fade_in) const {
	if (music != nullptr) {
		if ((music != music_playing || Mix_FadingMusic() == MIX_FADING_OUT)) {
			if (Mix_FadeInMusic(music, -1, fade_in) == 0)
				music_playing = music;
			else
				LOG("mdAudio : Could not play music, error: %s\n", Mix_GetError());
		}
		else
			LOG("mdAudio: Could not play music, music is already playing");
	}
	else
		LOG("mdAudio : Could not play music, music was null");

}

void mdAudio::musicVolume(int new_volume) const {
	LOG("mdAudio : Music volume changed from %d to %d", Mix_VolumeMusic(new_volume), Mix_VolumeMusic(-1));
	return;
}

void mdAudio::stopMusic(int fade_out) const {
	if (fade_out != 0)
		Mix_FadeOutMusic(fade_out);
	else {
		Mix_HaltMusic();
		music_playing = nullptr;
	}
}

