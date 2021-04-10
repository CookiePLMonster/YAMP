#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "RenderWindow.h"
#include "Y6-VF5FS.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	RenderWindow window(hInstance, nShowCmd);

	Y6::VF5FS::Run(window);

	return 0;
}