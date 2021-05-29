#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "RenderWindow.h"
#include "Y6-VF5FS.h"
#include "imgui/imgui.h"

#include "wil/com.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	// TODO: This is a hack, currently shutdown crashes because of mismatched allocators
	// Once this is handled, remove this
	auto shutdownHack = wil::scope_exit([] { TerminateProcess(GetCurrentProcess(), 0); });
	auto coinit = wil::CoInitializeEx(COINIT_MULTITHREADED);

	IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = nullptr;
    io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;

	HMODULE dll = Y6::VF5FS::LoadDLL();
	if (dll == nullptr)
	{
		// TODO: Show a native error message about game DLL not found
		return -1;
	}

	Y6::VF5FS::PreInitialize();

	RenderWindow window(hInstance, dll, nShowCmd);

	Y6::VF5FS::Run(window);

	return 0;
}