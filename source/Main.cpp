#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "Y6-VF5FS.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	Y6::VF5FS::Run();

	return 0;
}