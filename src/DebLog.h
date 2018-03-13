#ifndef __DEBLOG__
#define __DEBLOG__
#include <windows.h>
#include <stdio.h>

#define LOG(format, ...) log(__FILE__, __LINE__, format, __VA_ARGS__)
void log(const char file[], int line, const char* format, ...);

#endif //__DEBLOG__