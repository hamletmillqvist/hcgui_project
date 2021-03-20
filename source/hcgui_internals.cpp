#include "hcgui/hcgui_internals.h"
#include "hcgui/hcgui.h"

namespace hcgui
{
	bool instanceActive = false;
	const char *lastError = "";
	DWORD pid = 0;
	HANDLE p_stdout = nullptr;
	COORD cursorStartPosition;
	std::thread t_internalThread;

	DrawingArea drawingArea = {
		{0, 0, 0, 0},
		nullptr,
		{0, 0},
		0};

	void setError(const char *error_message)
	{
		lastError = error_message;
	}

	bool initialize()
	{
		instanceActive = true;

		// Read console buffer info in order to locate current cursor position
		CONSOLE_SCREEN_BUFFER_INFO consoleBufferInfo = {0};
		if (getConsoleBufferInfo(&consoleBufferInfo) == SystemState::Error)
		{
			return false;
		}

		cursorStartPosition = consoleBufferInfo.dwCursorPosition;
		drawingArea.WindowSize = consoleBufferInfo.srWindow;

		drawingArea.BufferCoords = {(short)(drawingArea.WindowSize.Right + 1), (short)(drawingArea.WindowSize.Bottom + 1)};
		drawingArea.BufferLenght = drawingArea.BufferCoords.X * drawingArea.BufferCoords.Y;
		drawingArea.p_Buffer = new CHAR_INFO[drawingArea.BufferLenght];

		for (int i = 0; i < drawingArea.BufferLenght; i++)
		{
			drawingArea.p_Buffer[i].Char.UnicodeChar = ' ';
			drawingArea.p_Buffer[i].Attributes =
				FOREGROUND_INTENSITY | FOREGROUND_RED |
				BACKGROUND_BLUE;
		}

		/*
			This code block is to circumvent the WriteConsoleOutput limit preventing output outside of current window
			height. We simply write some newlines in a row to create an area that we can draw onto later on at the
			onDraw() call.
		*/
		DWORD len = drawingArea.BufferCoords.Y;
		char *str = (char *)malloc(sizeof(char) * len);
		for (WORD i = 0; i < len; i++)
			str[i] = '\n';
		str[len - 1] = '\0';

		printf(str);

		t_internalThread = std::thread(threadStart);

		return true;
	}

	void onDraw()
	{
		SHORT output_row = cursorStartPosition.Y;
		COORD buffer_origin = {0, 0};
		SMALL_RECT writeArea = {0, output_row, drawingArea.BufferCoords.X, (SHORT)(output_row + drawingArea.BufferCoords.Y)};

		WriteConsoleOutput(p_stdout, drawingArea.p_Buffer, drawingArea.BufferCoords, buffer_origin, &writeArea);
	}

	// Called when window size != last stored window size
	void onWindowSizeChanged(CONSOLE_SCREEN_BUFFER_INFO &screenBufferInfo)
	{
		// todo: not implemented
	}

	void threadStart() // <--- t_internalThread starts here
	{
		// Internal running loop
		while (instanceActive)
		{
			CONSOLE_SCREEN_BUFFER_INFO screenBufferInfo;
			if (getConsoleBufferInfo(&screenBufferInfo) == SystemState::OK)
			{
				// If current window size != last stored window size
				if (screenBufferInfo.srWindow.Top != drawingArea.WindowSize.Top ||
					screenBufferInfo.srWindow.Right != drawingArea.WindowSize.Right ||
					screenBufferInfo.srWindow.Bottom != drawingArea.WindowSize.Bottom ||
					screenBufferInfo.srWindow.Left != drawingArea.WindowSize.Left)
				{
					onWindowSizeChanged(screenBufferInfo);
				}
			}

			onDraw();
		}

		// todo : clean up window to its original state
	}
}
