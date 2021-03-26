#include <stdio.h>
#include <Windows.h>
#include <sstream>
#include "hcgui/hcgui.h"

bool OnBufferResized(hcgui::EVENT_INFO event_info)
{
	std::stringstream ss;
	COORD old_size = event_info.bufferAreaResized.previousSize;
	COORD new_size = event_info.bufferAreaResized.newSize;
	ss << "Previous size: (" << old_size.X << ";" << old_size.Y << ")\nNew size: (" << new_size.X << ";" << new_size.Y << ")";

	hcgui::CreateWindowsPopup("Buffer was resized!", ss.str().c_str());
	return true;
}

int main(int argc, char **argv)
{
	if (hcgui::createInstance() == hcgui::SystemState::Error)
	{
		printf("Error initializing HCGUI library.\nError message: %s\n", hcgui::getError());
		return -1;
	}

	// id is used to unsubscribe at a later point
	DWORD subscription_id = hcgui::addEventSubscriber(hcgui::EventType::BufferAreaResized, &OnBufferResized);

	while (hcgui::checkInstance())
	{
		//if (!strcmp(hcgui::getError(), hcgui::NO_ERROR_STR))
		//{
		//	std::stringstream ss;
		//	ss << "HCGUI Error caught: " << hcgui::getError();
		//	hcgui::CreateWindowsPopup("Buffer was resized!", ss.str().c_str());
		//	hcgui::resetError();
		//}
	}

	hcgui::destroyInstance();
	printf("Shutting down...\n");
	return 0;
}
