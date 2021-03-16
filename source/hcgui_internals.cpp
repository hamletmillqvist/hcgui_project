#include "hcgui/hcgui_internals.h"

namespace hcgui
{
	/////// Variables declared in hcgui_internals.h ///////
	bool instanceActive = false;
	const char *lastError = "";
	DWORD pid = 0;
	HANDLE p_stdout = nullptr;
	COORD cursorStartPosition;
	CHAR_INFO *p_drawingBuffer = nullptr;
	SMALL_RECT drawingArea = {0, 0, 0, 0};
	SMALL_RECT windowSize = {0, 0, 0, 0};

	void setError(const char *error_message)
	{
		lastError = error_message;
	}

	bool checkInstance()
	{
		if (!instanceActive)
		{
			setError("No instance of HCGUI running!\n");
			return false;
		}
		return true;
	}
}
