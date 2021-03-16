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

		drawingArea.CellBufferSize = {drawingArea.WindowSize.Right, drawingArea.WindowSize.Bottom};
		drawingArea.CellBufferLength = drawingArea.CellBufferSize.X * drawingArea.CellBufferSize.Y;
		drawingArea.p_CellBuffer = new CHAR_INFO[drawingArea.CellBufferLength];

		for (int i = 0; i < drawingArea.CellBufferLength; i++)
		{
			drawingArea.p_CellBuffer[i].Char.UnicodeChar = ' ';
			drawingArea.p_CellBuffer[i].Attributes =
				FOREGROUND_INTENSITY | FOREGROUND_RED |
				BACKGROUND_BLUE;
		}

		SHORT output_row = cursorStartPosition.Y;
		COORD buffer_origin = {0, 0};
		SMALL_RECT writeArea = {0, output_row, drawingArea.CellBufferSize.X, (SHORT)(output_row + drawingArea.CellBufferSize.Y)};

		//printf("L:%d T:%d R:%d B:%d", writeArea.Left, writeArea.Top, writeArea.Right, writeArea.Bottom);

		t_internalThread = std::thread(threadStart);

		return true;
	}

	// Called when window size != last stored window size
	void windowSizeChanged(CONSOLE_SCREEN_BUFFER_INFO &screenBufferInfo)
	{
		// todo: not implemented
	}

	void threadStart() // <--- t_internalThread starts here
	{
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
					windowSizeChanged(screenBufferInfo);
				}
			}

			// todo : now drawing past screen bottom
			SHORT output_row = cursorStartPosition.Y;
			COORD buffer_origin = {0, 0};
			SMALL_RECT writeArea = {0, output_row, drawingArea.CellBufferSize.X, (SHORT)(output_row + drawingArea.CellBufferSize.Y)};

			WriteConsoleOutput(p_stdout, drawingArea.p_CellBuffer, drawingArea.CellBufferSize, buffer_origin, &writeArea);
		}
	}
}
