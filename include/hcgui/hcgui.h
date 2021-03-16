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
	// Creates the HCGUI instance. This should be called at the start before any other methods.
	extern "C" DLL SystemState createInstance();

	// Destroys the HCGUI instance. Make sure to do this last.
	extern "C" DLL void destroyInstance();

	// checkInstance will return TRUE if HCGUI is initialized and has not been destroyed.
	extern "C" DLL bool checkInstance();

	// Retreives the last recorded error message.
	extern "C" DLL const char *getError();

	// Read current CONSOLE_SCREEN_BUFFER_INFO from the instance.
	extern "C" DLL SystemState getConsoleBufferInfo(CONSOLE_SCREEN_BUFFER_INFO *address_out);

	// Moves the cursor to absolute x and y coordinates.
	extern "C" DLL SystemState setCursorPosition(int16_t posX, int16_t posY);
}
