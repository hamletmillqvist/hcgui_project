#pragma once
#include <inttypes.h>
#include <Windows.h>
#include <stdint.h>
#include "hcgui_internals.h"

#ifndef DLLDIR_EX
#define DLL __declspec(dllexport)
#else
#define DLLDIR __declspec(dllimport)
#endif

namespace hcgui
{
	const char *const NO_ERROR_STR = "NO_ERROR";

	// Creates the HCGUI instance. This should be called at the start before any other methods.
	extern "C" DLL SystemState createInstance();

	// Destroys the HCGUI instance. Make sure to do this last.
	extern "C" DLL void destroyInstance();

	// Returns TRUE if HCGUI is initialized and has not been destroyed.
	extern "C" DLL bool checkInstance();

	// Retreives the last recorded error message.
	extern "C" DLL const char *getError();

	// Removes the currently stored error message.
	extern "C" DLL void resetError();

	// Read current CONSOLE_SCREEN_BUFFER_INFO from the instance.
	extern "C" DLL SystemState getConsoleBufferInfo(CONSOLE_SCREEN_BUFFER_INFO *address_out);

	// Moves the cursor to x and y coordinates on the drawing area.
	extern "C" DLL SystemState setCursorPosition(int16_t posX, int16_t posY);

	// Add a new subscriber (observer) to an event handler. callback_addr needs to be the function called on event trigger. Returns a callback_id
	extern "C" DLL uint32_t addEventSubscriber(hcgui::EventType eventType, bool (*callback_addr)(hcgui::EVENT_INFO));

	// Removes a subscriber (observer) from an event handler. Second arguments needs to be the id of the subscriber to remove.
	extern "C" DLL void removeEventSubscriber(hcgui::EventType eventType, uint32_t callback_id);

	// Windows only function that creatues a normal popup window with a title and message. Mostly used for debugging right now.
	extern "C" DLL void CreateWindowsPopup(const char *title, const char *message);
}
