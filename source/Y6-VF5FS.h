#pragma once

#include "RenderWindow.h"

using module_func_t = int(*)(size_t args, const void* argp);

namespace Y6
{
	namespace VF5FS
	{
		HMODULE LoadDLL();
		void Run(RenderWindow& window);
		bool GameLoop(module_func_t func, RenderWindow& window);
	}
}