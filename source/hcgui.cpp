#include <stdio.h>

#include "hcgui/hcgui.h"
#include "hcgui/hcgui_internals.h"

namespace hcgui
{
	DLL FRAME_HANDLE createFrame(FRAME_HANDLE p_owner, const char *p_title, SMALL_RECT rect)
	{
		FRAME_HANDLE frame = new Frame(p_owner, p_title);
		frame->setRect(rect);

		hcgui::frameList.emplaceNode(frame);

		return frame;
	}

	DLL void createWindowsPopup(const char *title, const char *message)
	{
		MessageBoxA(NULL, message, title, MB_OK);
	}

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

		return SystemState::OK;
	}

	DLL void destroyInstance()
	{
		if (!instanceActive)
			return;

		instanceActive = false;
		pid = 0;
		p_stdout = nullptr;

		delete p_eventHandlers;
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

	DLL void resetError()
	{
		setError(hcgui::NO_ERROR_STR);
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

	DLL uint32_t addEventSubscriber(hcgui::EventType eventType, bool (*callback_addr)(hcgui::EventInfo))
	{
		if (!checkInstance())
		{
			return -1;
		}

		return p_eventHandlers[(int)eventType].addSubscriber(callback_addr);
	}

	DLL void removeEventSubscriber(hcgui::EventType eventType, uint32_t subscriber_id)
	{
		if (!checkInstance())
		{
			return;
		}

		p_eventHandlers[(int)eventType].removeSubscriber(subscriber_id);
	}
}
