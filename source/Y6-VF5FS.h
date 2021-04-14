#pragma once

#include "RenderWindow.h"

using module_func_t = int(*)(size_t args, const void* argp);

namespace Y6
{
	namespace VF5FS
	{
		void Run(const RenderWindow& window);
		void GameLoop(module_func_t func);
	}
}