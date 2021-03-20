#include <stdio.h>

#include "hcgui/hcgui.h"
#include "hcgui/hcgui_internals.h"

namespace hcgui
{
	DLL void CreateWindowsPopup(const char *title, const char *message)
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
		return hcgui::setCursorPositionAbsolute(cursorStartPosition.X + posX, cursorStartPosition.Y + posY);
	}

	DLL SystemState setCursorPositionAbsolute(int16_t posX, int16_t posY)
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

	DLL DWORD addEventListener(hcgui::EventType eventType, bool (*callback_addr)(hcgui::EVENT_INFO))
	{
		if (!checkInstance())
		{
			return -1;
		}

		// Get event handler for specified event type
		hcgui::EVENT_HANDLER &handler = p_eventHandlers[(int)eventType];

		// Get subscriber list for event handler
		hcgui::EVENT_SUBSCRIPTION_CONTAINER *current_container = handler.p_SubscriberList;

		DWORD subscriber_id = 0;

		// If list is not empty, we go to the last container and attach the new one on top of it
		if (current_container != nullptr)
		{
			subscriber_id = 1;
			while (current_container->p_Next != nullptr)
			{
				subscriber_id++;
				current_container = current_container->p_Next;
			}

			current_container->p_Next = new hcgui::EVENT_SUBSCRIPTION_CONTAINER{
				hcgui::EVENT_SUBSCRIBER{subscriber_id, callback_addr},
				nullptr};
		}
		// Else set the new one as first
		else
		{
			handler.p_SubscriberList = new hcgui::EVENT_SUBSCRIPTION_CONTAINER{
				hcgui::EVENT_SUBSCRIBER{subscriber_id, callback_addr},
				nullptr};
		}

		return subscriber_id;
	}

	DLL void removeEventListener(hcgui::EventType eventType, WORD subscriber_id)
	{
		if (!checkInstance())
		{
			return;
		}

		// Get event handler for specified event type
		hcgui::EVENT_HANDLER handler = p_eventHandlers[(int)eventType];

		// Get subscriber list for event handler
		hcgui::EVENT_SUBSCRIPTION_CONTAINER *current_container = handler.p_SubscriberList,
											*found = nullptr;

		// There are subscriber
		if (current_container != nullptr)
		{
			// Current subscriber is not the one we're looking for
			if (current_container->Subscriber.ID != subscriber_id)
			{
				// Loop untill we find the target
				while (current_container->p_Next != nullptr)
				{
					// Target found is next node
					if (current_container->p_Next->Subscriber.ID == subscriber_id)
					{
						found = current_container->p_Next;
						current_container->p_Next = current_container->p_Next->p_Next;
						delete found;
					}
					else
					{
						current_container = current_container->p_Next;
					}
				}
			}
			else
			{
				found = current_container;
				current_container = current_container->p_Next;

				delete found;
			}
		}
	}
}
