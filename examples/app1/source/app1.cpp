#include <stdio.h>
#include <Windows.h>
#include <sstream>
#include <string>
#include "hcgui/hcgui.h"

int main(int argc, char **argv)
{
	if (hcgui::createInstance() == hcgui::SystemState::Error)
	{
		printf("Error initializing HCGUI library.\nError message: %s\n", hcgui::getError());
		return -1;
	}

	FRAME_HANDLE my_frame = hcgui::createFrame(nullptr, "MyFrame", {5, 5, 10, 6});

	while (hcgui::checkInstance())
	{
		if (!strcmp(hcgui::getError(), hcgui::NO_ERROR_STR))
		{
			std::stringstream ss;
			ss << "HCGUI Error caught: " << hcgui::getError();
			hcgui::createWindowsPopup("Buffer was resized!", ss.str().c_str());
			hcgui::resetError();
		}
	}

	hcgui::destroyInstance();
	printf("Shutting down...\n");
	return 0;
}
