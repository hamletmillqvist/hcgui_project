#include <stdio.h>
#include <Windows.h>
#include "hcgui/hcgui.h"

int main(int argc, char **argv)
{
	// Create HGCUI instance.
	if (hcgui::createInstance() == hcgui::SystemState::Error)
	{
		printf("Error initializing HCGUI library.\nError message: %s\n", hcgui::getError());
		return -1;
	}

	// Destroy HCGUI instance.
	hcgui::destroyInstance();
	return 0;
}
