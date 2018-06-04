#include "mdVideoplayer.h"
#include "DebLog.h"
#include "Application.h"
#include "mdTextures.h"
#include "mdRender.h"
#include "SDL/include/SDL.h"


mdVideoplayer::mdVideoplayer() {
	name = "video";
}

mdVideoplayer::~mdVideoplayer() {
}

bool mdVideoplayer::preUpdate() {
	bool ret = true;
	App->textures->unload(frame_texture);
	return ret;
}

bool mdVideoplayer::update(float dt) {
	bool ret = true;
	if (is_playing) {
		ret = grabAVIFrame();
	}

	return ret;
}

bool mdVideoplayer::cleanUp() {
	bool ret = true;
	if (is_playing)
		stopPlaying();
	return ret;
}

bool mdVideoplayer::playAVI(const char* path, bool fullscreen) {
	AVIFileInit();                          // Opens The AVIFile Library
						
	if (AVIStreamOpenFromFile(&pavi, path, streamtypeVIDEO, 0, OF_READ, NULL) != 0) {// Opens The AVI Stream
		LOG("mdVideoplayer : Failed to open AVI frame");
		return false;
	}

	AVIStreamInfo(pavi, &stream_info, sizeof(stream_info));					// Reads Information About The Stream Into psi
	width = stream_info.rcFrame.right - stream_info.rcFrame.left;           // Width Is Right Side Of Frame Minus Left
	height = stream_info.rcFrame.bottom - stream_info.rcFrame.top;          // Height Is Bottom Of Frame Minus Top
	lastFrame = AVIStreamLength(pavi);						// The Last Frame Of The Stream
	fps = stream_info.dwRate / stream_info.dwScale;
	pointer_to_frame = AVIStreamGetFrameOpen(pavi, (LPBITMAPINFOHEADER)AVIGETFRAMEF_BESTDISPLAYFMT);              // Create The PGETFRAME Using Our Request Mode
	if (pointer_to_frame == NULL) {
		LOG("mdVideoplayer : Failed to open frame stream");
		return false;
	}

	frame_timer.start();
	is_playing = true;
	is_fullscreen = fullscreen;

	return true;
}

bool mdVideoplayer::grabAVIFrame() {
	LPBITMAPINFOHEADER lpbi;													 // Holds The Bitmap Header Information
	lpbi = (LPBITMAPINFOHEADER)AVIStreamGetFrame(pointer_to_frame, frame);					// Grab Data From The AVI Stream
	pdata = (char *)lpbi + lpbi->biSize + lpbi->biClrUsed * sizeof(RGBQUAD);    // Pointer To Data Returned By AVIStreamGetFrame
																				// (Skip The Header Info To Get To The Data)
																				// Convert Data To Requested Bitmap Format

																				/*
																				- pdata holds the texture data (pixels)
																				- biBitCount holds the depht in bits and is contained in the LPBITMAPINFOHEADER structure
																				- pitch is the length of a row of pixels in bytes (widht x 3)
																				*/
	SDL_Surface* surface = SDL_CreateRGBSurfaceFrom(pdata, width, height, lpbi->biBitCount, width * 3, 0, 0, 0, 0);
	SDL_Surface* flipped = App->textures->flipSurface(surface, FLIP_VERTICAL);
	frame_texture = App->textures->loadSurface(flipped);
	
	int position_x = 0;
	int position_y = 0;
	float scale = 1;
	if (is_fullscreen) {
		scale = App->render->resolution.first / (float)width;
	}
	else {
		position_x = (App->render->resolution.first - width) / 2;
		position_y = (App->render->resolution.second - height) / 2;
	}
	App->render->drawSprite(10, frame_texture, position_x, position_y, nullptr, scale, false, 1, 0, 1, 1, false);
	double frametime = 1000 / fps;

	if (frame_timer.read() >= frametime) {
		frame++;
		frame_timer.start();
	}
	if (frame >= lastFrame) {
		frame = 0;
		is_playing = false;
	}

	SDL_FreeSurface(surface);
	SDL_FreeSurface(flipped);

	return true;
}

bool mdVideoplayer::isPlaying() const {
	return is_playing;
}

void mdVideoplayer::stopPlaying() {
	is_playing = false;
	AVIStreamGetFrameClose(pointer_to_frame);                // Deallocates The GetFrame Resources

	AVIStreamRelease(pavi);                     // Release The Stream

	AVIFileExit();                              // Release The File
}
