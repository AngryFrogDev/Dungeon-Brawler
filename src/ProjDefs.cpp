#include "ProjDefs.h"

void releasePointer(void* x, bool is_array) {
	if (x != nullptr && x != NULL) {
		if (is_array)
			delete[] x;
		else
			delete x;
		x = nullptr;
	}
}

