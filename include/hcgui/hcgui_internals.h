#pragma once
#include <thread>
#include <Windows.h>

namespace hcgui
{
	// Enum classed used in call returns.
	enum class SystemState
	{
		OK,
		Warning,
		Error
	};

	// If the current instance is initialized and is still running.
	extern bool instanceActive;

	// The last recorded error string.
	extern const char *lastError;

	// Internal thread performing all hcgui updates looping threadStart().
	extern std::thread t_internalThread;

	// Stored process ID.
	extern DWORD pid;

	// Handle to the console output.
	extern HANDLE p_stdout;

	// Initial cursor starting position when launchin the instance.
	extern COORD cursorStartPosition;

	// Cell drawing area data
	struct DrawingArea
	{
		// Last stored window state
		SMALL_RECT WindowSize;

		// Array storing information for all cells within the drawing area
		CHAR_INFO *p_CellBuffer;

		// Array size
		COORD CellBufferSize;

		// Size of p_CellBuffer
		uint32_t CellBufferLength;
	};

	extern DrawingArea drawingArea;

	// Sets last error message to the passed argument string
	void setError(const char *error_message);

	// Sets up initial values for the HCGUI instance.
	bool initialize();

	// Internal loop-method used by the t_internalThread.
	void threadStart();

	// Method called from threadStart() when a change in window-size is detected.
	void windowSizeChanged(CONSOLE_SCREEN_BUFFER_INFO &screenBufferInfo);
}
