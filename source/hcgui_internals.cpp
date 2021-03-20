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

	hcgui::DRAWING_AREA drawingArea = {
		{0, 0, 0, 0},
		nullptr,
		{0, 0},
		0};

	hcgui::EVENT_CONTAINER *p_eventQueue = nullptr;
	hcgui::EVENT_HANDLER *p_eventHandlers = nullptr;

	void setError(const char *error_message)
	{
		lastError = error_message;
	}

	bool initialize()
	{
		instanceActive = true;

		CONSOLE_SCREEN_BUFFER_INFO consoleBufferInfo = {0};
		if (getConsoleBufferInfo(&consoleBufferInfo) == hcgui::SystemState::Error)
		{
			return false;
		}

		cursorStartPosition = consoleBufferInfo.dwCursorPosition;

		/*
			This code block is to circumvent the WriteConsoleOutput limit preventing output outside of current window
			height. We simply write some newlines in a row to create an area that we can draw onto later on at the
			onDraw() call.
		*/
		{
			DWORD len = consoleBufferInfo.srWindow.Bottom + 1;
			char *str = (char *)malloc(sizeof(char) * len);
			for (WORD i = 0; i < len; i++)
				str[i] = '\n';
			str[len - 1] = '\0';
			printf(str);
		}

		// We read the buffer info one more time to get the updates information
		if (getConsoleBufferInfo(&consoleBufferInfo) == hcgui::SystemState::Error)
		{
			return false;
		}

		// Setup initial information about the output buffer
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

		// Create event handlers for all possible events to poll
		p_eventHandlers = new hcgui::EVENT_HANDLER[hcgui::EventType::EVENT_TYPE_COUNT];

		for (int i = 0; i < hcgui::EventType::EVENT_TYPE_COUNT; i++)
		{
			EVENT_HANDLER ev_h;
			ev_h.p_SubscriberList = nullptr;

			p_eventHandlers[i] = ev_h;
		}

		// Launch internal thread
		t_internalThread = std::thread(threadStart);
		return true;
	}

	void triggerEvent(hcgui::EVENT_INFO event_info)
	{
		hcgui::EVENT_HANDLER handler = p_eventHandlers[(int)event_info.EventType];
		hcgui::EVENT_SUBSCRIPTION_CONTAINER *current_container = handler.p_SubscriberList;

		bool event_pass_forward = true;
		while (event_pass_forward && current_container != nullptr)
		{
			event_pass_forward = current_container->Subscriber.CALLBACK_ADDR(event_info);
			current_container = current_container->p_Next;
		}
	}

	void scheduleEvent(hcgui::EVENT_INFO event_info)
	{
		hcgui::EVENT_CONTAINER *new_event = new hcgui::EVENT_CONTAINER{event_info, nullptr};

		// Append event at end of linked list
		if (p_eventQueue == nullptr)
		{
			p_eventQueue = new_event;
		}
		else
		{
			hcgui::EVENT_CONTAINER *current = p_eventQueue;
			while (current->p_Next != nullptr)
			{
				current = current->p_Next;
			}

			current->p_Next = new_event;
		}
	}

	bool pollEvents(hcgui::EVENT_INFO *event_out)
	{
		// If there's events in the queue
		if (p_eventQueue != nullptr)
		{
			// Grab first event container in the queue and remove it from the queue
			hcgui::EVENT_CONTAINER *current = p_eventQueue;
			p_eventQueue = p_eventQueue->p_Next;

			// Read event info and delete the container
			*event_out = current->INFO;
			delete current;

			return true;
		}
		return false;
	}

	void onDraw()
	{
		hcgui::setCursorPosition(0, 0);

		SHORT output_row = cursorStartPosition.Y;
		COORD buffer_origin = {0, 0};
		SMALL_RECT writeArea = {0, output_row, drawingArea.BufferCoords.X, (SHORT)(output_row + drawingArea.BufferCoords.Y)};

		WriteConsoleOutput(p_stdout, drawingArea.p_Buffer, drawingArea.BufferCoords, buffer_origin, &writeArea);
	}

	void threadStart() // <- t_internalThread starts here
	{
		hcgui::EVENT_INFO event;
		CONSOLE_SCREEN_BUFFER_INFO screenBufferInfo;

		// Internal running loop
		while (instanceActive)
		{
			if (hcgui::getConsoleBufferInfo(&screenBufferInfo) == SystemState::OK)
			{
				if (screenBufferInfo.srWindow.Right != drawingArea.WindowSize.Right ||
					screenBufferInfo.srWindow.Bottom != drawingArea.WindowSize.Bottom)
				{
					//printf("Prev/New Right: %d/%d\nPrev/New Bottom: %d/%d\n", drawingArea.WindowSize.Right, screenBufferInfo.srWindow.Right, drawingArea.WindowSize.Bottom, screenBufferInfo.srWindow.Bottom);
					drawingArea.WindowSize = screenBufferInfo.srWindow;
					COORD prev = drawingArea.BufferCoords;
					drawingArea.BufferCoords = {(short)(drawingArea.WindowSize.Right + 1), (short)(drawingArea.WindowSize.Bottom + 1)};

					hcgui::EVENT_INFO ev;
					ev.EventType = hcgui::EventType::BufferAreaResized;
					ev.bufferAreaResized.previousSize = prev;
					ev.bufferAreaResized.newSize = drawingArea.BufferCoords;
					scheduleEvent(ev);
				}
			}

			while (pollEvents(&event))
			{
				// Trigger the event if it is not internal
				if (!event.InternalOnly)
				{
					triggerEvent(event);
				}
				// Handle internal events
				else
				{
				}
			}

			onDraw();
		}

		// todo : clean up window to its original state
	}
}
