#ifndef __MDVIDEOPLAYER_H__
#define __MDVIDEOPLAYER_H__

#include "Module.h"

#include <dshow.h>
#include <Vfw.h>                          
#pragma comment( lib, "vfw32.lib" )                 // Search For VFW32.lib While Linking

struct SDL_Texture;

class mdVideoplayer : public Module {
public:

	mdVideoplayer();
	// Destructor
	~mdVideoplayer();

	bool preUpdate();
	bool update(float dt);
	bool cleanUp();

	bool playAVI(const char* path);
	bool grabAVIFrame();

	void stopPlaying();
	

private:
	bool				is_playing = false;

	int			frame = 0;							// Frame Counter

	AVISTREAMINFO       stream_info;                        // Pointer To A Structure Containing Stream Info
	PAVISTREAM			pavi;                       // Handle To An Open Stream
	PGETFRAME			pointer_to_frame;                        // Pointer To A GetFrame Object

	long				lastFrame;                  // Last Frame Of The Stream
	int					width;                      // Video Width
	int					height;                     // Video Height
	char*				pdata;					    // Pointer To Texture Data
	int					fps;                        // Will Hold Rough Milliseconds Per Frame

	SDL_Texture*		frame_texture;

	float				last_frame_time;
};

#endif // __MDVIDEOPLAYER_H__
