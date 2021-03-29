#include "hcgui/hcgui_internals.h"
#include "hcgui/hcgui.h"
#include <csignal>

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

	LinkedList eventQueue;
	hcgui::EventHandler *p_eventHandlers = nullptr;

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
		size_t memsize = sizeof(hcgui::EventHandler) * hcgui::EventType::EVENT_TYPE_COUNT;
		p_eventHandlers = (hcgui::EventHandler *)malloc(memsize);

		for (int i = 0; i < hcgui::EventType::EVENT_TYPE_COUNT; i++)
		{
			p_eventHandlers[i] = hcgui::EventHandler((hcgui::EventType)i);
		}

		eventQueue = LinkedList();

		// Launch internal thread
		t_internalThread = std::thread(threadStart);
		return true;
	}

	void invokeEvent(hcgui::EVENT_INFO event_info)
	{
		hcgui::EventHandler handler = p_eventHandlers[(int)event_info.EventType];
		handler.triggerEvent(event_info);
	}

	void scheduleEvent(hcgui::EVENT_INFO *event_info)
	{
		eventQueue.emplaceNode(event_info);
	}

	bool pollEvents(hcgui::EVENT_INFO *event_out)
	{
		// If queue is not empty
		if (eventQueue.getCount() > 0)
		{
			// Read node at head and extract object
			LINKED_NODE *event_node = eventQueue.getHead();
			hcgui::EVENT_INFO *node_obj = (hcgui::EVENT_INFO *)event_node->p_Object;

			// Copy event information into the out parameter
			*event_out = *node_obj;

			// Destroy object and node
			delete node_obj; // TODO : MEMORY ACCESS VIOLATION

			if (eventQueue.removeNode(event_node))
			{
				return true;
			}
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
		bool isDone = false;

		// Internal running loop
		while (instanceActive)
		{
			if (hcgui::getConsoleBufferInfo(&screenBufferInfo) == SystemState::OK)
			{
				if (screenBufferInfo.srWindow.Right != drawingArea.WindowSize.Right ||
					screenBufferInfo.srWindow.Bottom != drawingArea.WindowSize.Bottom)
				{
					//printf("Prev/New Right: %d/%d\nPrev/New Bottom: %d/%d\n", drawingArea.WindowSize.Right, screenBufferInfo.srWindow.Right, drawingArea.WindowSize.Bottom, screenBufferInfo.srWindow.Bottom);
					if (!isDone)
					{
						drawingArea.WindowSize = screenBufferInfo.srWindow;
						COORD prev = drawingArea.BufferCoords;
						drawingArea.BufferCoords = {(short)(drawingArea.WindowSize.Right + 1), (short)(drawingArea.WindowSize.Bottom + 1)};

						hcgui::EVENT_INFO *new_event = new hcgui::EVENT_INFO();
						new_event->EventType = hcgui::EventType::BufferAreaResized;
						new_event->bufferAreaResized.previousSize = prev;
						new_event->bufferAreaResized.newSize = drawingArea.BufferCoords;
						scheduleEvent(new_event);
						isDone = true;
					}
				}
			}

			while (pollEvents(&event))
			{
				// Trigger the event if it is not internal
				if (!event.InternalOnly)
				{
					invokeEvent(event);
				}
				// Handle internal events
				else
				{
					hcgui::CreateWindowsPopup("Not Implemented", "Internal events have not been implemented!");
				}
			}
			onDraw();
		}

		// todo : clean up window to its original state
	}
}
