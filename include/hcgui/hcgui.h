#pragma once
#include <inttypes.h>
#include <Windows.h>
#include "hcgui_internals.h"

#ifndef DLLDIR_EX
#define DLL __declspec(dllexport)
#else
#define DLLDIR __declspec(dllimport)
#endif

namespace hcgui
{
	// External functions
	extern "C" DLL SystemState createInstance();
	extern "C" DLL void destroyInstance();
	extern "C" DLL const char *getError();
	extern "C" DLL SystemState getConsoleBufferInfo(CONSOLE_SCREEN_BUFFER_INFO *address_out);
	extern "C" DLL SystemState setCursorPosition(int16_t posX, int16_t posY);
}
