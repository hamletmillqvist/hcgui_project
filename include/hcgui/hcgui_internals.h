#pragma once
#include <Windows.h>

namespace hcgui
{
	// Constants
	enum class SystemState
	{
		OK,
		Warning,
		Error
	};

	/////// Variables defined in hcgui_internals.cpp ///////

	extern bool instanceActive;
	extern const char *lastError;

	// Identifier variables
	extern DWORD pid;
	extern HANDLE p_stdout;

	// Initial cursor starting position when launchin the instance.
	extern COORD cursorStartPosition;

	// Cell drawing area data
	extern SMALL_RECT drawingArea;
	extern SMALL_RECT windowSize;
	extern CHAR_INFO *p_drawingBuffer; // array

	void setError(const char *error_message);
	bool checkInstance();
}
