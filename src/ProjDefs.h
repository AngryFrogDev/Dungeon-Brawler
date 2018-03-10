#ifndef __PROJDEFS__
#define __PROJDEFS__

#include <type_traits>

#ifdef NULL
	#undef NULL
#endif
#define NULL 0

#define RELEASE(x) releasePointer(x, false)
#define RELEASE_ARRAY(x) releasePointer(x, true)

void releasePointer(void* x, bool is_array) {
	if (x != nullptr) {
		if (is_array)
			delete[] x;
		else
			delete x;
		x = nullptr;
	}
}

typedef unsigned int uint;
typedef unsigned __int32 uint32;
typedef unsigned __int64 uint64;
typedef unsigned char uchar;

#endif