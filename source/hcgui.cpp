#include <stdio.h>

#include "hcgui/hcgui.h"
#include "hcgui/hcgui_internals.h"

namespace hcgui
{
	DLL SystemState createInstance()
	{
		if (instanceActive)
		{
			setError("Can only run one instance at a time!");
			return SystemState::Error;
		}

		// Store process ID and stdoput handle
		pid = GetCurrentProcessId();
		if (!pid)
		{
			setError("Could not read process ID.\n");
			return SystemState::Error;
		}

		p_stdout = GetStdHandle(STD_OUTPUT_HANDLE);
		if (p_stdout == NULL)
		{
			setError("Could not fetch stdout handle.\n");
			return SystemState::Error;
		}

		if (!initialize())
		{
			instanceActive = false;
			return SystemState::Error;
		}

		setError("NO_ERROR\n");
		return SystemState::OK;
	}

	DLL void destroyInstance()
	{
		instanceActive = false;
		pid = 0;
		p_stdout = nullptr;
	}

	DLL bool checkInstance()
	{
		if (!instanceActive)
		{
			setError("No instance of HCGUI running!\n");
			return false;
		}
		return true;
	}

	DLL const char *getError()
	{
		return lastError;
	}

	DLL SystemState getConsoleBufferInfo(CONSOLE_SCREEN_BUFFER_INFO *address_out)
	{
		if (!instanceActive)
		{
			setError("No instance of HCGUI running!\n");
			return SystemState::Error;
		}

		if (!GetConsoleScreenBufferInfo(p_stdout, address_out))
		{
			setError("Could not read console information.\n");
			return SystemState::Error;
		}

		return SystemState::OK;
	}

	DLL SystemState setCursorPosition(int16_t posX, int16_t posY)
	{
		if (!checkInstance())
		{
			return SystemState::Error;
		}

		COORD coord = {posX, posY};
		if (!SetConsoleCursorPosition(p_stdout, coord))
		{
			setError("Write-failure to console buffer. Could not set curor position!");
			return SystemState::Error;
		}

		return SystemState::OK;
	}
}
