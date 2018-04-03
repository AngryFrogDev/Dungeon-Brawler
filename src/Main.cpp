#include <stdlib.h>
#include "Application.h"
#include "DebLog.h"
#include "ProjDefs.h"
// This is needed here because SDL redefines main function and Brofiler needs to identify it
// do not add any other libraries here, instead put them in their modules
#include "SDL/include/SDL.h"
#pragma comment( lib, "SDL/libx86/SDL2.lib" )
#pragma comment( lib, "SDL/libx86/SDL2main.lib" )

#include "Brofiler/Brofiler.h"
#pragma comment( lib, "Brofiler/ProfilerCore32.lib" )

enum MainState {
	CREATE = 0,
	AWAKE,
	START,
	LOOP,
	CLEAN,
	FAIL,
	EXIT
};

Application* App = NULL;

int main(int argc, char* args[]) {
	LOG("Engine starting ... %d");

	MainState state = MainState::CREATE;
	int result = EXIT_FAILURE;

	while (state != EXIT) {
		switch (state) {

			// Allocate the engine --------------------------------------------
		case CREATE:
			LOG("CREATION PHASE ===============================");

			App = new Application(argc, args);

			if (App != NULL)
				state = AWAKE;
			else
				state = FAIL;

			break;

			// Awake all modules -----------------------------------------------
		case AWAKE:
			LOG("AWAKE PHASE ===============================");
			if (App->awake() == true)
				state = START;
			else {
				LOG("ERROR: Awake failed");
				state = FAIL;
			}

			break;

			//Start all modules
		case START:
			LOG("Start PHASE ===============================");
			if (App->start() == true)
				state = LOOP;
			else {
				LOG("ERROR: Start failed");
				state = FAIL;
			}

			break;


			// Loop all modules until we are asked to leave ---------------------
		case LOOP: {
			BROFILER_FRAME("Frame");
			if (App->update() == false)
				state = CLEAN;
			break;
		}

			// Cleanup allocated memory -----------------------------------------
		case CLEAN:
			LOG("CLEANUP PHASE ===============================");
			if (App->cleanUp() == true) {
				RELEASE(App);
				result = EXIT_SUCCESS;
				state = EXIT;
			}
			else
				state = FAIL;

			break;

			// Exit with errors and shame ---------------------------------------
		case FAIL:
			LOG("Something failed, exiting");
			result = EXIT_FAILURE;
			state = EXIT;
			break;
		}
	}

	LOG("Exiting application\n");

	// Dump memory leaks
	return result;
}